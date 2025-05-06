#ifndef RDBRIDGEPILE_H
#define RDBRIDGEPILE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>
#include "geoSolid/geoCube.h"
#include "rdBridgeObject.h"

struct _bridgepilePara
{
	QString name;
	float r;
	float height;
	QVector3D pos;
	int flag;
};

//桩
class rdBridgePile : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgePile();
	virtual ~rdBridgePile(void);
	 
	//void render(QOpenGLShaderProgram* program, geoCylinder*pCylinder);
	//void renderBox(QOpenGLShaderProgram* program, geoCube* pCube)=0;
	virtual bool ptIn(QVector3D pt) = 0;
	void setPos(QVector3D pos);
	QVector3D getPos();
	virtual void calMinMaxPt() = 0;

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	virtual void copy(rdBridgePile* pPile) = 0;

	void seFlag(int flag);
	int getFlag();
protected:
	QVector3D m_pos;

	int m_flag;//0无意义1左侧第1桩2右侧第1桩3和4为最后桩
};
#endif // RDBRIDGEPILE_H
