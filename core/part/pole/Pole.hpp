#pragma once
#include <vector>
#include <string>

#include <part/Part.hpp>

class Pole
{
protected:
    std::vector<Part*> parts;
    std::vector<std::string> textures;
    
public:
    Pole();
    void createFromJsonFile(const char* fileName);
    virtual ~Pole();

    std::vector<Part*>& getParts();

    std::vector<std::string>& getTextures();
};