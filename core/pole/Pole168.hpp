#pragma once

#include <pole/Pole.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/Cube.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

class Pole168 : public Pole {
public:
    Pole168();
    ~Pole168() override;
};