#include <utils/TextureManager.hpp>

static QMap<QString, unsigned int> textures;
unsigned int TextureManager::loadTexture(const char *path)
{
    if (textures.contains(path))
    {
        return textures[path];
    }

    
    
    return 0;
}