#pragma once
#include <QVector>

#include <part/Part.hpp>

class Pole
{
protected:
    QVector<Part> parts;    
    
public:
    Pole();
    void createFromJsonFile(const QString& fileName);
    virtual ~Pole() = default;

    QVector<Part> getParts() const;
};