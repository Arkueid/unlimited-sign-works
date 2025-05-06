#ifndef RDGRID_H
#define RDGRID_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class rdGrid : protected QOpenGLFunctions
{
public:
	rdGrid(int width = 100, int depth = 100);
	virtual ~rdGrid(void);
	 
	//int GetTotalVertices();
	//int GetTotalIndices(); 
	//GLenum GetPrimitiveType();
	void initGeometry();
	/*void FillVertexBuffer( GLfloat* pBuffer);
	void FillIndexBuffer( GLuint* pBuffer);*/
	void render(QOpenGLShaderProgram* program);
private:
	int width, depth;
	QOpenGLBuffer vertsBuf;
	int vertsCount;
	int lineCount;
};
#endif // RDGRID_H
