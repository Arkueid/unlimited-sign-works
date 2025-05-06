#include "rdGrid1.h"

rdGrid1::rdGrid1(int width, int depth)
{
	//initializeOpenGLFunctions();
	this->width = width;
	this->depth = depth;
}

rdGrid1::~rdGrid1(void)
{
	
} 

void rdGrid1::initGeometry()
{
	initializeOpenGLFunctions();
	bool re = vertsBuf.create();

	int num = 10;
	float dx = (float)width / num;
	float dy = (float)depth / num;
	float halfw = width / 2.0f;
	float halfh = depth / 2.0f;
	QVector3D* pVerts = new QVector3D[2 * 2 * (num + 1) + num * num * 4];
	lineCount = 2 * 2 * (num + 1);
	vertsCount = 2 * 2 * (num + 1) + num * num * 4;
	//垂线
	float x0 = -halfw;
	float y0 = -halfh;
	float y1 = halfh;
	int index = 0;
	for (int i = 0; i <= num; i++)
	{
		pVerts[index++] = QVector3D(x0, y0, 0);
		pVerts[index++] = QVector3D(x0, y1, 0);

		x0 += dx;
	}

	//水平线
	x0 = -halfw;
	float x1 = halfw;
	y0 = -halfh;
	for (int i = 0; i <= num; i++)
	{
		pVerts[index++] = QVector3D(x0, y0, 0);
		pVerts[index++] = QVector3D(x1, y0, 0);

		y0 += dy;
	}
	y0 = -halfh;
	y1 = -halfh + dy;
	for (int j = 0; j < num; j++)
	{		
		x0 = -halfw;
		x1 = -halfw + dx;
		for (int i = 0; i < num; i++)
		{
			pVerts[index++] = QVector3D(x0, y0, 0);
			pVerts[index++] = QVector3D(x0, y1, 0);
			pVerts[index++] = QVector3D(x1, y1, 0);
			pVerts[index++] = QVector3D(x1, y0, 0);

			x0 = x1;
			x1 += dx;
		}

		y0 = y1;
		y1 += dy;
	}
	vertsBuf.bind();
	vertsBuf.allocate(pVerts, vertsCount * sizeof(QVector3D));

	delete[] pVerts;
}

void rdGrid1::render(QOpenGLShaderProgram* program)
{
	int vertexLocation = program->attributeLocation("vVertex");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	vertsBuf.bind();
	int count = vertsCount - lineCount;
	program->setUniformValue("Color", QVector3D(0.8, 0.8, 0.8));
	glDrawArrays(GL_QUADS, lineCount, count);

	glDisable(GL_DEPTH_TEST);

	//网格线
	program->setUniformValue("Color", QVector3D(0.3, 0.3, 0.3));	
	glDrawArrays(GL_LINES, 0, lineCount);	
	glEnable(GL_DEPTH_TEST);
}