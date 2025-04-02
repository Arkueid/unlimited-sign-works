#include <utils/TextureManager.hpp>
#include <QDebug>
#include <sqlite3/SQLite.hpp>

static QMap<QString, QOpenGLTexture *> textures;
static QMap<int, QString> texturePaths;

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
        texture->setWrapMode(QOpenGLTexture::ClampToEdge);
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
        return loadTexture("../../assets/textures/default/default.png");
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
            QString path = "../../assets/textures/" + QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
            texturePaths[id] = path;
        }
    }
    else
    {
        const char *errMsg = sqlite3_errmsg(db);
        qDebug() << "Error while loading texture paths: " << errMsg;
    }
    sqlite3_finalize(stmt);
}