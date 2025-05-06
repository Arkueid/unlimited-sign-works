#include "rdFramePilePlatform.h"
#include "../common/common.h"

rdFramePilePlatform::rdFramePilePlatform()
{
	m_height = 5.0f;
}


rdFramePilePlatform::~rdFramePilePlatform(void)
{
	
} 

void rdFramePilePlatform::calCoord()
{
	QVector3D pts[12];
	float halflen = m_length / 2.0f;
	float halfwidth = m_width / 2.0f;

	pts[0] = QVector3D(-halflen,  halfwidth, 0);
	pts[1] = QVector3D(-halflen, -halfwidth, 0);
	float sublen1 = -halflen + m_sublength[0];
	pts[2] = QVector3D(sublen1, -halfwidth, 0);
	float subwid1 = -halfwidth + m_subwidth[0];
	pts[3] = QVector3D(sublen1, subwid1, 0);
	float sublen2 = sublen1 + m_sublength[1];
	pts[4] = QVector3D(sublen2, subwid1, 0);
	pts[5] = QVector3D(sublen2, -halfwidth, 0);
	pts[6] = QVector3D(halflen, -halfwidth, 0);

	pts[7] = QVector3D(halflen, halfwidth, 0);
	pts[8] = QVector3D(sublen2, halfwidth, 0);
	pts[9] = QVector3D(sublen2, -subwid1, 0);
	pts[10] = QVector3D(sublen1, -subwid1, 0);
	pts[11] = QVector3D(sublen1, halfwidth, 0);

	//上下面
	m_norms[0] = QVector3D(0, 0, 1);
	m_norms[1] = QVector3D(0, 0, -1);
	//侧面
	m_norms[2] = QVector3D(-1, 0, 0);
	m_norms[3] = QVector3D(0,-1,  0);
	m_norms[4] = QVector3D(1, 0, 0);
	m_norms[5] = QVector3D(0, -1, 0);
	m_norms[6] = QVector3D(-1, 0, 0);
	m_norms[7] = QVector3D(0, -1, 0);
	m_norms[8] = QVector3D(1, 0, 0);
	m_norms[9] = QVector3D(0,1,  0);
	m_norms[10] = QVector3D(-1, 0, 0);
	m_norms[11] = QVector3D(0, 1, 0);
	m_norms[12] = QVector3D(1,0,  0);
	m_norms[13] = QVector3D(0,1,  0);

	QMatrix4x4 transMatrix;
	transMatrix.translate(m_center);

	QMatrix4x4 rotateMatrix;
	rotateMatrix.rotate(0, 0, 0, 1);
	if (m_dir.x() == 0)//垂线
	{

	}
	else
	{
		float s = m_dir.y() / m_dir.x();
		float angle = atan(s);
		angle = angle * 180 / 3.14;
		rotateMatrix.rotate(angle, 0, 0, 1);
	}

	QMatrix4x4 allMatrix = transMatrix * rotateMatrix;
	for (int i = 0; i < 12; i++)
		m_corrs[i] = allMatrix * pts[i];

	QMatrix4x4 inverseMatrix = rotateMatrix.inverted();
	QMatrix4x4 temp = inverseMatrix.transposed();

	for (int i = 2; i < 14; i++) m_norms[i] = temp * m_norms[i];
}

void rdFramePilePlatform::initGeometry()
{
	QVector3D vertices[72];//(12+6)*4
	QVector3D normals[72];
	int indexV = 0;
	int indexN = 0;
	//上面
	vertices[indexV++] = m_corrs[0];normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[1]; normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[2]; normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[11]; normals[indexN++] = m_norms[0];

	vertices[indexV++] = m_corrs[3]; normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[4]; normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[9]; normals[indexN++] = m_norms[0];
	vertices[indexV++] = m_corrs[10]; normals[indexN++] = m_norms[0];

	for (int i = 5; i < 9; i++)
	{
		vertices[indexV++] = m_corrs[i]; 
		normals[indexN++] = m_norms[0];
	}

	//底面
	QVector3D pt(0, 0, m_height);
	vertices[indexV++] = m_corrs[0] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[1] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[2] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[11] - pt; normals[indexN++] = m_norms[1];

	vertices[indexV++] = m_corrs[3] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[4] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[9] - pt; normals[indexN++] = m_norms[1];
	vertices[indexV++] = m_corrs[10] - pt; normals[indexN++] = m_norms[1];

	for (int i = 5; i < 9; i++)
	{
		vertices[indexV++] = m_corrs[i] - pt;
		normals[indexN++] = m_norms[1];
	}
	
	for (int i = 0; i < 12; i++)
	{
		int end = (i + 1) % 12;
		QVector3D pt0 = m_corrs[i];
		QVector3D pt1 = m_corrs[end];

		vertices[indexV++] = pt0;		normals[indexN++] = m_norms[2 + i];
		vertices[indexV++] = pt0 - pt;	normals[indexN++] = m_norms[2 + i];
		vertices[indexV++] = pt1 - pt;	normals[indexN++] = m_norms[2 + i];
		vertices[indexV++] = pt1;		normals[indexN++] = m_norms[2 + i];
	}
	
	initializeOpenGLFunctions();
	bool re = coorBuf.create();
	re = coorBuf.bind();
	coorBuf.allocate(vertices, 72 * sizeof(QVector3D));

	re = normBuf.create();
	normBuf.bind();
	normBuf.allocate(normals, 72 * sizeof(QVector3D));
}

void rdFramePilePlatform::setLength(float l, float* subLength)
{
	m_length = l;
	for (int i = 0; i < 3; i++)m_sublength[i] = subLength[i];
}

float rdFramePilePlatform::getLength()
{
	return m_length;
}

void rdFramePilePlatform::setWidth(float w, float* subWidth)
{
	m_width = w;
	for (int i = 0; i < 3; i++)m_subwidth[i] = subWidth[i];
}

float rdFramePilePlatform::getWidth()
{
	return m_width;
}

void rdFramePilePlatform::setDir(QVector3D dir)
{
	m_dir = dir;
}

void rdFramePilePlatform::setHeight(float height)
{
	m_height = height;
}

float rdFramePilePlatform::getHeight()
{
	return m_height;
}

void rdFramePilePlatform::calMinMaxPt()
{
	/*QVector3D pos = m_pos;
	float minx = pos.x() - m_r - Offset;
	float miny = pos.y() - m_r - Offset;
	float maxx = pos.x() + m_r + Offset;
	float maxy = pos.y() + m_r + Offset;
	float maxz = pos.z() + Offset;
	float minz = pos.z() - m_height - Offset;

	m_minPt = QVector3D(minx, miny, minz);
	m_maxPt = QVector3D(maxx, maxy, maxz);*/
}

bool rdFramePilePlatform::ptIn(QVector3D pt)
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

void rdFramePilePlatform::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	//绘制
	coorBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_QUADS, 0, 72);
}

void rdFramePilePlatform::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
{
	//if (!m_bShow) return;

	//QVector3D size = (m_maxPt - m_minPt);
	//
	//QMatrix4x4 scaleMatrix;
	//scaleMatrix.scale(size);
	//QMatrix4x4 modelMatrix;
	//modelMatrix.translate(m_pos.x(),m_pos.y(),m_pos.z() - size.z()/2);

	//modelMatrix = modelMatrix * scaleMatrix;
	////QMatrix4x4 mpv = pv * modelMatrix;
	//program->setUniformValue("Model", modelMatrix);
	//pCube->drawBox(program);
}

bool rdFramePilePlatform::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdFramePilePlatform::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdFramePilePlatform::copy(rdBridgePileUp* pPileUp)
{
	rdFramePilePlatform* pPile = (rdFramePilePlatform*)pPileUp;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}