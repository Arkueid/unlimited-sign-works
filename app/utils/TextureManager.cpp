#include <utils/TextureManager.hpp>
#include <QDebug>
#include "TextureManager.hpp"

static QMap<QString, QOpenGLTexture*> textures;
static std::vector<std::string> texturePaths = {
    "../../assets/textures/panel/D25.bmp",
    "../../assets/textures/metal/metal6.bmp",
    "../../assets/textures/metal/metal7.bmp",
    "../../assets/textures/metal/metal3.bmp",
    "../../assets/textures/metal/metal5.bmp"
};

QOpenGLTexture* TextureManager::loadTexture(const char *path)
{
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
        return texture;
    }
    else
    {
        qDebug() << "Failed to load texture: " << path;
    }
    
    return nullptr;
}

const std::vector<std::string> *TextureManager::getTexturePaths()
{
    return &texturePaths;
}
void TextureManager::releaseTextures()
{
    qDeleteAll(textures);
    textures.clear();
}
