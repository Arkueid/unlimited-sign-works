#pragma once
#include <QString>
#include <algorithm>
#include <QVector>

#define StartY 4143074.167	
#define StartX 612422.139
#define StartZ 758.205 
#define EPS 0.000001
#define CIRCLEPTS 20

enum sectionType
{
	//桥基础 桥桩 桥承台bearing platform 下部构造 桥墩 桥台bridge abutment 盖梁
	None = 0,BRIDGE,BASE,PILE,BPLATFORM,DOWN, PIER, BABUTMENT, COPING
};

struct _sectionTypeValue
{
	sectionType type;
	QString svalue;
	_sectionTypeValue()
	{
		type = None;
		svalue = "";
	}
};

#define Offset 0.1