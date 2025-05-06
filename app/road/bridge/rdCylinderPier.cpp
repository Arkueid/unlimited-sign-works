#include "rdCylinderPier.h"
#include "../common/common.h"

rdCylinderPier::rdCylinderPier()
{
	m_lheight = 5.0f;
	m_curlheight = 0.0f;

	m_rheight = 5.0f;
	m_currheight = 0.0f;
}


rdCylinderPier::~rdCylinderPier(void)
{
	
} 

void rdCylinderPier::setLeftPos(QVector3D pos)
{
	m_lpos = pos;
}

QVector3D rdCylinderPier::getLeftPos()
{
	return m_lpos;
}

void rdCylinderPier::setRightPos(QVector3D pos)
{
	m_rpos = pos;
}

QVector3D rdCylinderPier::getRightPos()
{
	return m_rpos;
}

void rdCylinderPier::setLeftRadius(float r)
{
	m_lr = r;
}

float rdCylinderPier::getLeftRadius()
{
	return m_lr;
}

void rdCylinderPier::setRightRadius(float r)
{
	m_rr = r;
}

float rdCylinderPier::getRightRadius()
{
	return m_rr;
}

void rdCylinderPier::setLeftHeight(float height)
{
	m_lheight = height;
}

float rdCylinderPier::getLeftHeight()
{
	return m_lheight;
}

void rdCylinderPier::setRightHeight(float height)
{
	m_rheight = height;
}

float rdCylinderPier::getRightHeight()
{
	return m_rheight;
}

void rdCylinderPier::setLeftDir(QVector3D dir)
{
	m_ldir = dir;
}

void rdCylinderPier::setRightDir(QVector3D dir)
{
	m_rdir = dir;
}

void rdCylinderPier::calCoord()
{
	QVector3D pts[CIRCLEPTS];
	QVector3D normals[CIRCLEPTS];

	float dalpha = 2 * 3.14159 / CIRCLEPTS;
	float alpha = 0;
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		pts[i] = QVector3D(m_lr * cos(alpha), m_lr * sin(alpha), 0);
		normals[i] = QVector3D(cos(alpha), sin(alpha), 0);
		alpha += dalpha;
	}
	
	QMatrix4x4 ltransMatrix;
	ltransMatrix.translate(m_lpos);

	for (int i = 0; i < CIRCLEPTS; i++) m_lcorrs[i] = ltransMatrix * pts[i];
	m_norms[0] = QVector3D(0, 0, 1);
	m_norms[1] = QVector3D(0, 0, -1);
	for (int i = 0; i < CIRCLEPTS; i++) m_norms[i+2] = normals[i];

	alpha = 0;
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		pts[i] = QVector3D(m_rr * cos(alpha), m_rr * sin(alpha), 0);
		alpha += dalpha;
	}

	QMatrix4x4 rtransMatrix;
	rtransMatrix.translate(m_rpos);

	for (int i = 0; i < CIRCLEPTS; i++) m_rcorrs[i] = rtransMatrix * pts[i];
}

void rdCylinderPier::initGeometry()
{
	int size = CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2;
	QVector3D vertices[CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2];
	QVector3D normals[CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2];
	//左
	int indexV = 0;
	int indexN = 0;
	//底面
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_lcorrs[i];normals[indexN++] = m_norms[1];
		vertices[indexV++] = m_lcorrs[e]; normals[indexN++] = m_norms[1];
		vertices[indexV++] = m_lpos;; normals[indexN++] = m_norms[1];
	}
	//顶面
	QVector3D pt = QVector3D(0, 0,m_lheight);
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_lcorrs[i] + pt; normals[indexN++] = m_norms[0];
		vertices[indexV++] = m_lcorrs[e] + pt; normals[indexN++] = m_norms[0];
		vertices[indexV++] = m_lpos + pt; normals[indexN++] = m_norms[0];
	}

	//侧面
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_lcorrs[i]; normals[indexN++] = m_norms[i + 2];
		vertices[indexV++] = m_lcorrs[e]; normals[indexN++] = m_norms[e + 2];	
		vertices[indexV++] = m_lcorrs[i] + pt; normals[indexN++] = m_norms[i + 2];
		
		vertices[indexV++] = m_lcorrs[e]; normals[indexN++] = m_norms[e + 2];
		vertices[indexV++] = m_lcorrs[e] + pt; normals[indexN++] = m_norms[e + 2];		
		vertices[indexV++] = m_lcorrs[i] + pt; normals[indexN++] = m_norms[i + 2];
	}

	initializeOpenGLFunctions();
	bool re = coorlBuf.create();	
	re = coorlBuf.bind();
	coorlBuf.allocate(vertices, size * sizeof(QVector3D));
	
	re = normlBuf.create();
	normlBuf.bind();
	normlBuf.allocate(normals, size * sizeof(QVector3D));

	//右
	indexV = 0;
	indexN = 0;
	//底面
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_rcorrs[i]; normals[indexN++] = m_norms[1];
		vertices[indexV++] = m_rcorrs[e]; normals[indexN++] = m_norms[1];
		vertices[indexV++] = m_rpos;; normals[indexN++] = m_norms[1];
	}
	//顶面
	pt = QVector3D(0, 0, m_rheight);
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_rcorrs[i] + pt; normals[indexN++] = m_norms[0];
		vertices[indexV++] = m_rcorrs[e] + pt; normals[indexN++] = m_norms[0];
		vertices[indexV++] = m_rpos + pt; normals[indexN++] = m_norms[0];
	}

	//侧面
	for (int i = 0; i < CIRCLEPTS; i++)
	{
		int e = (i + 1) % CIRCLEPTS;
		vertices[indexV++] = m_rcorrs[i]; normals[indexN++] = m_norms[i + 2];
		vertices[indexV++] = m_rcorrs[e]; normals[indexN++] = m_norms[e + 2];
		vertices[indexV++] = m_rcorrs[i] + pt; normals[indexN++] = m_norms[i + 2];

		vertices[indexV++] = m_rcorrs[e]; normals[indexN++] = m_norms[e + 2];
		vertices[indexV++] = m_rcorrs[e] + pt; normals[indexN++] = m_norms[e + 2];
		vertices[indexV++] = m_rcorrs[i] + pt; normals[indexN++] = m_norms[i + 2];
	}

	re = coorrBuf.create();
	re = coorrBuf.bind();
	coorrBuf.allocate(vertices, size * sizeof(QVector3D));

	re = normrBuf.create();
	normrBuf.bind();
	normrBuf.allocate(normals, size * sizeof(QVector3D));
}

void rdCylinderPier::calMinMaxPt()
{
	QVector3D pos = m_lpos;
	float minx = pos.x() - m_lr - Offset;
	float miny = pos.y() - m_lr - Offset;
	float maxx = pos.x() + m_lr + Offset;
	float maxy = pos.y() + m_lr + Offset;
	float maxz = pos.z() + Offset;
	float minz = pos.z() + m_lheight + Offset;

	m_minPt = QVector3D(minx, miny, minz);
	m_maxPt = QVector3D(maxx, maxy, maxz);

	pos = m_rpos;
	minx = pos.x() - m_rr - Offset;
	miny = pos.y() - m_rr - Offset;
	maxx = pos.x() + m_rr + Offset;
	maxy = pos.y() + m_rr + Offset;
	maxz = pos.z() + Offset;
	minz = pos.z() + m_rheight + Offset;

	m_minRightPt = QVector3D(minx, miny, minz);
	m_maxRightPt = QVector3D(maxx, maxy, maxz);
}

bool rdCylinderPier::ptInRightBox(QVector3D pt)
{
	if (pt.x() > m_maxRightPt.x() || pt.x() < m_minRightPt.x()) return false;
	if (pt.y() > m_maxRightPt.y() || pt.y() < m_minRightPt.y()) return false;
	if (pt.z() > m_maxRightPt.z() || pt.z() < m_minRightPt.z()) return false;

	return true;
}

bool rdCylinderPier::ptIn(QVector3D pt)
{
	bool binleft = ptInBox(pt);
	bool binright = ptInRightBox(pt);
	//if (!ptInBox(pt)) return false;//包围盒测试
	if (!binleft && !binright) return false;//包围盒测试

	//左测试
	QVector3D center = m_lpos - QVector3D(0, 0, m_lheight / 2);
	float zmin = m_lpos.z() ;
	float zmax = m_lpos.z() + m_lheight;

	float offset = 0.1;

	if (!(pt.z() > (zmax + offset) || pt.z() < (zmin - offset)))
	{
		float r = (m_lpos.x() - pt.x()) * (m_lpos.x() - pt.x()) + (m_lpos.y() - pt.y()) * (m_lpos.y() - pt.y());

		if (r <= (m_lr + offset) * (m_lr + offset)) return true;
	}
	//右测试
	center = m_rpos - QVector3D(0, 0, m_rheight / 2);
	zmin = m_rpos.z();
	zmax = m_rpos.z() + m_rheight;

	if (!(pt.z() > (zmax + offset) || pt.z() < (zmin - offset)))
	{
		float r = (m_rpos.x() - pt.x()) * (m_rpos.x() - pt.x()) + (m_rpos.y() - pt.y()) * (m_rpos.y() - pt.y());
		if (r <= (m_rr + offset) * (m_rr + offset)) return true;
	}

	return false;
}

void rdCylinderPier::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	//绘制
	int size = CIRCLEPTS * 3 * 2 + CIRCLEPTS * 3 * 2;
	coorlBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normlBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);	
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_TRIANGLES, 0, size);

	coorrBuf.bind();	
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normrBuf.bind();	
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_TRIANGLES, 0, size);
}

void rdCylinderPier::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdCylinderPier::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdCylinderPier::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdCylinderPier::copy(rdBridgePier* pPier)
{
	rdCylinderPier* pPile = (rdCylinderPier*)pPier;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}