#ifndef TERRAIN_H
#define TERRAIN_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QOpenGLTexture>

#include "rdTerrainSeg.h"
#include "rdTerrainTextureManager.h"

struct _TextBlock
{
	QVector2D p0;
	QVector2D p1;

	QString fname;
	QOpenGLTexture* texture;
	_TextBlock()
	{
		fname = "";
		texture = 0;
	}
	~_TextBlock()
	{
		if (texture) delete texture;
	}
};

class rdTerrain : protected QOpenGLFunctions
{
public:
	rdTerrain();
	~rdTerrain();

	rdTerrainSeg m_segs[500];
	int m_count;

	bool readSegPile();
	//读地形纹理信息
	bool readOrthophotoInfor();
	//读地形纹理数据
	bool readOrthophotoData();

	//读地形几何数据
	bool readTriData();

	void loadTerrainTexture();

	_TextBlock* m_pTextBlock;
	int m_textcount;

	rdTerrainTextureManager m_terrainTextureManager;

	void render(QOpenGLShaderProgram* program);
	void createVBO();
};
#endif // TERRAIN_H
