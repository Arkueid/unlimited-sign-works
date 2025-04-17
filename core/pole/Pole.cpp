#include <pole/Pole.hpp>

Pole::Pole(): parts(0), type(PoleType_Single)
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
int Pole::getType() const
{
    return type;
}

void Pole::setType(int type)
{
    this->type = type;
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
