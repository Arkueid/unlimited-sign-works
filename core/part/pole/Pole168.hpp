#pragma once

#include <part/pole/Pole.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/Cube.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

class Pole168 : public Pole {
    Cylinder sideBeam;
    Cylinder mainPole;
    Cylinder flange;
    RoundedCube panel;
    Cube bottom;
    Stiffener sideStiffener;
    Stiffener bottomStiffener;

    // RoundedRegTriPrism test;
public:
    Pole168();
    ~Pole168() override = default;
};