#include "rdRibbedPlateAbutment.h"
#include "../common/common.h"

rdRibbedPlateAbutment::rdRibbedPlateAbutment()
{
	m_lheight = 5.0f;
	m_rheight = 5.0f;

	m_bHead = true;
}


rdRibbedPlateAbutment::~rdRibbedPlateAbutment(void)
{
	
} 

void rdRibbedPlateAbutment::setBridgeHead(bool bHead)
{
	m_bHead = bHead;
}

bool rdRibbedPlateAbutment::head()
{
	return m_bHead;
}

void rdRibbedPlateAbutment::setLeftPos(QVector3D pos0, QVector3D pos1)
{
	m_lpos0 = pos0;
	m_lpos1 = pos1;
}

void rdRibbedPlateAbutment::setRightPos(QVector3D pos0, QVector3D pos1)
{
	m_rpos0 = pos0;
	m_rpos1 = pos1;
}

QVector3D rdRibbedPlateAbutment::getLeftPos()
{
	if (m_bHead) return m_lpos0;
	
	return m_lpos1;
}

QVector3D rdRibbedPlateAbutment::getRightPos()
{
	if (m_bHead) return m_rpos0;

	return m_rpos1;
}

void rdRibbedPlateAbutment::setWidth(float up,float down)
{
	m_upwidth = up;
	m_downwidth = down;
}

void rdRibbedPlateAbutment::setLength(float length)
{
	m_length = length;
}

void rdRibbedPlateAbutment::setLeftHeight(float height)
{
	m_lheight = height;
}

float rdRibbedPlateAbutment::getLeftHeight()
{
	return m_lheight;
}

void rdRibbedPlateAbutment::setRightHeight(float height)
{
	m_rheight = height;
}

float rdRibbedPlateAbutment::getRightHeight()
{
	return m_rheight;
}

void rdRibbedPlateAbutment::calVerticalLine2Pts(QVector2D p0, QVector2D p1, QVector2D p, float length, QVector2D& lp, QVector2D& rp)
{
	double x = p0.x();
	double y = p0.y();
	double m = (p1.y() - p0.y()) / (p1.x() - p0.x());
	double m1 = -1 / m;
	double c1 = p.y() + (1 / m) * p.x();
	double c = y - m * x;

	double xx = p.x() + 0.1;
	double yy = m1 * xx + c1;
	QVector2D pp0 = QVector2D(p.x(), p.y());
	QVector2D pp1 = QVector2D(xx, yy);
	double dddd = pp0.distanceToPoint(pp1);
	double halflength = length / 2;
	
	double t0 = -halflength / dddd;
	double t1 = halflength / dddd;
	lp = pp0 + (pp1 - pp0) * t0;
	rp = pp0 + (pp1 - pp0) * t1;

	double mm = (lp.y() - rp.y()) / (lp.x() - rp.x());
}

void rdRibbedPlateAbutment::calCoord()
{
	QVector3D uppts[4];
	QVector3D downpts[4];

	float dis = m_lpos0.distanceToPoint(m_lpos1);
	//lpos0 lpos1
	QVector3D p0,p1;
	p0 = m_lpos0;
	p1 = m_lpos1;
	if (!m_bHead)
	{
		p0 = m_lpos1;
		p1 = m_lpos0;
	}
	double x = p0.x();
	double y = p0.y();
	double m = (p1.y() - p0.y()) / (p1.x() - p0.x());
	double m1 = -1 / m;
	double c1 = y + (1 / m) * x;
	double c =  y - m * x;
	
	QVector2D p02 = QVector2D(p0.x(), p0.y());
	QVector2D p12 = QVector2D(p1.x(), p1.y());

	//下点
	QVector2D dlpt0, drpt0;
	calVerticalLine2Pts(p02, p12, p02, m_length, dlpt0, drpt0);

	double right = m_rpos0.y() - (m_rpos0.x() * m + c);
	double temp0 = dlpt0.y() - (dlpt0.x() * m + c);
	//判断左右
	if (right * temp0 > 0)
	{
		QVector2D temppt0 = dlpt0;
		dlpt0 = drpt0;
		drpt0 = temppt0;
	}
	
	QVector2D dlpt1, drpt1;
	calVerticalLine2Pts(p02, p12, p12, m_length, dlpt1, drpt1);
	temp0 = dlpt1.y() - (dlpt1.x() * m + c);

	//判断左右
	if (right * temp0 > 0)
	{
		QVector2D temppt1 = dlpt1;
		dlpt1 = drpt1;
		drpt1 = temppt1;
	}
	//上面4个点
	double t = m_upwidth / dis;
	QVector2D ulpt1, urpt1;
	ulpt1 = dlpt0 + (dlpt1 - dlpt0) * t;
	urpt1 = drpt0 + (drpt1 - drpt0) * t;


	int index = 0;
	double h = m_lpos0.z();
	if (m_bHead)
	{
		m_lcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(urpt1.x(), urpt1.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(ulpt1.x(), ulpt1.y(), h + m_lheight);

		m_lcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h);
		m_lcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h);
		m_lcorrs[index++] = QVector3D(drpt1.x(), drpt1.y(), h);
		m_lcorrs[index++] = QVector3D(dlpt1.x(), dlpt1.y(), h);
	}
	else
	{
		m_lcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(ulpt1.x(), ulpt1.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(urpt1.x(), urpt1.y(), h + m_lheight);
		m_lcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h + m_lheight);
		
		m_lcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h);
		m_lcorrs[index++] = QVector3D(dlpt1.x(), dlpt1.y(), h);
		m_lcorrs[index++] = QVector3D(drpt1.x(), drpt1.y(), h);
		m_lcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h);		
	}

	//右侧
	p0 = m_rpos0;
	p1 = m_rpos1;
	if (!m_bHead)
	{
		p0 = m_rpos1;
		p1 = m_rpos0;
	}
	x = p0.x();
	y = p0.y();
	m = (p1.y() - p0.y()) / (p1.x() - p0.x());
	m1 = -1 / m;
	c1 = y + (1 / m) * x;
	 c = y - m * x;

	p02 = QVector2D(p0.x(), p0.y());
	p12 = QVector2D(p1.x(), p1.y());

	//下点	
	calVerticalLine2Pts(p02, p12, p02, m_length, dlpt0, drpt0);
	float ddddl = dlpt0.distanceToPoint(drpt0);
	double left = m_lpos0.y() - (m_lpos0.x() * m + c);
	temp0 = drpt0.y() - (drpt0.x() * m + c);
	//判断左右
	if (left * temp0 > 0)
	{
		QVector2D temppt0 = dlpt0;
		dlpt0 = drpt0;
		drpt0 = temppt0;
	}

	calVerticalLine2Pts(p02, p12, p12, m_length, dlpt1, drpt1);
	temp0 = drpt1.y() - (drpt1.x() * m + c);

	//判断左右
	if (left * temp0 > 0)
	{
		QVector2D temppt1 = dlpt1;
		dlpt1 = drpt1;
		drpt1 = temppt1;
	}
	//上面4个点
	ulpt1 = dlpt0 + (dlpt1 - dlpt0) * t;
	urpt1 = drpt0 + (drpt1 - drpt0) * t;

	index = 0;
	h = m_rpos0.z();
	if (m_bHead)
	{
		m_rcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(urpt1.x(), urpt1.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(ulpt1.x(), ulpt1.y(), h + m_lheight);

		m_rcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h);
		m_rcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h);
		m_rcorrs[index++] = QVector3D(drpt1.x(), drpt1.y(), h);
		m_rcorrs[index++] = QVector3D(dlpt1.x(), dlpt1.y(), h);
	}
	else
	{
		m_rcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(ulpt1.x(), ulpt1.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(urpt1.x(), urpt1.y(), h + m_lheight);
		m_rcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h + m_lheight);
		
		m_rcorrs[index++] = QVector3D(dlpt0.x(), dlpt0.y(), h);
		m_rcorrs[index++] = QVector3D(dlpt1.x(), dlpt1.y(), h);
		m_rcorrs[index++] = QVector3D(drpt1.x(), drpt1.y(), h);
		m_rcorrs[index++] = QVector3D(drpt0.x(), drpt0.y(), h);		
	}
	
	//以下是旋转法计算
	//uppts[0] = QVector3D(0, -halflength, m_lheight);
	//uppts[1] = QVector3D(m_upwidth, -halflength, m_lheight);
	//uppts[2] = QVector3D(m_upwidth, halflength, m_lheight);
	//uppts[3] = QVector3D(0, halflength, m_lheight);

	//downpts[0] = QVector3D(0, -halflength, 0);
	//downpts[1] = QVector3D(m_downwidth, -halflength, 0);
	//downpts[2] = QVector3D(m_downwidth, halflength, 0);
	//downpts[3] = QVector3D(0, halflength, 0);

	////左侧肋板
	//QVector3D dir = m_lpos0 - m_lpos1;
	//QMatrix4x4 rotateMatrix;
	//rotateMatrix.rotate(0, 0, 0, 1);
	//if (dir.x() == 0)//垂线
	//{

	//}
	//else
	//{
	//	float s = dir.y() / dir.x();
	//	float angle = atan(s);
	//	angle = angle * 180 / 3.14;
	//	if (m_bHead) angle += 180;
	//	rotateMatrix.rotate(angle, 0, 0, 1);
	//}
	////左侧肋板
	//QVector3D pt = m_lpos0;
	//if (!m_bHead)pt = m_lpos1;
	//QMatrix4x4 translateMatrix;
	//translateMatrix.translate(pt);
	//QMatrix4x4 allMatrix = translateMatrix * rotateMatrix;

	//int index = 0;
	//for (int i = 0; i < 4; i++) m_lcorrs[index++] = allMatrix * uppts[i];
	//for (int i = 0; i < 4; i++) m_lcorrs[index++] = allMatrix * downpts[i];

	////右侧肋板
	// pt = m_rpos0;
	//if (!m_bHead)pt = m_rpos1;
	//QMatrix4x4 translateMatrixR;
	//translateMatrixR.translate(pt);
	//allMatrix = translateMatrixR * rotateMatrix;

	//index = 0;
	//for (int i = 0; i < 4; i++) m_rcorrs[index++] = allMatrix * uppts[i];
	//for (int i = 0; i < 4; i++) m_rcorrs[index++] = allMatrix * downpts[i];
}

void rdRibbedPlateAbutment::initGeometry()
{
	int size = 6 * 4 * 2;
	QVector3D vertices[6 * 4 * 2];
	QVector3D normals[6 * 4 * 2];
	
	int indexV = 0;
	int indexN = 0;
	QVector3D normal(0, 0, 1);
	
	//左
	//顶面
	for (int i = 0; i < 4; i++)
	{
		vertices[indexV++] = m_lcorrs[i];
		normals[indexN++] = normal;
	}
	//底面
	for (int i = 0; i < 4; i++)
	{
		vertices[indexV++] = m_lcorrs[i+4];
		normals[indexN++] = -normal;
	}

	//左侧面0374
	QVector3D v0 = m_lcorrs[3] - m_lcorrs[0];
	QVector3D v1 = m_lcorrs[7] - m_lcorrs[3];
	QVector3D nn = QVector3D::crossProduct(v0,v1);
	vertices[indexV++] = m_lcorrs[0];normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[3]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[7]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[4]; normals[indexN++] = nn;

	//右侧面 1562
	v0 = m_lcorrs[6] - m_lcorrs[5];
	v1 = m_lcorrs[1] - m_lcorrs[5];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_lcorrs[1]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[5]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[6]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[2]; normals[indexN++] = nn;

	//前面 1045
	v0 = m_lcorrs[0] - m_lcorrs[1];
	v1 = m_lcorrs[5] - m_lcorrs[1];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_lcorrs[1]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[0]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[4]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[5]; normals[indexN++] = nn;

	//后面 2673
	v0 = m_lcorrs[3] - m_lcorrs[7];
	v1 = m_lcorrs[6] - m_lcorrs[7];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_lcorrs[2]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[6]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[7]; normals[indexN++] = nn;
	vertices[indexV++] = m_lcorrs[3]; normals[indexN++] = nn;
	
	//右
	//顶面
	for (int i = 0; i < 4; i++)
	{
		vertices[indexV++] = m_rcorrs[i];
		normals[indexN++] = normal;
	}
	//底面
	for (int i = 0; i < 4; i++)
	{
		vertices[indexV++] = m_rcorrs[i + 4];
		normals[indexN++] = -normal;
	}

	//左侧面0374
	v0 = m_rcorrs[3] - m_rcorrs[0];
	v1 = m_rcorrs[7] - m_rcorrs[3];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_rcorrs[0]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[3]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[7]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[4]; normals[indexN++] = nn;

	//右侧面 1562
	v0 = m_rcorrs[6] - m_rcorrs[5];
	v1 = m_rcorrs[1] - m_rcorrs[5];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_rcorrs[1]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[5]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[6]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[2]; normals[indexN++] = nn;

	//前面 1045
	v0 = m_rcorrs[0] - m_rcorrs[1];
	v1 = m_rcorrs[5] - m_rcorrs[1];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_rcorrs[1]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[0]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[4]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[5]; normals[indexN++] = nn;

	//后面 2673
	v0 = m_rcorrs[3] - m_rcorrs[7];
	v1 = m_rcorrs[6] - m_rcorrs[7];
	nn = QVector3D::crossProduct(v0, v1);
	vertices[indexV++] = m_rcorrs[2]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[6]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[7]; normals[indexN++] = nn;
	vertices[indexV++] = m_rcorrs[3]; normals[indexN++] = nn;

	initializeOpenGLFunctions();
	bool re = coorBuf.create();	
	re = coorBuf.bind();
	coorBuf.allocate(vertices, size * sizeof(QVector3D));
	
	re = normBuf.create();
	normBuf.bind();
	normBuf.allocate(normals, size * sizeof(QVector3D));
}

void rdRibbedPlateAbutment::calMinMaxPt()
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

bool rdRibbedPlateAbutment::ptIn(QVector3D pt)
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

void rdRibbedPlateAbutment::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	//绘制
	int size = 6 * 4 * 2;
	coorBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);	
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_QUADS, 0, size);
}

void rdRibbedPlateAbutment::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdRibbedPlateAbutment::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdRibbedPlateAbutment::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdRibbedPlateAbutment::copy(rdBridgeAbutment* pAbutment)
{
	rdRibbedPlateAbutment* pPile = (rdRibbedPlateAbutment*)pAbutment;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}