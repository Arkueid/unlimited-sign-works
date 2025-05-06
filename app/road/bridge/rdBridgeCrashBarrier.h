#ifndef RDBRIDGECRASHBARRIER_H
#define RDBRIDGECRASHBARRIER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>
#include "rdBridgeObject.h"

class rdBridgeCrashBarrier : public rdBridgeObject// protected QOpenGLFunctions
{
	//Q_OBJECT
public:
	rdBridgeCrashBarrier();
	virtual ~rdBridgeCrashBarrier(void);
	 
	virtual void render(QOpenGLShaderProgram* program,int type) = 0;
	//void renderBox(QOpenGLShaderProgram* program, geoCube* pCube)=0;
	virtual bool ptIn(QVector3D pt) = 0;
	void setPos(QVector3D pos);
	QVector3D getPos();
	virtual void calMinMaxPt() = 0;
	virtual void calCoord() = 0;
	virtual void initGeometry() = 0;

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	virtual void copy(rdBridgeCrashBarrier* pCrashBarrier) = 0;
	
protected:
	QVector3D m_center;
	
};
#endif // RDBRIDGECRASHBARRIER_H
