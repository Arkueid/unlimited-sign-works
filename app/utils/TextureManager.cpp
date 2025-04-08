#include <utils/TextureManager.hpp>
#include <QDebug>
#include <sqlite3/SQLite.hpp>
#include <QFile>
#include <QDir>
#include <QMessageBox>

static QMap<QString, QOpenGLTexture *> textures;
static QMap<int, QString> texturePaths;

TextureSignals *TextureManager::texturesSignals = nullptr;

QOpenGLTexture *TextureManager::loadTexture(const QString &path)
{
    // qDebug() << "Loading texture: " << path;

    if (textures.contains(path))
    {
        return textures[path];
    }

    QImage image("../../assets/textures/" + path);

    if (!image.isNull())
    {
        QOpenGLTexture *texture = new QOpenGLTexture(image.mirrored().convertToFormat(QImage::Format_RGBA8888));
        texture->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
        texture->setMinificationFilter(QOpenGLTexture::Linear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        textures[path] = texture;
        // qDebug() << "Loaded texture: " << path;
        return texture;
    }
    else
    {
        qDebug() << "Failed to load texture: " << path;
    }

    return nullptr;
}

QOpenGLTexture *TextureManager::getTexture(int ref)
{
    if (!texturePaths.contains(ref)) 
    {
        return loadTexture("default/default.png");
    }

    const QString &path = texturePaths[ref];
    return loadTexture(path);
}

const QMap<int, QString>& TextureManager::getTexturePaths()
{
    return texturePaths;
}
void TextureManager::releaseTextures()
{
    qDeleteAll(textures);
    textures.clear();
}

void TextureManager::loadTexturesPaths()
{
    texturePaths.clear();

    sqlite3 *db = SQLite::getInstance();

    sqlite3_stmt *stmt;
    if (sqlite3_prepare(db, "select id, name from texture order by id asc", -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const int id = sqlite3_column_int(stmt, 0);
            texturePaths[id] = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        }
    }
    else
    {
        const char *errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while loading texture paths: " << errMsg;
    }
    sqlite3_finalize(stmt);
}

int TextureManager::registerTexture(const QString &externalPath, const QString &textureName)
{
    QDir dir = QFileInfo("../../assets/textures/" + textureName).dir();

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    QFile::copy(externalPath, "../../assets/textures/" + textureName);

    sqlite3 *db = SQLite::getInstance();

    std::string textureNameStr = textureName.toUtf8().toStdString();
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "insert into texture (name) values (?);", -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, textureNameStr.c_str(), -1, SQLITE_STATIC);
        int ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            const char *errMsg = sqlite3_errmsg(db);
            qDebug() << "Error while registering texture: " << errMsg;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        const char *errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while registering texture: " << errMsg;
    }

    int id;
    if (sqlite3_prepare_v2(db, "select id from texture where name = ?;", -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, textureNameStr.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0);
        }
        else
        {
            const char *errMsg = sqlite3_errmsg(db);
            qDebug() << "Error while getting texture id: " << errMsg;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        const char *errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while getting texture id: " << errMsg;
    }

    texturePaths[id] = textureName;

    emit texturesSignals->texturesUpdated();
    return id;
}
bool TextureManager::unregisterTexture(const QString &textureName)
{
    int key = texturePaths.key(textureName);
    if (key <= 9)
    {
        QMessageBox::warning(nullptr, "错误", "不能删除默认纹理");
        return false;
    }

    QString filePath = "../../assets/textures/" + textureName;
    if (QFile::exists(filePath))
    {
        QFile::remove(filePath);
    }

    sqlite3 *db = SQLite::getInstance();
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "delete from texture where name = ?;", -1, &stmt, nullptr) == SQLITE_OK)
    {
        std::string textureNameStr = textureName.toUtf8().toStdString();
        sqlite3_bind_text(stmt, 1, textureNameStr.c_str(), -1, SQLITE_STATIC);
        int ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            const char *errMsg = sqlite3_errmsg(db);
            qDebug() << "Error while unregistering texture: " << errMsg;
            return false;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        const char* errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while unregistering texture: " << errMsg;
        return false;
    }

    texturePaths.remove(key);

    emit texturesSignals->texturesUpdated();
    return true;
}
void TextureManager::initSignals()
{
    texturesSignals = new TextureSignals();
}

void TextureManager::destroySignals()
{
    delete texturesSignals;
}
