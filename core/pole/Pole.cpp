#include <pole/Pole.hpp>
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

void Pole::addPart(Part *part)
{
    parts.push_back(part);
}

void Pole::removePart(Part *part)
{
    parts.erase(std::find(parts.begin(), parts.end(), part));
    delete part;
}
