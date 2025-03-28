#pragma once

#include <part/pole/Pole.hpp>
#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedRegTriPrism.hpp>

class Pole89: public Pole
{
	Cylinder* mainPole;
	RoundedRegTriPrism* panel;
	Cube* bottom;
	Stiffener* bottomStiffener;

public:
	Pole89();
	~Pole89() override;
};