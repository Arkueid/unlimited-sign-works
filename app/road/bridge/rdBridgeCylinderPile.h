#ifndef RDBRIDGECYLINDERPILE_H
#define RDBRIDGECYLINDERPILE_H

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
#include "rdBridgePile.h"

class rdBridgeCylinderPile : public rdBridgePile// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeCylinderPile();
	virtual ~rdBridgeCylinderPile(void);
	 
	void render(QOpenGLShaderProgram* program, geoCylinder*pCylinder);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);
	void setRadius(float r);
	float getRadius();
	void setHeight(float height);	
	float getHeight();
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgePile* pCylinderPile);
private:
	float m_r;
	float m_height;
};
#endif // RDBRIDGECYLINDERPILE_H
