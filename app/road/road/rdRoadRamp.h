#ifndef ROADRAMP_H
#define ROADRAMP_H

#include "rdRoadSlope.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

struct _rampdata
{
	QString _KNo;
	float   KNo;
	int leftSign;// 0为不绘制 1为填 2为挖
	int rightSign;
	int leftnum;
	int rightnum;

	float  lwidth;
	float  rwidth;
};

struct _RoadRampData
{
	int no;
	QString   start_KNo;
	float    startno;
	QString   end_KNo;
	float    endno;
	float  lwidth;
	float  rwidth;
};

class rdRoadRamp : protected QOpenGLFunctions
{
public:
	rdRoadRamp();
	~rdRoadRamp();
	bool readData();//htNo 1开始
	void dataProcessing(rdRoadSlope &roadSlope);
	void leftDataProcessing();
	void computing(rdRoadSlope& roadSlope);
	QVector<_RoadRampData> m_RoadRamp;
	QVector<_rampdata> m_rampData[14];
	//bool Computing();
	//_RoadCrossSection GetRCS(int index, int htNo);
	//QVector2D GetPeg(float x, float y);//x是桩号，y是合同段
	//QVector3D GetNearestPeg(float x, float y);
	//QVector4D GetMainLinePeg(float x, float y);//返回：桩号，合同段序号（开始1）、到路线的垂直距离及左右侧（1左，2右）
	//QVector4D GetLeftLinePeg(float x, float y);
	//int       GetHT(float peg);

	//struct _PosAng FindPos(float peg);

	//void initGeometry();
	//void Render(QOpenGLShaderProgram* program, QMatrix4x4& proj, QMatrix4x4& view);
private:
	/*QVector<_RoadCrossSection> RoadLine[14];
	QVector<_RoadCrossSection> RoadLeftLine[14];

	QVector<_LineComp> RoadMainLineComp[14];
	QVector<_LineComp> RoadLeftLineComp[14];

	_HtPos m_htpos[14];
	int m_num;*/
	
	QOpenGLBuffer vertsBuf;
	long m_allpts;
};

#endif // ROADRAMP_H