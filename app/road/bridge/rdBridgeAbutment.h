#ifndef RDBRIDGEABUTMENT_H
#define RDBRIDGEABUTMENT_H

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

//肋板式轻型桥台
struct _ribbedplatePara
{
	QString name;
	QString lname0, lname1;//桥桩
	QString rname0, rname1;//桥桩

	float upwidth;
	float downwidth;
	float lheight;	//h1
	float rheight;	//h2
	float length;
};

class rdBridgeAbutment : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeAbutment();
	virtual ~rdBridgeAbutment(void);
	 
	virtual void render(QOpenGLShaderProgram* program) = 0;
	//void renderBox(QOpenGLShaderProgram* program, geoCube* pCube)=0;
	virtual bool ptIn(QVector3D pt) = 0;
	void setPos(QVector3D pos);
	QVector3D getPos();
	virtual QVector3D getLeftPos() = 0;
	virtual QVector3D getRightPos() = 0;
	virtual float getLeftHeight() = 0;
	virtual float getRightHeight() = 0;
	virtual bool head() = 0;

	virtual void calMinMaxPt() = 0;
	virtual void calCoord() = 0;
	virtual void initGeometry() = 0;

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	virtual void copy(rdBridgeAbutment* pAbutment) = 0;
	void setPileNames(QVector <QString>& pileNames);
protected:
	QVector3D m_center;

	bool m_bHead;
	QVector3D m_lpos0;
	QVector3D m_lpos1;
	QVector3D m_rpos0;
	QVector3D m_rpos1;

	QVector <QString> m_pileNames;
};
#endif // RDBRIDGEABUTMENT_H
