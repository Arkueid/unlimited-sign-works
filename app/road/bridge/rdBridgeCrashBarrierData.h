#ifndef RDBRIDGECRASHBARRIERDATA_H
#define RDBRIDGECRASHBARRIERDATA_H

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

struct _solidCrashBarrierDataPara
{
	double length;
	double offset;
};

class rdBridgeCrashBarrierData : public rdBridgeObject
{
	//Q_OBJECT
public:
	rdBridgeCrashBarrierData();
	virtual ~rdBridgeCrashBarrierData(void);
	 
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

	virtual void copy(rdBridgeCrashBarrierData* pCrashBarrier) = 0;
	void setLength(double length);
	double getLength();
	void setAngleX(double angle);
	double getAngleX();
protected:
	QVector3D m_center;
	float m_angle;
	double m_anglex;
	int m_flag;//0中间1左幅左侧2左幅右侧3右幅左侧4右幅右侧
	double m_length;
};
#endif // RDBRIDGECRASHBARRIERDATA_H
