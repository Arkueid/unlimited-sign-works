#pragma once

#include <part/pole/Pole.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/Cube.hpp>

class Pole168 : public Pole {
    Cylinder sideBeam;
    Cylinder mainPole;
    Cylinder flange;
    Cube panel;
    Cube bottom;
    Stiffener sideStiffener;
    Stiffener bottomStiffener;
public:
    Pole168();
    ~Pole168() override = default;
};