//肋板式轻型桥台
#ifndef RDRIBBEDPLATEABUTMENT_H
#define RDRIBBEDPLATEABUTMENT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>
#include "geoSolid/geoCylinder.h"
#include "rdBridgeObject.h"
#include "rdBridgeAbutment.h"
#include "../common/common.h"
//肋板式桥台
class rdRibbedPlateAbutment : public rdBridgeAbutment// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdRibbedPlateAbutment();
	virtual ~rdRibbedPlateAbutment(void);
	 
	void setBridgeHead(bool bHead);
	void calCoord();

	bool head();

	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftPos(QVector3D pos0, QVector3D pos1);
	void setRightPos(QVector3D pos, QVector3D pos1);

	QVector3D getLeftPos();
	QVector3D getRightPos();

	void setWidth(float up,float down);
	void setLength(float length);

	void setLeftHeight(float height);
	float getLeftHeight();
	void setRightHeight(float height);
	float getRightHeight();

	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeAbutment* pAbutment);

	void initGeometry();
private:
	float m_upwidth;
	float m_downwidth;
	float m_length;
	float m_lheight;	//h1
	float m_rheight;	//h2
	
	QVector3D m_lcorrs[8];
	QVector3D m_rcorrs[8];

	QOpenGLBuffer coorBuf;
	QOpenGLBuffer normBuf;
	QOpenGLBuffer boxBuf;

	//计算与过p与p0p1垂直的线上的两点，且两点到p的距离相等且和为length
	void calVerticalLine2Pts(QVector2D p0, QVector2D p1, QVector2D p, float length, QVector2D& lp, QVector2D& rp);
};
#endif // RDRIBBEDPLATEABUTMENT_H
