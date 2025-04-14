#include <utils/TextureManager.hpp>
#include <QDebug>
#include <sqlite3/SQLite.hpp>
#include <QFile>
#include <QDir>
#include <QMessageBox>

// 缓存
static QMap<QString, QOpenGLTexture *> textures;
// id, name
static QMap<int, TextureItem> texturePaths;

static QMap<int, QString> publicTexturePaths;

TextureSignals *TextureManager::texturesSignals = nullptr;

QOpenGLTexture *TextureManager::loadTexture(const QString &path)
{
    // qDebug() << "Loading texture: " << path;

    if (textures.contains(path))
    {
        return textures[path];
    }

    QImage image(path);

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
        return loadTexture("../../assets/textures/default/default.bmp");
    }

    const char* prefix = texturePaths[ref].unique ? "../../assets/textures/generated/"  : "../../assets/textures/";
    const QString &path = prefix + texturePaths[ref].path;
    return loadTexture(path);
}

const QMap<int, QString>& TextureManager::getTexturePaths()
{
    publicTexturePaths.clear();
    for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it) 
    {
        if (it.value().unique)
        {
            continue;
        }
        publicTexturePaths.insert(it.key(), it.value().path);
    }
    return publicTexturePaths;
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
            bool unique = sqlite3_column_int(stmt, 1) == 1;
            texturePaths[id] = {QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))), unique};
        }
    }
    else
    {
        const char *errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while loading texture paths: " << errMsg;
    }
    sqlite3_finalize(stmt);
}

int TextureManager::registerTexture(const QString &externalPath, const QString &textureName, bool unique)
{
    QDir dir = QFileInfo("../../assets/textures/" + textureName).dir();

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    if (!unique)
    {
        QFile::copy(externalPath, "../../assets/textures/" + textureName);
    }

    sqlite3 *db = SQLite::getInstance();

    std::string textureNameStr = textureName.toUtf8().toStdString();
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "insert into texture (name, is_private) values (?, ?);", -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, textureNameStr.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, unique ? 1 : 0);
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

    texturePaths[id] = {textureName, unique};

    emit texturesSignals->texturesUpdated();
    return id;
}
bool TextureManager::unregisterTexture(const QString &textureName)
{
    int key;
    for (auto k : texturePaths.keys())
    {
        if (texturePaths[k].path == textureName)
        {
            key = k;
            break;
        }
    }
    if (key <= 9)
    {
        QMessageBox::warning(nullptr, "警告", "不能删除默认纹理");
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
