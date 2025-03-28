#include <utils/TextureManager.hpp>
#include <QDebug>

static QMap<QString, QOpenGLTexture*> textures;
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

void TextureManager::releaseTextures()
{
    qDeleteAll(textures);
    textures.clear();
}
