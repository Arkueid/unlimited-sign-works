#include "rdSolidPilePlatform.h"
#include "../common/common.h"

rdSolidPilePlatform::rdSolidPilePlatform()
{
	m_height = 5.0f;
}


rdSolidPilePlatform::~rdSolidPilePlatform(void)
{
	
} 

void rdSolidPilePlatform::calCoord()
{
	QVector3D pts[4];
	pts[0] = QVector3D(-0.5, 0.5, 0);
	pts[1] = QVector3D(-0.5, -0.5, 0);
	pts[2] = QVector3D(0.5, -0.5, 0);
	pts[3] = QVector3D(0.5, 0.5, 0);
	
	QMatrix4x4 scaleMatrix;
	scaleMatrix.scale(m_length, m_width, 1);
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

	QMatrix4x4 allMatrix = transMatrix * rotateMatrix * scaleMatrix;
	for (int i=0; i<4; i++)
		m_corrs[i] = allMatrix * pts[i];

	QMatrix4x4 inverseMatrix = rotateMatrix.inverted();
	QMatrix4x4 temp = inverseMatrix.transposed();
	//上下
	m_norms[0] = QVector3D(0, 0, 1);
	m_norms[1] = QVector3D(0, 0, -1);
	//左右
	m_norms[2] = QVector3D(-1, 0, 0);
	m_norms[3] = QVector3D(1, 0, 0);
	//前后
	m_norms[4] = QVector3D(0,-1,  0);
	m_norms[5] = QVector3D(0, 1,  0);

	for (int i = 0; i < 6; i++) m_norms[i] = temp * m_norms[i];
}

void rdSolidPilePlatform::initGeometry()
{
	QVector3D vertices[24];
	QVector3D normals[24];
	int indexV = 0;
	int indexN = 0;
	for (int i = 0; i < 4; i++) vertices[indexV++] = m_corrs[i];
	for (int i = 0; i < 4; i++) normals[indexN++] = m_norms[0];

	for (int i = 0; i < 4; i++) vertices[indexV++] = m_corrs[i] - QVector3D(0,0,m_height);
	for (int i = 0; i < 4; i++) normals[indexN++] = m_norms[1];

	vertices[indexV++] = m_corrs[0]; normals[indexN++] = m_norms[2];
	vertices[indexV++] = m_corrs[0] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[2];
	vertices[indexV++] = m_corrs[1] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[2];
	vertices[indexV++] = m_corrs[1]; normals[indexN++] = m_norms[2];

	vertices[indexV++] = m_corrs[2]; normals[indexN++] = m_norms[3];
	vertices[indexV++] = m_corrs[2] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[3];
	vertices[indexV++] = m_corrs[3] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[3];
	vertices[indexV++] = m_corrs[3]; normals[indexN++] = m_norms[3];
	
	vertices[indexV++] = m_corrs[1]; normals[indexN++] = m_norms[4];
	vertices[indexV++] = m_corrs[1] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[4];
	vertices[indexV++] = m_corrs[2] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[4];
	vertices[indexV++] = m_corrs[2]; normals[indexN++] = m_norms[4];

	vertices[indexV++] = m_corrs[3]; normals[indexN++] = m_norms[5];
	vertices[indexV++] = m_corrs[3] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[5];
	vertices[indexV++] = m_corrs[0] - QVector3D(0, 0, m_height); normals[indexN++] = m_norms[5];
	vertices[indexV++] = m_corrs[0]; normals[indexN++] = m_norms[5];

	initializeOpenGLFunctions();
	bool re = coorBuf.create();	
	re = coorBuf.bind();
	coorBuf.allocate(vertices, 24 * sizeof(QVector3D));
	
	re = normBuf.create();
	normBuf.bind();
	normBuf.allocate(normals, 24 * sizeof(QVector3D));
}

void rdSolidPilePlatform::setLength(float l)
{
	m_length = l;
}

float rdSolidPilePlatform::getLength()
{
	return m_length;
}

void rdSolidPilePlatform::setHeight(float height)
{
	m_height = height;
}

void rdSolidPilePlatform::setWidth(float w)
{
	m_width = w;
}

float rdSolidPilePlatform::getWidth()
{
	return m_width;
}

float rdSolidPilePlatform::getHeight()
{
	return m_height;
}

void rdSolidPilePlatform::setDir(QVector3D dir)
{
	m_dir = dir;
}

void rdSolidPilePlatform::calMinMaxPt()
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

bool rdSolidPilePlatform::ptIn(QVector3D pt)
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

void rdSolidPilePlatform::render(QOpenGLShaderProgram* program)
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
	glDrawArrays(GL_QUADS, 0, 24);
}

void rdSolidPilePlatform::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdSolidPilePlatform::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdSolidPilePlatform::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdSolidPilePlatform::copy(rdBridgePileUp* pPileUp)
{
	rdSolidPilePlatform* pPile = (rdSolidPilePlatform*)pPileUp;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}