#ifndef RDBRIDGECONICALSLOPE_H
#define RDBRIDGECONICALSLOPE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>
#include "rdBridgeObject.h"

struct _conicalSlopePara
{
	QString name;
	QString abutmentname;
	float h0;
	float h;
	float scale;
	float platwidth;

	float middlewidth;

	//围墙
	float wallheight;
	float leftwidth;
	float rightwidth;
	float downwidth;
	float downheight;
	float heightscale1;//1:0.2
	float heightscale2;//1:0.25
	float downscale;//0.2:1
	//围墙基础
	float basewidth;
	float baseheight;
	float baseleft;
	float baseright;

	bool bHead;//桥头true(小桩号)
};

class rdBridgeConicalSlope : public rdBridgeObject// protected QOpenGLFunctions
{
	//Q_OBJECT
public:
	rdBridgeConicalSlope();
	virtual ~rdBridgeConicalSlope(void);
	 
	void render(QOpenGLShaderProgram* program) ;
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);
	void setPos(QVector3D pos);
	QVector3D getPos();
	void calMinMaxPt();
	void calCoord();
	void initGeometry();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeConicalSlope* ConicalSlope);
	void setNames(QVector <QString>& pileNames);

	void setPara(_conicalSlopePara csp);
	_conicalSlopePara getPara();
protected:
	_conicalSlopePara m_conicalSlopeData;

	QOpenGLBuffer m_coorSlopeBuf;
	QOpenGLBuffer m_textSlopeBuf;
	int m_slopPoints;

	QOpenGLBuffer m_coorPlatformBuf;
	QOpenGLBuffer m_textPlatformBuf;
	int m_platformPoints;

	QOpenGLBuffer m_coorWallBuf;
	QOpenGLBuffer m_textWallBuf;
	int m_wallPoints;

	QOpenGLBuffer m_coorBaseBuf;
	QOpenGLBuffer m_textBaseBuf;
	int m_basePoints;

	void initTopSlopeGeometry( float h1,QVector<QVector3D>& topCircle,QVector<QVector3D>& slopePts,
		                      QVector<QVector2D>& slopeTextures);
	void initPlatformGeometry(QVector<QVector3D>& upCircle, QVector<QVector3D>& downCircle,
		                      QVector<QVector3D>& platformPts, QVector<QVector2D>& platformTextures);
	void initBottomSlopeGeometry(QVector<QVector3D>& midCircle, QVector<QVector3D>& downCircle,
		                         QVector<QVector3D>& slopePts,QVector<QVector2D>& slopeTextures);
	void initWallGeometry(QVector<QVector3D>* bottomCircle, QVector<QVector3D>& wallpts,
		QVector<QVector2D>& walltexts,QVector<QVector3D>& basepts,QVector<QVector2D> &basetexts);
};
#endif // RDBRIDGECONICALSLOPE_H
