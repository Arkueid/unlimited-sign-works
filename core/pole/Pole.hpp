#pragma once
#include <vector>
#include <string>

#include <part/Part.hpp>

class Pole
{
protected:
    std::vector<Part*> parts;
    
public:
    Pole();
    void createFromJsonFile(const char* fileName);
    virtual ~Pole();

    std::vector<Part*>& getParts();

    void addPart(Part* part);

    void removePart(Part* part);
};