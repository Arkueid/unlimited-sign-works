#ifndef RDBRIDGEGIRDERDATA_H
#define RDBRIDGEGIRDERDATA_H

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

struct _boxGirderDataPara
{
	double lenght;
	double angle;
};

class rdBridgeGirderData : public rdBridgeObject
{
	Q_OBJECT
public:
	rdBridgeGirderData();
	virtual ~rdBridgeGirderData(void);
	 
	virtual void render(QOpenGLShaderProgram* program) = 0;
	//void renderBox(QOpenGLShaderProgram* program, geoCube* pCube)=0;
	virtual bool ptIn(QVector3D pt) = 0;
	void setPos(QVector3D pos);
	void setAngle(float angle);
	void setFlag(int flag);
	int getFlag();

	QVector3D getPos();
	float getAngle();
	virtual void calMinMaxPt() = 0;
	virtual void calCoord() = 0;
	virtual void initGeometry() = 0;

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	virtual void copy(rdBridgeGirderData* pGirder) = 0;

	QVector3D* getEdgePoints();
	void setFrontBackCenter(QVector3D frontCenter, QVector3D backCenter);
	void setAngleX(double angle);
	double getAngleX();
protected:
	QVector3D m_center;
	float m_angle;
	double m_xangle;
	int m_flag;//0中间1左幅左侧2左幅右侧3右幅左侧4右幅右侧
	QVector3D m_edgePts[4];//表面4个点
	QVector3D m_frontCenter;//前中心点（小桩号）
	QVector3D m_backCenter;//后中心点（大桩号）
};
#endif // RDBRIDGEGIRDERDATA_H
