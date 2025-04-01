#include <pole/Pole.hpp>
Pole::Pole(): parts(0)
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

const std::string &Pole::getName() const
{
    return name;
}

void Pole::setName(const std::string &name)
{
    this->name = name;
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
