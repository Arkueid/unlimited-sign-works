#include <utils/TextureManager.hpp>
#include <QDebug>
#include <mysql/MySQLManager.hpp>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "TextureManager.hpp"

struct TextureItem
{
    QString path;
    bool unique;
};

// 缓存
static QMap<QString, QOpenGLTexture*> textures;
// id, name
static QMap<int, TextureItem> texturePaths;

static QMap<int, QString> publicTexturePaths;

TextureSignals *TextureManager::texturesSignals = nullptr;
QOpenGLTexture* TextureManager::loadTexture(const QString &path)
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

QOpenGLTexture* TextureManager::getTexture(int ref)
{
    if (ref == -2)
    {
        return loadTexture("../../assets/textures/default/面板.bmp");
    }

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
void TextureManager::checkTextureLeak()
{
    if (!textures.isEmpty()) 
    {
        qDebug() << "Texture leak detected: " << textures.size();
    }
}

void TextureManager::releaseTextures() 
{
    qDeleteAll(textures);
    textures.clear();
}

void TextureManager::loadTexturesPaths()
{
    texturePaths.clear();

    QSqlDatabase db = MySQLManager::getDatabase();

    QSqlQuery query(db);
    if (!query.exec("select * from texture order by id asc"))
    {
        qDebug() << "Error while loading textures: " << query.lastError().text();
        return;
    }

    while (query.next()) 
    {
        const int id = query.value(0).toInt();
        const QString &textureName = query.value(1).toString();
        const bool unique = query.value(2).toBool();
        texturePaths[id] = {textureName, unique};
    }
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

    QSqlDatabase db = MySQLManager::getDatabase();

    std::string textureNameStr = textureName.toUtf8().toStdString();

    QSqlQuery  query(db);
    query.prepare("insert into texture (path, is_private) values (?, ?);");
    query.bindValue(0, textureName);
    query.bindValue(1, unique);
    if (query.exec()) 
    {
        int id = query.lastInsertId().toInt();
        texturePaths[id] = {textureName, unique};
        emit texturesSignals->texturesUpdated();
        return id;
    }
    else 
    {
        qDebug() << "Error while registering texture: " << query.lastError().text();
    }

    return -1;
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

    QSqlDatabase db = MySQLManager::getDatabase();
    QSqlQuery query(db);
    query.prepare("delete from texture where id = ?");
    query.bindValue(0, key);
    if (!query.exec())
    {
        qDebug() << "Error while unregistering texture: " << query.lastError().text();
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

const QString & TextureManager::getTextureName(int id)
{
    return texturePaths[id].path;
}
