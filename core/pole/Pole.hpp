#pragma once
#include <vector>
#include <string>

#include <part/Part.hpp>

// 单柱，双柱，门架，单悬臂，双悬臂
enum PoleType
{
    PoleType_Single = 0, 
    PoleType_Double = 1,
    PoleType_Gate = 2,
    PoleType_SingleArm = 3,
    PoleType_DoubleArm = 4,  
};

class Pole
{
private:
    int id;
protected:
    int type;
    std::string name;
    std::vector<Part*> parts;
    
public:
    Pole();
    virtual ~Pole();

    int getType() const;

    void setType(int type);

    std::vector<Part*>& getParts();

    const std::string& getName() const;

    void setName(const std::string& name);

    void addPart(Part* part);

    void removePart(Part* part);

    int getId();

    void setId(int id);
};