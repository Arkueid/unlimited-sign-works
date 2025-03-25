#include <part/pole/Pole.hpp>

Pole::Pole()
{
}
void Pole::createFromJsonFile(const QString &fileName)
{
    
}

QVector<Part> Pole::getParts() const
{
    return parts;
}
