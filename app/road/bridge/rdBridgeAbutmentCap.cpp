#include "rdBridgeAbutmentCap.h"
#include "../common/common.h"

rdBridgeAbutmentCap::rdBridgeAbutmentCap()
{
	m_name = "1-1";
	//m_r = 0.50f;
	//m_height = 5.0f;
	m_center = QVector3D(0,0,0);

	m_bHead = true;
	m_pPadStone = 0;
}


rdBridgeAbutmentCap::~rdBridgeAbutmentCap(void)
{
	
} 

void rdBridgeAbutmentCap::setPadStone(rdPadStone* pPadStone)
{
	m_pPadStone = pPadStone;
}

void rdBridgeAbutmentCap::setBridgeHead(bool bHead)
{
	m_bHead = bHead;
}

void rdBridgeAbutmentCap::setLeftPos(QVector3D pos)
{
	m_lpos = pos;
}

void rdBridgeAbutmentCap::setRightPos(QVector3D pos)
{
	m_rpos = pos;
}

void rdBridgeAbutmentCap::setLength(float length)
{
	m_length = length;	
}

void rdBridgeAbutmentCap::setWidth(float width)
{
	m_width = width;	
}

void rdBridgeAbutmentCap::setOffsetWidth(float width)
{
	m_offsetwidth = width;
}

void rdBridgeAbutmentCap::setHeight(float h1, float h2, float h3)
{
	m_height1 = h1;
	m_height2 = h2;
	m_height3 = h3;
}

void rdBridgeAbutmentCap::setSubLength(float uplength1, float uplength2, float uplength3, float uplength4)
{
	m_uplength1 = uplength1;
	m_uplength2 = uplength2;
	m_uplength3 = uplength3;
	m_uplength4 = uplength4;
}

void rdBridgeAbutmentCap::setBackWidth(float w1, float w2)
{
	m_backwidth1 = w1;
	m_backwidth2 = w2;
}

void rdBridgeAbutmentCap::setBackHeight(float h1, float h2, float h3, float h4)
{
	m_backheight1 = h1;
	m_backheight2 = h2;
	m_backheight3 = h3;
	m_backheight4 = h4;
}

QVector3D rdBridgeAbutmentCap::getPos()
{
	return m_center;
}

void rdBridgeAbutmentCap::setPadStone(int padstonecubecount, float padstonecubewidth, float padstonecubeheight, int padstonecount,
	float padstonegap, float padstonedis)
{
	m_padstonecubecount = padstonecubecount;
	m_padstonecubewidth = padstonecubewidth;
	m_padstonecubeheight = padstonecubeheight;
	m_padstonecount = padstonecount;
	m_padstonegap = padstonegap;
	m_padstonedis = padstonedis;
}

void rdBridgeAbutmentCap::calPts(float width, bool bLeft, QVector3D& center, QVector2D& pt0, QVector2D& pt1)
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

void rdBridgeAbutmentCap::calCoord()
{
	QVector3D pts[14];
	
	float halflength = m_length / 2;
	float halfwidth = m_width / 2;
/*QVector3D fpts[14];
	QVector3D bpts[14];
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
	calPts(m_length, true, lcpt, pt0, pt1);

	{
		double h = lpt.z() + m_height1 + m_height2;
		QVector3D bp = QVector3D(ppp1.x(), ppp1.y(), h);
		fpts[0] = bp;
		bp = QVector3D(ppp0.x(), ppp0.y(), h);
		bpts[0] = bp;
	}*/

	QVector3D center = (m_lpos + m_rpos) / 2;
	float ldz =  m_lpos.z() - center.z();
	float rdz =   m_rpos.z() - center.z();

	float backoffset;
	if (m_bHead)
	{
		halfwidth = m_offsetwidth;
		backoffset = m_width - m_offsetwidth;
	}
	else
	{
		halfwidth = m_width - m_offsetwidth;
		backoffset = m_offsetwidth;
	}

	pts[0] = QVector3D(-halflength, -halfwidth, m_height1 + ldz);
	pts[1] = QVector3D(-halflength, -halfwidth, ldz);
	pts[2] = QVector3D(halflength, -halfwidth, rdz);
	pts[3] = QVector3D(halflength, -halfwidth, m_height1 + rdz);

	pts[4] = QVector3D(halflength - m_uplength4, -halfwidth, m_height1 + rdz);
	pts[5] = QVector3D(halflength - m_uplength4, -halfwidth, m_height1 + m_height2 + rdz);
	pts[6] = QVector3D(halflength - m_uplength4 - m_uplength2, -halfwidth, m_height1 + m_height2 + rdz);
	pts[7] = QVector3D(halflength - m_uplength4 - m_uplength2 - m_uplength3, -halfwidth, m_height1 + m_height3 + rdz);
	pts[8] = QVector3D(halflength - m_uplength4 - m_uplength2 - m_uplength3, -halfwidth, m_height1 + rdz);	

	pts[9] = QVector3D(-halflength + m_uplength1 + m_uplength2 + m_uplength3, -halfwidth, m_height1 + ldz);
	pts[10] = QVector3D(-halflength + m_uplength1 + m_uplength2 + m_uplength3, -halfwidth, m_height1 + m_height3  + ldz);
	pts[11] = QVector3D(-halflength + m_uplength1 + m_uplength2 , -halfwidth, m_height1 + m_height2 + ldz);
	pts[12] = QVector3D(-halflength + m_uplength1, -halfwidth, m_height1 + m_height2  + ldz);
	pts[13] = QVector3D(-halflength + m_uplength1, -halfwidth, m_height1 + ldz);
	

	QVector3D dir = m_lpos - m_rpos;
	QMatrix4x4 rotateMatrix;
	rotateMatrix.rotate(0, 0, 0, 1);
	if (dir.x() == 0)//垂线
	{

	}
	else
	{
		float s = dir.y() / dir.x();
		float angle = atan(s);
		angle = angle * 180 / 3.14;
		rotateMatrix.rotate(angle, 0, 0, 1);

		m_angle = angle;
	}

	QMatrix4x4 transMatrix;
	transMatrix.translate(center);

	QMatrix4x4 allMatrix = transMatrix * rotateMatrix;
	for (int i = 0; i < 14; i++) m_frontpts[i] = allMatrix * pts[i];
	for (int i = 0; i < 14; i++) pts[i].setY(backoffset);
	for (int i = 0; i < 14; i++) m_backpts[i] = allMatrix * pts[i];

	//后部
	QVector3D* pFirst = m_frontpts; 
	QVector3D* pEnd = m_backpts;
	if (m_bHead)
	{
		pFirst = m_backpts;
		pEnd = m_frontpts;
	}
	{
		m_backleftpts[0] = pEnd[0];
		float dis = pEnd[0].distanceToPoint(pFirst[0]);
		m_backleftpts[1] = pEnd[0] + (pFirst[0] - pEnd[0]) * (m_backwidth1 / dis);
		m_backleftpts[2] = m_backleftpts[1] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3 + m_backheight4);
		m_backleftpts[3] = m_backleftpts[0] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3 + m_backheight4);
		m_backleftpts[4] = m_backleftpts[0] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3);
		m_backleftpts[5] = pEnd[0] + (pFirst[0] - pEnd[0]) * (-m_backwidth2 / dis)
			+ QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3);
		m_backleftpts[6] = m_backleftpts[5];
		m_backleftpts[6].setZ(m_backleftpts[5].z() - m_backheight3);

		m_backleftpts[7] = m_backleftpts[0] + QVector3D(0, 0, m_backheight1);
		//m_backrightpts[8];
		m_backrightpts[0] = pEnd[3];
		m_backrightpts[1] = pEnd[3] + (pFirst[3] - pEnd[3]) * (m_backwidth1 / dis);
		m_backrightpts[2] = m_backrightpts[1] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3 + m_backheight4);
		m_backrightpts[3] = m_backrightpts[0] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3 + m_backheight4);
		m_backrightpts[4] = m_backrightpts[0] + QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3);
		m_backrightpts[5] = pEnd[3] + (pFirst[3] - pEnd[3]) * (-m_backwidth2 / dis)
			+ QVector3D(0, 0, m_backheight1 + m_backheight2 + m_backheight3);
		m_backrightpts[6] = m_backrightpts[5];
		m_backrightpts[6].setZ(m_backrightpts[5].z() - m_backheight3);

		m_backrightpts[7] = m_backrightpts[0] + QVector3D(0, 0, m_backheight1);
	}

	//计算垫石
	float r = (m_padstonedis - m_padstonegap) / 2;
	m_padstoneradius = r;
	float wpadstonelength = 4 * r + 3 * m_padstonegap;
	m_padstonecubelength = wpadstonelength;

	QVector3D fp0 = m_frontpts[9];
	QVector3D fp1 = m_frontpts[8];
	QVector3D bp0 = m_backpts[9];
	QVector3D bp1 = m_backpts[8];
	
	double fw = m_width - m_backwidth1 - m_padstonecubewidth;
	double t0 = fw / m_width;
	QVector3D lp0 = fp0 + (bp0 - fp0) * t0;
	fw = m_width - m_backwidth1;
	double t1 = fw / m_width;
	QVector3D lp1 = fp0 + (bp0 - fp0) * t1;

	QVector3D rp0 = fp1 + (bp1 - fp1) * t0;
	QVector3D rp1 = fp1 + (bp1 - fp1) * t1;

	QVector3D centerp0 = (lp0 + lp1) / 2;
	QVector3D centerp1 = (rp0 + rp1) / 2;

	double offset = (m_length - m_uplength1 - 2 * (m_uplength2 + m_uplength3) - m_uplength4 - wpadstonelength- 2*m_padstonegap) / (m_padstonecubecount - 1);

	double len = m_length - m_uplength1 - 2 * (m_uplength2 + m_uplength3) - m_uplength4;
	double len0 = m_padstonegap + wpadstonelength / 2;
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
}

void rdBridgeAbutmentCap::initGeometry()
{
	int size = 20*2 + 14*4 + 8 + 8 + 8*4;
	QVector3D *pVertices = new QVector3D[size];
	QVector3D *pNormals = new QVector3D[size];
	int indexV = 0;
	int indexN = 0;
	
	//前面
	QVector3D v1 = m_frontpts[0] - m_frontpts[1];
	QVector3D v2 = m_frontpts[2] - m_frontpts[1];
	QVector3D normal = - QVector3D::crossProduct(v1, v2);
		
	//0123
	for (int i = 0; i < 4; i++)
	{
		pVertices[indexV++] = m_frontpts[i];
		pNormals[indexN++] = normal;
	}
	//4567
	v1 = m_frontpts[6] - m_frontpts[5];
	v2 = m_frontpts[4] - m_frontpts[5];
	normal = QVector3D::crossProduct(v1, v2);

	for (int i = 4; i < 8; i++)
	{
		pVertices[indexV++] = m_frontpts[i];
		pNormals[indexN++] = normal;
	}

	//4788
	v1 = m_frontpts[7] - m_frontpts[4];
	v2 = m_frontpts[8] - m_frontpts[4];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[8]; pNormals[indexN++] = normal;

	//9101112
	v1 = m_frontpts[11] - m_frontpts[10];
	v2 = m_frontpts[9] - m_frontpts[10];
	normal = QVector3D::crossProduct(v1, v2);

	for (int i = 9; i < 13; i++)
	{
		pVertices[indexV++] = m_frontpts[i];
		pNormals[indexN++] = normal;
	}

	//9121313
	v1 = m_frontpts[12] - m_frontpts[9];
	v2 = m_frontpts[13] - m_frontpts[9];
	normal = QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_frontpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[12]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[13]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_frontpts[13]; pNormals[indexN++] = normal;

	//后面
	//0123
	v1 = m_backpts[0] - m_backpts[1];
	v2 = m_backpts[2] - m_backpts[1];
	normal = QVector3D::crossProduct(v1, v2);
	for (int i = 0; i < 4; i++)
	{
		pVertices[indexV++] = m_backpts[i];
		pNormals[indexN++] = normal;
	}
	//4567
	v1 = m_backpts[6] - m_backpts[5];
	v2 = m_backpts[4] - m_backpts[5];
	normal = -QVector3D::crossProduct(v1, v2);

	for (int i = 4; i < 8; i++)
	{
		pVertices[indexV++] = m_backpts[i];
		pNormals[indexN++] = normal;
	}

	//4788
	v1 = m_backpts[7] - m_backpts[4];
	v2 = m_backpts[8] - m_backpts[4];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[8]; pNormals[indexN++] = normal;

	//9101112
	v1 = m_backpts[11] - m_backpts[10];
	v2 = m_backpts[9] - m_backpts[10];
	normal = -QVector3D::crossProduct(v1, v2);

	for (int i = 9; i < 13; i++)
	{
		pVertices[indexV++] = m_backpts[i];
		pNormals[indexN++] = normal;
	}

	//9121313
	v1 = m_backpts[12] - m_backpts[9];
	v2 = m_backpts[13] - m_backpts[9];
	normal = -QVector3D::crossProduct(v1, v2);

	pVertices[indexV++] = m_backpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[12]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[13]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_backpts[13]; pNormals[indexN++] = normal;

	for (int i = 0; i < 14; i++)
	{
		int e = (i + 1) % 14;
		v1 = m_backpts[i] - m_frontpts[i];
		v2 = m_frontpts[e] - m_frontpts[i];
		normal = QVector3D::crossProduct(v1, v2);
		
		pVertices[indexV++] = m_frontpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_frontpts[e]; pNormals[indexN++] = normal;
	}

	//后部左
	v1 = m_backleftpts[2] - m_backleftpts[1];
	v2 = m_backleftpts[0] - m_backleftpts[1];
	normal = QVector3D::crossProduct(v1, v2);
	if (m_bHead) normal = -normal;
	for (int i = 0; i < 8; i++)
	{
		pVertices[indexV++] = m_backleftpts[i];

		pNormals[indexN++] = normal;
	}
	//后部右
	for (int i = 0; i < 8; i++)
	{
		pVertices[indexV++] = m_backrightpts[i];
		pNormals[indexN++] = -normal;
	}

	for (int i = 0; i < 8; i++)
	{
		int e = (i + 1) % 8;
		v1 = m_backrightpts[i] - m_backleftpts[i];
		v2 = m_backleftpts[e] - m_backleftpts[i];
		normal = QVector3D::crossProduct(v1, v2);
		if (m_bHead) normal = -normal;

		pVertices[indexV++] = m_backleftpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backrightpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backrightpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_backleftpts[e]; pNormals[indexN++] = normal;
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

void rdBridgeAbutmentCap::calMinMaxPt()
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

bool rdBridgeAbutmentCap::ptIn(QVector3D pt)
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
void rdBridgeAbutmentCap::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	
	int size = 20*2 + 14*4 + 8 + 8 + 8*4;
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
	if (m_pPadStone) m_pPadStone->render(program, m_rpsp, m_padstonecubecount);
}

void rdBridgeAbutmentCap::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdBridgeAbutmentCap::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdBridgeAbutmentCap::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdBridgeAbutmentCap::copy(rdBridgeAbutmentCap* pBentCap)
{

}

QVector3D* rdBridgeAbutmentCap::getPadStonePos(int& count)
{
	count = m_padstonecubecount;
	return m_padstonecubePos;
}

float rdBridgeAbutmentCap::getPadStoneHeight()
{
	return m_padstonecubeheight;
}