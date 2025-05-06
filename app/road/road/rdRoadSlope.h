#ifndef ROADSLOPE_H
#define ROADSLOPE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLTexture>

class rdRoadLine;
class rdRoadRamp;
class rdRoadDitch;

struct _RoadSlopeData
{
	QString   _pileKNo; //中心桩号k+
	float    pileNo; //中心桩号

	int leftSign;// 0为不绘制 1为填 2为挖
	int rightSign;

	float leftHeight;//左右侧高度
	float rightHeight;

	float leftDistance;//中心坐标到坡脚距离
	float rightDistance;

	int leftNum;//左侧斜坡个数
	int rightNum;

	int leftRnum;//左侧碎落台个数
	int rightRnum;

	float left[8];//每个斜坡的比值
	float right[8];

	float leftr[7];//每个碎落台的宽值
	float rightr[7];
};

struct _mp
{
	QString   _pileKNo; //中心桩号k+

	//cgj
	float  pile;
	//cgj

	int leftsign;
	int rightsign;

	int leftNum;//左侧斜坡个数
	int rightNum;

	float hl;//左侧的总高度
	float hr;

	float ld;//左侧的总距离
	float rd;

	QVector3D cpoints;
	QVector3D lpoints[20];
	QVector3D rpoints[20];

	//排水沟（挖方为边沟）
	bool bLDitch;
	bool bRDitch;
	QVector3D lditch[6];
	QVector3D rditch[6];

	//护坡道（挖方为碎落台）
	bool bLRamp;
	bool bRRamp;
	QVector3D lramp[2];
	QVector3D rramp[2];

	//路面
	QVector3D lsurface[2];
	QVector3D rsurface[2];

	//平台
	int iLplatform;
	int iRplatform;
	QVector3D lplatform[40];
	QVector3D rplatform[40];
	//平台排水沟
	//iLplatform-1;iRplatform-1
	QVector3D lplatformditch[20];
	QVector3D rplatformditch[20];

	float theta;

	int lsign;//是否绘制护坡排水沟标记
	int rsign;

	//部件 1路基 2桥 3隧道
	unsigned int lefttype;
	unsigned int righttype;

	//int status;//0无； 1：清表土；2：软基挖；3：软基碾压；4：填挖；5：完工
	//int lstatus, rstatus;
	//vec3 workingcpoints;//路基顶面中心点

	//bool bLWorking;
	//bool bRWorking;

	//int iLWorkingLevel;
	//int iRWorkingLevel;

	//int workinglSlopePtsCount;
	//vec3 workinglSlopePoints[20];
	//int workingrSlopePtsCount;
	//vec3 workingrSlopePoints[20];

	//int workinglPlatformPtsCount;
	//vec3 workinglPlatformPoints[20];
	//int workingrPlatformPtsCount;
	//vec3 workingrPlatformPoints[20];
};

struct _lrdis
{
	float  ldis;
	float  rdis;
};

struct _QUAD
{
	QVector3D cord[4];
	QVector3D normal;
	QVector2D text[4];
};

struct _RoadSlopeQuad//1个桩号对应
{
	float pileNo; //中心桩号
	float pileEndNo;
	int leftSign;// 0为不绘制 1为填 2为挖
	int rightSign;

	bool m_bLFillToExcave;//是否填到挖
	bool m_bRFillToExcave;//是否填到挖

	int leftNum;//左侧斜坡个数
	int rightNum;

	_QUAD m_left[10];//左斜坡
	_QUAD m_right[10];//右斜坡

	_QUAD m_leftsurface;//左路面
	_QUAD m_rightsurface;//右路面

	bool m_bleftRamp, m_brightRamp;
	_QUAD m_leftRamp;//左护坡道（碎落台）
	_QUAD m_rightRamp;//右护坡道（碎落台）

	bool m_bleftDitch, m_brightDitch;
	_QUAD m_leftDitch[5];//左排水沟（边沟）
	_QUAD m_rightDitch[5];//右排水沟（边沟）

	int m_iLeftPlatform, m_iRightPlatform;
	_QUAD m_leftPlatform[20];//左平台
	_QUAD m_rightPlatform[20];//右平台

	int  m_iLeftPlatformDitch, m_iRightPlatformDitch;
	_QUAD m_leftPlatformDitch[30];//左平台排水沟
	_QUAD m_rightPlatformDitch[30];//右平台排水沟

	_QUAD m_quad;//最外围四边形
	bool m_bQuad;

	_QUAD m_lowerquad;//最外围四边形
	bool m_bLowerQuad;

	//部件 1路基 2桥 3隧道
	unsigned int lefttype;
	unsigned int righttype;

	//附加
	//边沟和碎落台结束
	bool m_bleftBSEnd;
	_QUAD m_leftBSEnd[5];
	bool m_brightBSEnd;
	_QUAD m_rightBSEnd[5];

	//平台（含平台排水沟）
	int m_iLeftPlatformEnd, m_iRightPlatformEnd;
	_QUAD m_leftPlatformEnd[50];//左平台
	_QUAD m_rightPlatformEnd[50];//右平台

	int status;//0无； 1：清表土；2：软基挖；3：软基碾压；4：填挖（施工状态）；5：完工
	int next_status;

	_QUAD m_pure;//清表土多边形
	bool m_bPure;
	_QUAD m_soft;//软基多边形
	bool m_bSoft;

	_QUAD m_roll;//碾压
	bool m_bRoll;

	QString date;//特殊处理
	QString date1;//路基

	//修补四边形
	QVector<_QUAD> m_MendingQuads;
	QVector<_QUAD> m_HeadTailQuads;//头尾修补
	QVector<_QUAD> m_MendingLeftQuads;
	QVector<_QUAD> m_HeadTailLeftQuads;//头尾修补
	//施工阶段的路基
	//顶面
	QVector<_QUAD>  m_SurfaceWorkingQuads;
	//填方护坡
	QVector<_QUAD> m_SlopeFillQuads;
	//填方平台
	QVector<_QUAD> m_PlatformFillQuads;
	//挖方护坡
	QVector<_QUAD> m_SlopeExcaveQuads;
	//挖方平台
	QVector<_QUAD> m_PlatformExcaveQuads;

	_QUAD m_terrainQuad;//路基施工时用，地形与路基的接触面

	//dvQuad m_clipQuad;
};

class rdRoadSlope : protected QOpenGLFunctions
{
public:
	rdRoadSlope();
	~rdRoadSlope();
	bool readData();//htNo 1开始
	void computing(rdRoadLine& rline, rdRoadRamp& rramp, rdRoadDitch& rditch);
	QVector<_RoadSlopeData> m_RoadSlope[14];
	QVector<_lrdis> dis2[14];
	QVector<_mp> m_fiPoints2[14];//完成数据

	void createVBO(bool bleft = false);
	void renderSurface(QOpenGLShaderProgram* program);
	void renderSlope(QOpenGLShaderProgram* program);

	void loadTexture();
private:
	float ptDis(QVector3D p0, QVector3D p1);

	QOpenGLBuffer m_slopBuf;
	QOpenGLBuffer m_ditchBuf;
	long m_allpts;

	void makeQuad();//斜坡和路面
	void makeDitchQuad(bool bleft = false);
	void makeRampQuad(bool bleft = false);
	void makePlatformQuad(bool bleft = false);
	
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
	QVector<_RoadSlopeQuad> m_quad[14];
	//路面
	//unsigned int vaoHandleSurface[14];
	QOpenGLBuffer surfaceVertsBuf[14];
	//unsigned int vboHandle[14];
	int m_quadCountSurface[14];

	int m_quadFillCount[14][8]; //填方
	QOpenGLBuffer slopFillVertsBuf[14][8][3];//8级
	int m_quadExcaveCount[14][8];//挖方
	QOpenGLBuffer slopExcaveVertsBuf[14][8][3];
	
	QOpenGLTexture* m_texFill1;
	QOpenGLTexture* m_texFill2;
};

#endif // ROADSLOPE_H