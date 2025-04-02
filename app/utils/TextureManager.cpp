#include <utils/TextureManager.hpp>
#include <QDebug>
#include "TextureManager.hpp"

static QMap<QString, QOpenGLTexture*> textures;
static QStringList texturePaths = {
    "../../assets/textures/panel/CN_road_sign.png",
    "../../assets/textures/panel/CN_road_sign2.png",
    "../../assets/textures/metal/metal1.bmp",
    "../../assets/textures/metal/metal2.bmp",
    "../../assets/textures/metal/metal3.bmp",
    "../../assets/textures/metal/metal4.bmp",
    "../../assets/textures/metal/metal5.bmp",
    "../../assets/textures/metal/metal6.bmp",
    "../../assets/textures/metal/metal7.bmp",
};

QOpenGLTexture* TextureManager::loadTexture(const QString& path)
{
    // qDebug() << "Loading texture: " << path;

    if (textures.contains(path))
    {
        return textures[path];
    }

    QImage image(path);

    if (!image.isNull())
    {
        QOpenGLTexture* texture = new QOpenGLTexture(image.mirrored().convertToFormat(QImage::Format_RGBA8888));
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
    const QString& path = texturePaths[ref];
    return loadTexture(path);
}

QStringList *TextureManager::getTexturePaths()
{
    return &texturePaths;
}
QStringList TextureManager::getTextureNames()
{
    QStringList names;
    for (const QString& path : texturePaths)
    {
        QString p = path;
        names.append(p.replace("../../assets/textures/", ""));
    }
    return names;
}
void TextureManager::releaseTextures()
{
    qDeleteAll(textures);
    textures.clear();
}
