#pragma once

#include <QMap>
#include <QOpenGLTexture>

class TextureManager {    
public:
    static QOpenGLTexture* loadTexture(const char *path);
    static const std::vector<std::string>* getTexturePaths();
    static void releaseTextures();
};