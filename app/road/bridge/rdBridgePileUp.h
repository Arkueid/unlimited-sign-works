#ifndef RDBRIDGEPILEUP_H
#define RDBRIDGEPILEUP_H

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

struct _solidplatformPara
{
	QString name;
	int rows;
	QVector<int> cols;
	QVector<QString> pilesname;
	float length;
	float width;
	float height;
};
struct _frameplatformPara
{
	QString name;
	int rows;
	QVector<int> cols;
	QVector<QString> pilesname;
	float length;
	float width;
	float height;
	float sublength[3];
	float subwidth[3];
};

class rdBridgePileUp : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgePileUp();
	virtual ~rdBridgePileUp(void);
	 
	virtual void render(QOpenGLShaderProgram* program) = 0;
	//void renderBox(QOpenGLShaderProgram* program, geoCube* pCube)=0;
	virtual bool ptIn(QVector3D pt) = 0;
	void setPos(QVector3D pos);
	QVector3D getPos();
	virtual void calMinMaxPt() = 0;
	virtual void calCoord() = 0;
	virtual void initGeometry() = 0;

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	virtual void copy(rdBridgePileUp* pPile) = 0;
	void setPileNames(QVector <QString>& pileNames);
protected:
	QVector3D m_center;
	QVector <QString> m_pileNames;
};
#endif // RDBRIDGEPILEUP_H
