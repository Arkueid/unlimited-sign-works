#pragma once
#include <vector>
#include <string>

#include <part/Part.hpp>

class Pole
{
protected:
    std::string name;
    std::vector<Part*> parts;
    
public:
    Pole();
    virtual ~Pole();

    std::vector<Part*>& getParts();

    const std::string& getName() const;

    void setName(const std::string& name);

    void addPart(Part* part);

    void removePart(Part* part);
};