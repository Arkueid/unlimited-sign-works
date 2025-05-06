#ifndef RDGRID1_H
#define RDGRID1_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class rdGrid1 : protected QOpenGLFunctions
{
public:
	rdGrid1(int width = 100, int depth = 100);
	virtual ~rdGrid1(void);
	 
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
#endif // RDGRID1_H
