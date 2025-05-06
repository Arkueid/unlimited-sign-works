#include "rdBridgeBentCap.h"
#include "../common/common.h"

rdBridgeBentCap::rdBridgeBentCap()
{
	m_name = "1-1";
	//m_r = 0.50f;
	//m_height = 5.0f;
	m_center = QVector3D(0,0,0);
	m_pPadStone = 0;
}


rdBridgeBentCap::~rdBridgeBentCap(void)
{
	
} 

void rdBridgeBentCap::setPadStone(rdPadStone* pPadStone)
{
	m_pPadStone = pPadStone;
}

void rdBridgeBentCap::setPadStone(int padstonecubecount, float padstonecubewidth, float padstonecubeheight, int padstonecount,
	float padstonegap,float dis)
{
	m_padstonecubecount = padstonecubecount;
	m_padstonecubewidth = padstonecubewidth;
	m_padstonecubeheight = padstonecubeheight;
	m_padstonecount = padstonecount;
	m_padstonegap = padstonegap;
	m_padstonedis = dis;
}

void rdBridgeBentCap::setLeftPos(QVector3D pos)
{
	m_lpos = pos;
}

void rdBridgeBentCap::setRightPos(QVector3D pos)
{
	m_rpos = pos;
}

void rdBridgeBentCap::setDirPos(QVector3D leftPos, QVector3D rightPos)
{
	m_dirLeftPos = leftPos;
	m_dirRightPos = rightPos;
}

void rdBridgeBentCap::setLength(float length)
{
	m_length = length;	
}

void rdBridgeBentCap::setWidth(float width)
{
	m_width = width;	
}

void rdBridgeBentCap::setHeight(float h1, float h2, float h3, float h4)
{
	m_height1 = h1;
	m_height2 = h2;
	m_height3 = h3;
	m_height4 = h4;
}

void rdBridgeBentCap::setSubLength(float downlength, float uplength1, float uplength2, float uplength3)
{
	m_downlength = downlength;
	m_uplength1 = uplength1;
	m_uplength2 = uplength2;
	m_uplength3 = uplength3;
}

QVector3D rdBridgeBentCap::getPos()
{
	return m_center;
}

void rdBridgeBentCap::calPts(float width,bool bLeft, QVector3D& center, QVector2D& pt0, QVector2D& pt1)
{
	double halfwidth = m_width / 2;

	double dis = m_lpos.distanceToPoint(m_rpos);
	double dd = (width - dis) / 2;
	QVector3D lpt = m_lpos + (m_rpos - m_lpos) * (-dd / dis);
	QVector3D rpt = m_lpos + (m_rpos - m_lpos) * (1 + dd / dis);
	double m = (rpt.y() - lpt.y()) / (rpt.x() - lpt.x());
	double m1 = -1 / m;
	if (bLeft)
	{
		center = lpt;
		//float dis2 = lpt.distanceToPoint(rpt);
		double a = lpt.x();
		double b = lpt.y();
		
		double c1 = b + (1 / m) * a;
		double c = b - m * a;
		double xx = lpt.x() + 1;
		double yy = m1 * xx + c1;
		QVector2D p0 = QVector2D(a, b);
		QVector2D p1 = QVector2D(xx, yy);
		double dddd = p0.distanceToPoint(p1);
		double t0 = -halfwidth / dddd;
		double t1 = halfwidth / dddd;
		pt0 = p0 + (p1 - p0) * t0;
		pt1 = p0 + (p1 - p0) * t1;
	}
	else
	{
		center = rpt;
		double dis2 = lpt.distanceToPoint(rpt);
		double a = rpt.x();
		double b = rpt.y();
		/*double m = (rpt.y() - lpt.y()) / (rpt.x() - lpt.x());
		double m1 = -1 / m;*/
		double c1 = b + (1 / m) * a;
		double c = b - m * a;
		double xx = rpt.x() + 1;
		double yy = m1 * xx + c1;
		QVector2D p0 = QVector2D(a, b);
		QVector2D p1 = QVector2D(xx, yy);
		double dddd = p0.distanceToPoint(p1);
		double t0 = -halfwidth / dddd;
		double t1 = halfwidth / dddd;
		pt0 = p0 + (p1 - p0) * t0;
		pt1 = p0 + (p1 - p0) * t1;
	}
}

void rdBridgeBentCap::calCoord()
{
	QVector3D pts[16];

	QVector3D fpts[16];
	QVector3D bpts[16];

	float halflength = m_length / 2;
	float halfwidth = m_width / 2;

	/// <summary>
	float dis = m_lpos.distanceToPoint(m_rpos);
	float dd = (m_length - dis) / 2;
	QVector3D lpt = m_lpos + (m_rpos - m_lpos) * (-dd / dis);
	QVector3D rpt = m_lpos + (m_rpos - m_lpos) * (1 + dd / dis);
	float dis2 = lpt.distanceToPoint(rpt);
	double a = lpt.x();
	double b = lpt.y();
	double m = (rpt.y() - lpt.y()) / (rpt.x() - lpt.x());
	double m1 = -1 / m;
	double c1 = b + (1 / m) * a;
	double c = b - m * a;
	double xx = lpt.x() + 1;
	double yy = m1 * xx + c1;
	QVector2D p0 = QVector2D(a, b);
	QVector2D p1 = QVector2D(xx, yy);
	double dddd = p0.distanceToPoint(p1);
	double t0 = -halfwidth / dddd;
	double t1 = halfwidth / dddd;
	QVector2D ppp0 = p0 + (p1 - p0) * t0;
	QVector2D ppp1 = p0 + (p1 - p0) * t1;
	
	QVector2D pt0, pt1;
	QVector3D lcpt;
	calPts(m_length,true, lcpt,pt0, pt1);

	double df = ppp0.distanceToPoint(ppp1);
	
	//if (ff * f1 > 0) // ppp1前ppp0后
	{
		double h = lpt.z() + m_height1 + m_height2;
		QVector3D bp = QVector3D(ppp1.x(), ppp1.y(), h);
		fpts[0] = bp;
		bp = QVector3D(ppp0.x(), ppp0.y(), h);
		bpts[0] = bp;

		h = lpt.z() + m_height1;
		bp = QVector3D(ppp1.x(), ppp1.y(), h);
		fpts[1] = bp;
		bp = QVector3D(ppp0.x(), ppp0.y(), h);
		bpts[1] = bp;

		QVector3D pt;
		calPts(m_length - m_downlength, true,pt,pt0, pt1);
		
		h = pt.z();
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[2] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[2] = bp;

		calPts(m_length - m_downlength, false, pt, pt0, pt1);
		h = pt.z();
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[3] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[3] = bp;

		calPts(m_length, false, pt, pt0, pt1);
		h = pt.z() + m_height1;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[4] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[4] = bp;

		h = pt.z() + m_height1 + m_height2;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[5] = bp ;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[5] = bp;

		calPts(m_length - m_uplength1, false, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[6] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[6] = bp;

		h = pt.z() + m_height1 + m_height2 + m_height3;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[7] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[7] = bp;

		calPts(m_length - m_uplength1- m_uplength2, false, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2 + m_height3;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[8] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[8] = bp;

		calPts(m_length - m_uplength1 - m_uplength2 - m_uplength3, false, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2 + m_height4;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[9] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[9] = bp;

		h = pt.z() + m_height1 + m_height2;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[10] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[10] = bp;

		calPts(m_length - m_uplength1 - m_uplength2 - m_uplength3, true, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2 ;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[11] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[11] = bp;

		h = pt.z() + m_height1 + m_height2 + m_height4;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[12] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[12] = bp;

		calPts(m_length - m_uplength1 - m_uplength2, true, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2 + m_height3;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[13] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[13] = bp;

		calPts(m_length - m_uplength1, true, pt, pt0, pt1);
		h = pt.z() + m_height1 + m_height2 + m_height3;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[14] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[14] = bp;

		h = pt.z() + m_height1 + m_height2;
		bp = QVector3D(pt1.x(), pt1.y(), h);
		fpts[15] = bp;
		bp = QVector3D(pt0.x(), pt0.y(), h);
		bpts[15] = bp;
	}

	//确定前后
	double ff = m_dirLeftPos.y() - m_dirLeftPos.x() * m - c;
	double f0 = ppp0.y() - ppp0.x() * m - c;
	double f1 = ppp1.y() - ppp1.x() * m - c;

	if (ff * f1 > 0)
	{
		for (int i = 0; i < 16; i++) m_frontpts[i] = fpts[i];
		for (int i = 0; i < 16; i++) m_backpts[i] = bpts[i];
	}
	else
	{
		for (int i = 0; i < 16; i++) m_frontpts[i] = bpts[i];
		for (int i = 0; i < 16; i++) m_backpts[i] = fpts[i];
	}
	/// </summary>
	//计算垫石
	QVector3D dir = m_lpos - m_rpos;
	//QMatrix4x4 rotateMatrix;
	//rotateMatrix.rotate(0, 0, 0, 1);
	if (dir.x() == 0)//垂线
	{

	}
	else
	{
		float s = dir.y() / dir.x();
		float angle = atan(s);
		angle = angle * 180 / 3.14;
		m_angle = angle;
		//rotateMatrix.rotate(angle, 0, 0, 1);
	}

	float r = (m_padstonedis - m_padstonegap) / 2;
	m_padstoneradius = r;
	float wpadstonelength = 4 * r + 3 * m_padstonegap;
	m_padstonecubelength = wpadstonelength;
	QVector3D centerp0 = lpt;
	QVector3D centerp1 = rpt;
	
	float temph = m_height1 + m_height2;
	centerp0.setZ(centerp0.z() + temph);
	centerp1.setZ(centerp1.z() + temph);

	double offset = (m_length - 2 * (m_uplength1 + m_uplength2 + m_uplength3)- wpadstonelength) / (m_padstonecubecount-1);
	double len = m_length;
	double len0 = m_uplength1 + m_uplength2 + m_uplength3 + wpadstonelength / 2;
	int indexPadstone = 0;
	for (int i = 0; i < m_padstonecubecount; i++)
	{
		QVector3D pt = centerp0 + (centerp1 - centerp0) * (len0 / len);		
		pt.setZ(pt.z() + m_padstonecubeheight / 4);
		m_padstonecubePos[i] = pt;

		double len1 = len0 - r - m_padstonegap / 2;
		for (int j = 0; j < m_padstonecount; j++)
		{			
			QVector3D pt0 = centerp0 + (centerp1 - centerp0) * (len1 / len);
			pt0.setZ(pt0.z() + 3 * m_padstonecubeheight / 4);
			m_padstonePos[indexPadstone++] = pt0;

			len1 = len0 + r + m_padstonegap / 2;
			/*pt0 = centerp0 + (centerp1 - centerp0) * (len1 / len);
			pt0.setZ(pt0.z() + 3 * m_padstonecubeheight / 4);
			m_padstonePos[indexPadstone++] = pt0;*/
		}

		len0 += offset;
		_rendpadstonepara rpsp;
		rpsp.pos = m_padstonecubePos[i];
		rpsp.length = m_padstonecubelength;
		rpsp.width = m_padstonecubewidth;
		rpsp.height = m_padstonecubeheight / 2;

		rpsp.count = m_padstonecount;
		rpsp.r = m_padstoneradius;
		rpsp.h = m_padstonecubeheight / 2;

		for (int j = 0; j < m_padstonecount; j++)
		{
			rpsp.cylinderPos[j] = m_padstonePos[i * m_padstonecount + j];
		}
		
		rpsp.angle = m_angle;
		m_rpsp[i] = rpsp;
	}

	//QVector3D center = (m_lpos + m_rpos) / 2;
	//float ldz = m_lpos.z() - center.z();
	//float rdz = m_rpos.z() - center.z();
	//pts[0] = QVector3D(-halflength, -halfwidth, m_height1 + m_height2 + ldz);
	//pts[1] = QVector3D(-halflength, -halfwidth, m_height1 + ldz);
	//pts[2] = QVector3D(-halflength + m_downlength, -halfwidth, ldz);
	//pts[3] = QVector3D(halflength - m_downlength, -halfwidth, rdz);
	//pts[4] = QVector3D(halflength, -halfwidth, m_height1 + rdz);
	//pts[5] = QVector3D(halflength, -halfwidth, m_height1 + m_height2 + rdz);

	//pts[6] = QVector3D(halflength - m_uplength1, -halfwidth, m_height1 + m_height2 + rdz);
	//pts[7] = QVector3D(halflength - m_uplength1, -halfwidth, m_height1 + m_height2 + m_height3 + rdz);
	//pts[8] = QVector3D(halflength - m_uplength1- m_uplength2, -halfwidth, m_height1 + m_height2 + m_height3 + rdz);
	//pts[9] = QVector3D(halflength - m_uplength1 - m_uplength2 - m_uplength3, -halfwidth, m_height1 + m_height2 + m_height4 + rdz);
	//pts[10] = QVector3D(halflength - m_uplength1 - m_uplength2 - m_uplength3, -halfwidth, m_height1 + m_height2  + rdz);
	//pts[11] = QVector3D(-halflength + m_uplength1 + m_uplength2 + m_uplength3, -halfwidth, m_height1 + m_height2 + ldz);
	//pts[12] = QVector3D(-halflength + m_uplength1 + m_uplength2 + m_uplength3, -halfwidth, m_height1 + m_height2 + m_height4 + ldz);
	//pts[13] = QVector3D(-halflength + m_uplength1 + m_uplength2, -halfwidth, m_height1 + m_height2 + m_height3 + ldz);
	//pts[14] = QVector3D(-halflength + m_uplength1 , -halfwidth, m_height1 + m_height2 + m_height3 + ldz);	
	//pts[15] = QVector3D(-halflength + m_uplength1, -halfwidth, m_height1 + m_height2 + ldz);


	//QVector3D dir = m_lpos - m_rpos;
	////QMatrix4x4 rotateMatrix;
	////rotateMatrix.rotate(0, 0, 0, 1);
	//if (dir.x() == 0)//垂线
	//{

	//}
	//else
	//{
	//	float s = dir.y() / dir.x();
	//	float angle = atan(s);
	//	angle = angle * 180 / 3.14;
	//	m_angle = angle;
	//	//rotateMatrix.rotate(angle, 0, 0, 1);
	//}

	//QMatrix4x4 transMatrix;
	//transMatrix.translate(center);

	//QMatrix4x4 allMatrix = transMatrix * rotateMatrix;
	//for (int i = 0; i < 16; i++) m_frontpts[i] = allMatrix * pts[i];
	//for (int i = 0; i < 16; i++)pts[i].setY(halfwidth);
	//for (int i = 0; i < 16; i++) m_backpts[i] = allMatrix * pts[i];
}

void rdBridgeBentCap::initGeometry()
{
	int size = 28*2 + 4*16;
	QVector3D *pVertices = new QVector3D[size];//(12+6)*4
	QVector3D *pNormals = new QVector3D[size];
	int indexV = 0;
	int indexN = 0;
	QVector3D v1 = m_frontpts[0] - m_frontpts[1];
	QVector3D v2 = m_frontpts[2] - m_frontpts[1];
	QVector3D normal = - QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[11]; pNormals[indexN++] = normal;

	v1 = m_frontpts[2] - m_frontpts[3];
	v2 = m_frontpts[10] - m_frontpts[3];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[11]; pNormals[indexN++] = normal;

	v1 = m_frontpts[3] - m_frontpts[4];
	v2 = m_frontpts[5] - m_frontpts[4];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[10]; pNormals[indexN++] = normal;

	v1 = m_frontpts[8] - m_frontpts[7];
	v2 = m_frontpts[6] - m_frontpts[7];
	normal = QVector3D::crossProduct(v1, v2);

	for (int i = 6; i < 10; i++)
	{
		pVertices[indexV++] = m_frontpts[i]; 
		pNormals[indexN++] = normal;
	}

	v1 = m_frontpts[9] - m_frontpts[6];
	v2 = m_frontpts[10] - m_frontpts[6];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[10]; pNormals[indexN++] = normal;

	v1 = m_frontpts[14] - m_frontpts[13];
	v2 = m_frontpts[12] - m_frontpts[13];
	normal = QVector3D::crossProduct(v1, v2);

	for (int i = 11; i < 15; i++)
	{
		pVertices[indexV++] = m_frontpts[i];
		pNormals[indexN++] = normal;
	}

	v1 = m_frontpts[14] - m_frontpts[11];
	v2 = m_frontpts[15] - m_frontpts[11];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[14]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[15]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[15]; pNormals[indexN++] = normal;

	//后面
	v1 = m_backpts[0] - m_backpts[1];
	v2 = m_backpts[2] - m_backpts[1];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[1]; pNormals[indexN++] = normal;
		
	v1 = m_backpts[2] - m_backpts[3];
	v2 = m_backpts[10] - m_backpts[3];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[3]; pNormals[indexN++] = normal;
	
	v1 = m_backpts[3] - m_backpts[4];
	v2 = m_backpts[5] - m_backpts[4];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[4]; pNormals[indexN++] = normal;
	
	v1 = m_backpts[8] - m_backpts[7];
	v2 = m_backpts[6] - m_backpts[7];
	normal = -QVector3D::crossProduct(v1, v2);

	for (int i = 6; i < 10; i++)
	{
		pVertices[indexV++] = m_backpts[15-i];
		pNormals[indexN++] = normal;
	}

	v1 = m_backpts[9] - m_backpts[6];
	v2 = m_backpts[10] - m_backpts[6];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[10]; pNormals[indexN++] = normal;

	v1 = m_backpts[14] - m_backpts[13];
	v2 = m_backpts[12] - m_backpts[13];
	normal = -QVector3D::crossProduct(v1, v2);

	for (int i = 11; i < 15; i++)
	{
		pVertices[indexV++] = m_backpts[25-i];
		pNormals[indexN++] = normal;
	}

	v1 = m_backpts[14] - m_backpts[11];
	v2 = m_backpts[15] - m_backpts[11];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[14]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[15]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[15]; pNormals[indexN++] = normal;

	for (int i = 0; i < 16; i++)
	{
		int e = (i + 1) % 16;
		v1 = m_backpts[i] - m_frontpts[i];
		v2 = m_frontpts[e] - m_frontpts[i];
		normal = QVector3D::crossProduct(v1, v2);
		
		pVertices[indexV++] = m_frontpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_frontpts[e]; pNormals[indexN++] = normal;
	}

	initializeOpenGLFunctions();
	bool re = coorBuf.create();
	re = coorBuf.bind();
	coorBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = normBuf.create();
	normBuf.bind();
	normBuf.allocate(pNormals, size * sizeof(QVector3D));

	delete[] pVertices;
	delete[] pNormals;
}

void rdBridgeBentCap::calMinMaxPt()
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

bool rdBridgeBentCap::ptIn(QVector3D pt)
{
//	if (!ptInBox(pt)) return false;//包围盒测试
//
//	QVector3D center = m_pos - QVector3D(0, 0, m_height / 2);
//	float zmin = m_pos.z() - m_height;
//	float zmax = m_pos.z();
//
//	float offset = 0.1;
//
//	if (pt.z() > (zmax+offset) || pt.z() < (zmin-offset)) return false;
//	float r = (m_pos.x() - pt.x()) * (m_pos.x() - pt.x()) + (m_pos.y() - pt.y()) * (m_pos.y() - pt.y());
//
//	if (r > (m_r+offset) * (m_r+offset)) return false;
//
	return true;
}
//
void rdBridgeBentCap::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	
	int size = 120;// 28 * 2 + 4 * 16;
	coorBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_QUADS, 0, size);

	//垫石
	m_pPadStone->render(program, m_rpsp, m_padstonecubecount);
}

void rdBridgeBentCap::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
{
	if (!m_bShow) return;

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

bool rdBridgeBentCap::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdBridgeBentCap::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdBridgeBentCap::copy(rdBridgeBentCap* pBentCap)
{

}

QVector3D* rdBridgeBentCap::getPadStonePos(int& count)
{
	count = m_padstonecubecount;
	return m_padstonecubePos;
}

float rdBridgeBentCap::getPadStoneHeight()
{
	return m_padstonecubeheight;
}