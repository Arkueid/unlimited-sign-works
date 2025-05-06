#ifndef RoadMediumStrip_H
#define RoadMediumStrip_H

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

struct _strip
{
	QVector<QVector3D> in;
	QVector<QVector3D> out;
};

class rdRoadMediumStrip : protected QOpenGLFunctions
{
public:
	rdRoadMediumStrip();
	~rdRoadMediumStrip();
	//bool ReadData();//htNo 1开始
	//void Computing(rdRoadLine& rline, rdRoadRamp& rramp, rdRoadDitch& rditch);
	//QVector<_RoadSlopeData> m_RoadSlope[14];
	//QVector<_lrdis> dis2[14];
	//QVector<_mp> m_fiPoints2[14];//完成数据

	//void CreateVBO(bool bleft = false);
	//void RenderSurface(QOpenGLShaderProgram* program, QMatrix4x4& proj, QMatrix4x4& view);
	//void RenderSlope(QOpenGLShaderProgram* program, QMatrix4x4& proj, QMatrix4x4& view);

	//void LoadTexture();
private:
//	float PtDis(QVector3D p0, QVector3D p1);
//
//	QOpenGLBuffer m_slopBuf;
//	QOpenGLBuffer m_ditchBuf;
//	long m_allpts;
//
//	void MakeQuad();//斜坡和路面
//	void MakeDitchQuad(bool bleft = false);
//	void MakeRampQuad(bool bleft = false);
//	void MakePlatformQuad(bool bleft = false);
//	
//	//bool Computing();
//	//_RoadCrossSection GetRCS(int index, int htNo);
//	//QVector2D GetPeg(float x, float y);//x是桩号，y是合同段
//	//QVector3D GetNearestPeg(float x, float y);
//	//QVector4D GetMainLinePeg(float x, float y);//返回：桩号，合同段序号（开始1）、到路线的垂直距离及左右侧（1左，2右）
//	//QVector4D GetLeftLinePeg(float x, float y);
//	//int       GetHT(float peg);
//
//	//struct _PosAng FindPos(float peg);
//
//	//void initGeometry();
//	//void Render(QOpenGLShaderProgram* program, QMatrix4x4& proj, QMatrix4x4& view);
//private:
//	QVector<_RoadSlopeQuad> m_quad[14];
//	//路面
//	//unsigned int vaoHandleSurface[14];
//	QOpenGLBuffer surfaceVertsBuf[14];
//	//unsigned int vboHandle[14];
//	int m_quadCountSurface[14];
//
//	int m_quadFillCount[14][8]; //填方
//	QOpenGLBuffer slopFillVertsBuf[14][8][3];//8级
//	int m_quadExcaveCount[14][8];//挖方
//	QOpenGLBuffer slopExcaveVertsBuf[14][8][3];
//	
//	QOpenGLTexture* m_texFill1;
//	QOpenGLTexture* m_texFill2;
};

#endif // RoadMediumStrip_H