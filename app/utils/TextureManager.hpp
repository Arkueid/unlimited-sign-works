#pragma once

#include <QMap>
#include <QOpenGLTexture>

class TextureManager {    
public:
    static void loadTexturesPaths();
    static QOpenGLTexture* loadTexture(const QString& path);
    static QOpenGLTexture* getTexture(int ref);
    static QStringList* getTexturePaths();
    static QStringList getTextureNames();
    static void releaseTextures();
};