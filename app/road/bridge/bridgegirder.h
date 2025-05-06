#pragma once
#include <QString>
#include <algorithm>
#include <QVector>

//空心板
struct _hollowSlabMiddlePara
{
	double w[8];
	double h[8];
};

struct _hollowSlabSidePara
{
	//	QString name;
	//	QString lpilename;
	//	QString rpilename;
	//	float lr;
	//    float lheight;
	//	float rr;
	//	float rheight;
};

struct _hollowSlabPara
{
	QString name;
	double length;
	double angle;

	_hollowSlabMiddlePara middle;
	_hollowSlabSidePara side;
};

//箱梁
struct _boxGirderMidPara
{
	double w[16];
	double h[14];
	double r;
	double ml, mr;
};

struct _boxGirderSidePara
{
	double w[13];
	double h[12];
	double r;
	double ml, mr;
};

struct _boxGirderPara
{
	QString name;
	double length;
	double angle;

	_boxGirderMidPara middle;
	_boxGirderSidePara side;
};
