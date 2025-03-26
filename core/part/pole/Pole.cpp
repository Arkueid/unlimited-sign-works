#include <part/pole/Pole.hpp>

Pole::Pole()
{
}
void Pole::createFromJsonFile(const char* fileName)
{
    
}

std::vector<Part>& Pole::getParts()
{
    return parts;
}

std::vector<std::string> &Pole::getTextures()
{
    return textures;
}
