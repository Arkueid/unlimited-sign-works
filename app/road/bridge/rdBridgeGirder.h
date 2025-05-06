#ifndef RDBRIDGEGIRDER_H
#define RDBRIDGEGIRDER_H

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

class rdBridgeGirder : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeGirder();
	virtual ~rdBridgeGirder(void);
	 
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

	virtual void copy(rdBridgeGirder* pGirder) = 0;
	virtual void getEdgePoints(int type,QVector3D*pts) = 0;
protected:
	QVector3D m_center;
};
#endif // RDBRIDGEGIRDER_H
