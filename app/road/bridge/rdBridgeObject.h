#ifndef RDBRIDGEOBJECT_H
#define RDBRIDGEOBJECT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>

#include "../geosolid/geoCube.h"
#include "../scene/rdTextures.h"

class rdBridgeObject : public QObject,protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeObject ();
	virtual ~rdBridgeObject (void);
	 
	QString getName();
	void setName(QString name);
	QVector3D getMinPt();
	QVector3D getMaxPt();

	virtual void calMinMaxPt() = 0;
	virtual void renderBox(QOpenGLShaderProgram* program, geoCube* pCube) = 0;
	virtual bool ptIn(QVector3D pt) = 0;
	bool ptInBox(QVector3D pt);
	void setShow(bool bShow);
	bool getShow();
	virtual bool save(QDataStream& fout) = 0;
	virtual bool read(QDataStream& fout) = 0;

	void setTextures(rdTextures* pTextures);
protected:
	QString m_name;

	QVector3D m_minPt;
	QVector3D m_maxPt;

	bool m_bShow;
	rdTextures* m_pTextures;
};
#endif // RDBRIDGEOBJECT_H
