#include <part/pole/Pole.hpp>
#include "Pole.hpp"

Pole::Pole()
{
}
void Pole::createFromJsonFile(const char* fileName)
{
    
}

Pole::~Pole()
{
    for (auto part : parts)
    {
        delete part;
    }
    parts.clear();
}
std::vector<Part*>& Pole::getParts()
{
    return parts;
}

std::vector<std::string> &Pole::getTextures()
{
    return textures;
}
