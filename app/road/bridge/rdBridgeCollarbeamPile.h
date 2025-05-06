#ifndef RDBRIDGECOLLARBEAMPILE_H
#define RDBRIDGECOLLARBEAMPILE_H

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
#include "rdBridgePileUp.h"

class rdBridgeCollarbeamPile : public rdBridgePileUp// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeCollarbeamPile();
	virtual ~rdBridgeCollarbeamPile(void);
	 
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

	void copy(rdBridgeCollarbeamPile* pCylinderPile);
private:
	float m_width;
	float m_height;
	float m_depth;
};
#endif // RDBRIDGECOLLARBEAMPILEBASE_H
