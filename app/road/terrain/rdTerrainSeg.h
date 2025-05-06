#ifndef TERRAINSEG_H
#define TERRAINSEG_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include <QVector2D>
#include <QString>
#include "rdTerrainTriangle.h"

struct _RenderSeg//纹理分配
{
	unsigned int m_id;//纹理ID
	QString m_textname;
	QVector<rdTerrainTriangle*> m_segs;
	//QVector<quads> m_clipquads;

	QVector2D m_xy1, m_st1, m_ddt;
};

class rdTerrainSeg : protected QOpenGLFunctions
{
public:
	rdTerrainSeg();
	~rdTerrainSeg();

	int m_index;
	float m_start, m_end;//起始终止桩号

	//每段地形使用的纹理数及序号
	int m_iTexImageCount;//子块个数及
	int m_iTexImageNo[4];

	_RenderSeg m_subSeg[4];
	
	QOpenGLBuffer m_subvbo[4];
	QOpenGLBuffer m_subtexcorvbo[4];
	void createVBO();
	//包围盒
	double m_minx;
	double m_maxx;
	double m_miny;
	double m_maxy;
};
#endif // TERRAINSEG_H
