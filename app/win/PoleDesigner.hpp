#pragma once

#include <ui_PoleDesigner.h>
#include <QTimer>

#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

class PoleDesigner: public QWidget
{
	Ui::PoleDesigner* ui;

	QTimer timer;

	Cube* cube;
	Cylinder* cylinder;
	Stiffener* stiffener;
	RoundedCube* roundedCube;
	RoundedRegTriPrism* rrTriPrism;

	void bindPart(Part* part);

	void bindCube(Part* part);
	void bindCylinder(Part* part);
	void bindStiffener(Part* part);
	void bindRoundedCube(Part* part);
	void bindRoundedRegTriPrism(Part* part);

public:
	explicit PoleDesigner(QWidget* parent = nullptr);
	~PoleDesigner() override;

};

