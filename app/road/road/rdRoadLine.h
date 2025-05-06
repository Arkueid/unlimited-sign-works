#ifndef ROADLINE_H
#define ROADLINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

#define StartY 4143074.167	
#define StartX 612422.139
#define StartZ 758.205 
#define EPS 0.000001

#ifndef PI
#define  PI 3.14159265358979323846
#endif

struct _RoadCrossSection
{
	QString _pileKNo; //中心桩号k+
	float    pileNo; //中心桩号
	QVector3D center;//中心点坐标
	QVector3D left;
	QVector3D right;

	bool sign = false;
	float lW1;
	float lW2;
	float lW3;
	float W0;
	float rW3;
	float rW2;
	float rW1;

	//直线方程3个系数
	double a;
	double b;
	double c;

	//垂直的直线方程
	double a1;
	double b1;
	double c1;

	//与x轴正向夹角
	double alpha;

	//部件 1路基 2桥 3隧道
	unsigned int lefttype;
	unsigned int righttype;
};

struct _HtPos
{
	double start;
	double end;

	double minx;
	double maxx;

	double miny;
	double maxy;
};

struct _LineComp
{
	float start;
	float end;

	unsigned int leftcomp;
	unsigned int rightcomp;
	QString leftname;
	QString rightname;
};

struct _PosAng
{
	double x;
	double y;
	double z;

	double angle;
	int section;//合同段
};

class rdRoadLine : protected QOpenGLFunctions
{
public:
	rdRoadLine();
	~rdRoadLine();

	void releaseGL();

	bool readData();//htNo 1开始
	bool computing();
	_RoadCrossSection getRCS(int index, int htNo);
	QVector2D getPeg(float x, float y);//x是桩号，y是合同段
	QVector3D getNearestPeg(float x, float y);
	QVector4D getMainLinePeg(float x, float y);//返回：桩号，合同段序号（开始1）、到路线的垂直距离及左右侧（1左，2右）
	QVector4D getLeftLinePeg(float x, float y);
	int       getHT(float peg);

	struct _PosAng findPos(float peg);

	void initGeometry();
	void render(QOpenGLShaderProgram* program);// , QMatrix4x4& proj, QMatrix4x4& view);
private:
	QVector<_RoadCrossSection> RoadLine[14];
	QVector<_RoadCrossSection> RoadLeftLine[14];

	QVector<_LineComp> RoadMainLineComp[14];
	QVector<_LineComp> RoadLeftLineComp[14];

	_HtPos m_htpos[14];
	int m_num;
	
	QOpenGLBuffer vertsBuf;
	long m_allpts;

	QOpenGLBuffer vertsLineBuf;
	int m_linepts;
};

#endif // ROADLINE_H