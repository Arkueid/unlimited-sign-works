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

    for (auto shape : shapes)
    {
        delete shape;
    }
    shapes.clear();
}
std::vector<Part*>& Pole::getParts()
{
    return parts;
}

std::vector<Mesh *> &Pole::getShapes()
{
    return shapes;
}

void Pole::addPart(Part *part)
{
    parts.push_back(part);
}

void Pole::addShape(Mesh *shape)
{
    shapes.push_back(shape);
}
