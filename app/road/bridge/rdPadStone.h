#ifndef RDPADSTONE_H
#define RDPADSTONE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>

struct _rendpadstonepara
{
	QVector3D pos;
	float length;
	float width;
	float height;

	float r;
	float h;

	int count;
	QVector3D cylinderPos[4];//最多4个

	float angle;
};

class rdPadStone : protected QOpenGLFunctions
{
public:
	rdPadStone();
	virtual ~rdPadStone(void);	 
	void render(QOpenGLShaderProgram* program, _rendpadstonepara* pdata,int count);
	bool ptIn(QVector3D pt);
	void initGeometry();
private:
	QOpenGLBuffer coorCubeBuf;
	QOpenGLBuffer coorCylinderBuf;
	QOpenGLBuffer normCubeBuf;
	QOpenGLBuffer normCylinderBuf;
	int m_cubeSize;
	int m_cylinderSize;
};
#endif // RDPADSTONE_H
