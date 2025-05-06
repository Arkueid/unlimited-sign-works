#include "rdPadStone.h"
#include "../common/common.h"

rdPadStone::rdPadStone()
{
}


rdPadStone::~rdPadStone(void)
{
	
} 
//
//void rdPadStone::calCoord()
//{
//	QVector3D pts[CIRCLEPTS];
//	QVector3D normals[CIRCLEPTS];
//
//	float dalpha = 2 * 3.14159 / CIRCLEPTS;
//	float alpha = 0;
//	for (int i = 0; i < CIRCLEPTS; i++)
//	{
//		pts[i] = QVector3D(m_lr * cos(alpha), m_lr * sin(alpha), 0);
//		normals[i] = QVector3D(cos(alpha), sin(alpha), 0);
//		alpha += dalpha;
//	}
//	
//	QMatrix4x4 ltransMatrix;
//	ltransMatrix.translate(m_lpos);
//
//	for (int i = 0; i < CIRCLEPTS; i++) m_lcorrs[i] = ltransMatrix * pts[i];
//	m_norms[0] = QVector3D(0, 0, 1);
//	m_norms[1] = QVector3D(0, 0, -1);
//	for (int i = 0; i < CIRCLEPTS; i++) m_norms[i+2] = normals[i];
//
//	alpha = 0;
//	for (int i = 0; i < CIRCLEPTS; i++)
//	{
//		pts[i] = QVector3D(m_rr * cos(alpha), m_rr * sin(alpha), 0);
//		alpha += dalpha;
//	}
//
//	QMatrix4x4 rtransMatrix;
//	rtransMatrix.translate(m_rpos);
//
//	for (int i = 0; i < CIRCLEPTS; i++) m_rcorrs[i] = rtransMatrix * pts[i];
//}

void rdPadStone::initGeometry()
{
	int size = 6*4;// CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2;
	QVector3D* pVertices = new QVector3D[size];
	QVector3D* pNnormals = new QVector3D[size];
	
	int indexV = 0;
	int indexN = 0;

	float w = 0.5;
	QVector3D pts[] = { {-w,-w,-w},{w,-w,-w},{w,w,-w},{-w,w,-w},{-w,-w,w},{w,-w,w},{w,w,w},{-w,w,w} };

	//顶面
	for (int i = 4; i < 8; i++)
	{
		pVertices[indexV++] = pts[i];
		pNnormals[indexN++] = QVector3D(0, 0, 1);
	}
	//底面
	for (int i = 0; i < 4; i++)
	{
		pVertices[indexV++] = pts[3-i];
		pNnormals[indexN++] = QVector3D(0, 0, -1);
	}

	//左侧面
	pVertices[indexV++] = pts[4]; pNnormals[indexN++] = QVector3D(-1, 0,0 );
	pVertices[indexV++] = pts[7]; pNnormals[indexN++] = QVector3D(-1, 0, 0);
	pVertices[indexV++] = pts[3]; pNnormals[indexN++] = QVector3D(-1, 0, 0);
	pVertices[indexV++] = pts[0]; pNnormals[indexN++] = QVector3D(-1, 0, 0);

	//右侧面
	pVertices[indexV++] = pts[5]; pNnormals[indexN++] = QVector3D(1, 0, 0);
	pVertices[indexV++] = pts[1]; pNnormals[indexN++] = QVector3D(1, 0, 0);
	pVertices[indexV++] = pts[2]; pNnormals[indexN++] = QVector3D(1, 0, 0);
	pVertices[indexV++] = pts[6]; pNnormals[indexN++] = QVector3D(1, 0, 0);

	//前面
	pVertices[indexV++] = pts[4]; pNnormals[indexN++] = QVector3D( 0,-1, 0);
	pVertices[indexV++] = pts[0]; pNnormals[indexN++] = QVector3D( 0,-1, 0);
	pVertices[indexV++] = pts[1]; pNnormals[indexN++] = QVector3D( 0,-1, 0);
	pVertices[indexV++] = pts[5]; pNnormals[indexN++] = QVector3D( 0,-1, 0);

	//后面
	pVertices[indexV++] = pts[7]; pNnormals[indexN++] = QVector3D(0, 1, 0);
	pVertices[indexV++] = pts[6]; pNnormals[indexN++] = QVector3D(0, 1, 0);
	pVertices[indexV++] = pts[2]; pNnormals[indexN++] = QVector3D(0, 1, 0);
	pVertices[indexV++] = pts[3]; pNnormals[indexN++] = QVector3D(0, 1, 0);

	initializeOpenGLFunctions();
	bool re = coorCubeBuf.create();
	re = coorCubeBuf.bind();
	coorCubeBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = normCubeBuf.create();
	normCubeBuf.bind();
	normCubeBuf.allocate(pNnormals, size * sizeof(QVector3D));

	delete[] pVertices;
	delete[] pNnormals;

	//圆柱
	QVector3D cpts[CIRCLEPTS];
	QVector3D normals[CIRCLEPTS];
	float dalpha = 2 * 3.14159 / CIRCLEPTS;
	float alpha = 0;
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		cpts[i] = QVector3D(cos(alpha), sin(alpha), 0);
		normals[i] = QVector3D(cos(alpha), sin(alpha), 0);
		alpha += dalpha;
	}

	int size1 = CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2;
	pVertices = new QVector3D[size1];
	pNnormals = new QVector3D[size1];

	indexV = 0;
	indexN = 0;
	QVector3D normal = QVector3D(0, 0, -1);
	//底面
	QVector3D center = QVector3D(0, 0, 0);
	QVector3D downpt = QVector3D(0, 0, -0.5);
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		pVertices[indexV++] = cpts[i] + downpt; pNnormals[indexN++] = normal;
		pVertices[indexV++] = cpts[e] + downpt;; pNnormals[indexN++] = normal;
		pVertices[indexV++] = center + downpt;; pNnormals[indexN++] = normal;
	}
	//顶面
	QVector3D uppt = QVector3D(0, 0, 0.5);
	normal = QVector3D(0, 0, 1);
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		pVertices[indexV++] = cpts[i] + uppt; pNnormals[indexN++] = normal;
		pVertices[indexV++] = cpts[e] + uppt; pNnormals[indexN++] = normal;
		pVertices[indexV++] = center + uppt; pNnormals[indexN++] = normal;
	}

	//侧面
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		pVertices[indexV++] = cpts[i] + downpt; pNnormals[indexN++] = normals[i];
		pVertices[indexV++] = cpts[e] + downpt; pNnormals[indexN++] = normals[e];
		pVertices[indexV++] = cpts[i] + uppt; pNnormals[indexN++] = normals[i];

		pVertices[indexV++] = cpts[e] + downpt; pNnormals[indexN++] = normals[e];
		pVertices[indexV++] = cpts[e] + uppt; pNnormals[indexN++] = normals[e];
		pVertices[indexV++] = cpts[i] + uppt; pNnormals[indexN++] = normals[i];
	}

	//initializeOpenGLFunctions();
	re = coorCylinderBuf.create();
	re = coorCylinderBuf.bind();
	coorCylinderBuf.allocate(pVertices, size1 * sizeof(QVector3D));

	re = normCylinderBuf.create();
	normCylinderBuf.bind();
	normCylinderBuf.allocate(pNnormals, size1 * sizeof(QVector3D));

	delete[] pVertices;
	delete[] pNnormals;

	m_cubeSize = size;
	m_cylinderSize = size1;
}
//
//void rdPadStone::calMinMaxPt()
//{
//	/*QVector3D pos = m_pos;
//	float minx = pos.x() - m_r - Offset;
//	float miny = pos.y() - m_r - Offset;
//	float maxx = pos.x() + m_r + Offset;
//	float maxy = pos.y() + m_r + Offset;
//	float maxz = pos.z() + Offset;
//	float minz = pos.z() - m_height - Offset;
//
//	m_minPt = QVector3D(minx, miny, minz);
//	m_maxPt = QVector3D(maxx, maxy, maxz);*/
//}

bool rdPadStone::ptIn(QVector3D pt)
{
	//if (!ptInBox(pt)) return false;//包围盒测试

	/*QVector3D center = m_pos - QVector3D(0, 0, m_height / 2);
	float zmin = m_pos.z() - m_height;
	float zmax = m_pos.z();

	float offset = 0.1;

	if (pt.z() > (zmax+offset) || pt.z() < (zmin-offset)) return false;
	float r = (m_pos.x() - pt.x()) * (m_pos.x() - pt.x()) + (m_pos.y() - pt.y()) * (m_pos.y() - pt.y());

	if (r > (m_r+offset) * (m_r+offset)) return false;*/

	return true;
}

void rdPadStone::render(QOpenGLShaderProgram* program, _rendpadstonepara* pdata, int count)
{
	//绘制
	QVector3D ka1 = QVector3D(0.3, 0.3, 0.3);//环境光反射系数
	QVector3D kd1 = QVector3D(0.5, 0.5, 0.5);//漫反射反射系数
	QVector3D ka2 = QVector3D(0.1, 0.1, 0.1);//环境光反射系数
	QVector3D kd2 = QVector3D(0.1, 0.1, 0.1);//漫反射反射系数

	for (int i = 0; i < count; i++)
	{
		program->setUniformValue("Ka", ka1);
		program->setUniformValue("Kd", kd1);

		_rendpadstonepara& rpsp = pdata[i];
		QMatrix4x4 scaleMatrix;
		scaleMatrix.scale(rpsp.length,rpsp.width,rpsp.height);
		QMatrix4x4 modelMatrix;
		modelMatrix.translate(rpsp.pos);

		QMatrix4x4 rotateMatrix;
		rotateMatrix.rotate(rpsp.angle, 0, 0, 1);
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;
		
		program->setUniformValue("ModelMatrix", modelMatrix);
	
		int size = CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2;
		coorCubeBuf.bind();
		int vertexLocation = program->attributeLocation("VertexPosition");
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
		normCubeBuf.bind();
		int normalLocation = program->attributeLocation("VertexNormal");
		program->enableAttributeArray(normalLocation);
		program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
		glDrawArrays(GL_QUADS, 0, m_cubeSize);

		program->setUniformValue("Ka", ka2);
		program->setUniformValue("Kd", kd2);
		coorCylinderBuf.bind();
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
		normCylinderBuf.bind();
		program->enableAttributeArray(normalLocation);
		program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

		QMatrix4x4 scaleMatrix1;
		scaleMatrix1.scale(rpsp.r, rpsp.r, rpsp.h);
		for (int j = 0; j < rpsp.count; j++)
		{
			QMatrix4x4 modelMatrix1;
			modelMatrix1.translate(rpsp.cylinderPos[j]);
			modelMatrix1 = modelMatrix1 * scaleMatrix1;
			program->setUniformValue("ModelMatrix", modelMatrix1);

			glDrawArrays(GL_TRIANGLES, 0, m_cylinderSize);
		}
	}
	//恢复矩阵
	QMatrix4x4 modelMatrix;
	modelMatrix.translate(0.0, 0.0, 0.0);
	program->setUniformValue("ModelMatrix", modelMatrix);

	program->setUniformValue("Ka", ka1);
	program->setUniformValue("Kd", kd1);
}
//
//void rdPadStone::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
//{
//	//if (!m_bShow) return;
//
//	//QVector3D size = (m_maxPt - m_minPt);
//	//
//	//QMatrix4x4 scaleMatrix;
//	//scaleMatrix.scale(size);
//	//QMatrix4x4 modelMatrix;
//	//modelMatrix.translate(m_pos.x(),m_pos.y(),m_pos.z() - size.z()/2);
//
//	//modelMatrix = modelMatrix * scaleMatrix;
//	////QMatrix4x4 mpv = pv * modelMatrix;
//	//program->setUniformValue("Model", modelMatrix);
//	//pCube->drawBox(program);
//}
