#pragma once

#include <QMap>
#include <QOpenGLTexture>

class TextureManager {    
public:
    static QOpenGLTexture* loadTexture(const char *path);
    static void releaseTextures();
};