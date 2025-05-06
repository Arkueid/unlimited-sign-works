#ifndef RDFRAMEPILEPLATFORM_H
#define RDFRAMEPILEPLATFORM_H

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

class rdFramePilePlatform : public rdBridgePileUp// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdFramePilePlatform();
	virtual ~rdFramePilePlatform(void);
	 
	void calCoord();
	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);
	void setLength(float l,float*subLength);
	float getLength();
	void setWidth(float w,float *subWidth);
	float getWidth();
	void setHeight(float height);
	float getHeight();
	void setDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgePileUp* pPileUp);

	void initGeometry();
private:
	float m_length;
	float m_sublength[3];
	float m_width;
	float m_subwidth[3];
	float m_height;	

	QVector3D m_dir;
	QVector3D m_corrs[12];
	QVector3D m_norms[14];

	QOpenGLBuffer coorBuf;
	QOpenGLBuffer normBuf;
	QOpenGLBuffer boxBuf;
};
#endif // RDFRAMEPILEPLATFORM_H
