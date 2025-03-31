#pragma once
#include <vector>
#include <string>

#include <part/Part.hpp>

class Pole
{
protected:
    std::vector<Part*> parts;
    std::vector<Mesh*> shapes;
    
public:
    Pole();
    void createFromJsonFile(const char* fileName);
    virtual ~Pole();

    std::vector<Part*>& getParts();

    std::vector<Mesh*>& getShapes();

    void addPart(Part* part);

    void addShape(Mesh* shape);
};