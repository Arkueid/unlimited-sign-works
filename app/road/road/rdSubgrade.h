#ifndef ROADSUBGRADE_H
#define ROADSUBGRADE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

#include "rdRoadSlope.h"
#include "rdRoadRamp.h"
#include "rdRoadDitch.h"
#include "rdRoadLine.h"
class rdSubgrade : protected QOpenGLFunctions
{
public:
	rdSubgrade();
	~rdSubgrade();
	void releaseGL();
	bool readData();//htNo 1开始
	void computing(rdRoadLine&roladLine);
	
	rdRoadSlope m_slop;
	rdRoadRamp  m_ramp;
	rdRoadDitch m_ditch;

	void createVBO();
	void loadTextures();
private:	
};

#endif // ROADSUBGRADE_H