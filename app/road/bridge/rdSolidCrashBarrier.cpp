#include "rdSolidCrashBarrier.h"
#include "../common/common.h"
#include "common/orQStrLine.h"

rdSolidCrashBarrier::rdSolidCrashBarrier()
{
	m_bRight = true;
}

rdSolidCrashBarrier::~rdSolidCrashBarrier(void)
{
	
} 

bool rdSolidCrashBarrier::readFile(QTextStream& fin, _solidcrashbarrierPara& cbp)
{
	QVector<QString>  words;

	//读长度和角度
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re <= 0) continue;
		if (re != 1) return false;
		cbp.length = words[0].toDouble();
		break;
	}
	//读参数
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re <= 0) continue;
		QString qstr = words[0];
		if (qstr == "/") break;
		if (re == 12)//中间
		{
			for (int i = 0; i < 5; i++) cbp.w[i] = words[i].toDouble();
			for (int i = 0; i < 6; i++) cbp.h[i] = words[i + 5].toDouble();

			cbp.r = words[11].toDouble();
		
			return true;
		}
		else break;
	}

	return false;
}

void rdSolidCrashBarrier::SetPara(_solidcrashbarrierPara scbp)
{
	m_scbp = scbp;
}

void rdSolidCrashBarrier::setLeftDir(QVector3D dir)
{
	m_ldir = dir;
}

void rdSolidCrashBarrier::setRightDir(QVector3D dir)
{
	m_rdir = dir;
}

void rdSolidCrashBarrier::calCoord()
{
	//右侧
	QVector3D fpts[9];

	fpts[0] = QVector3D(-m_scbp.w[0], 0, 0);
	fpts[1] = QVector3D( 0, 0, 0);
	fpts[2] = QVector3D(0, 0, m_scbp.h[5]);
	fpts[3] = QVector3D(m_scbp.w[1], 0, m_scbp.h[5] + m_scbp.h[4]);
	fpts[4] = QVector3D(m_scbp.w[1], 0, m_scbp.h[5] + m_scbp.h[4] + m_scbp.h[3]);
	fpts[5] = QVector3D(-(m_scbp.w[2] - m_scbp.w[1]), 0, m_scbp.h[5] + m_scbp.h[4] + m_scbp.h[3]);
	fpts[6] = QVector3D(-(m_scbp.w[2] - m_scbp.w[1]) - m_scbp.r, 0, m_scbp.h[5] + m_scbp.h[4] + m_scbp.h[3] - m_scbp.r);
	double x7 = -(m_scbp.w[2] - m_scbp.w[1]) - m_scbp.r - m_scbp.w[3];
	fpts[7] = QVector3D(x7, 0, m_scbp.h[0] + m_scbp.h[1]);
	fpts[8] = QVector3D(x7 - m_scbp.w[4], 0, m_scbp.h[0]);
	
	for (int i = 0; i < 9; i++) m_rightcorrs[i] = fpts[i];
	for (int i = 0; i < 9; i++)
	{
		m_leftcorrs[i] = fpts[i];
		m_leftcorrs[i].setX(-m_leftcorrs[i].x());
	}
}

void rdSolidCrashBarrier::initGeometry()
{
	initializeOpenGLFunctions();

	QVector3D boutpts[9];

	for (int i = 0; i < 9; i++)
	{
		boutpts[i] = m_rightcorrs[i];
		boutpts[i].setY(m_scbp.length);
	}

	QVector3D pt = QVector3D(0, -m_scbp.length / 2, 0);
	QMatrix4x4 transMatrix;
	transMatrix.translate(pt);

	for (int i = 0; i < 9; i++)
	{
		m_rightcorrs[i] = transMatrix * m_rightcorrs[i];
		boutpts[i] = transMatrix * boutpts[i];
	}

	int size = 16*2 + 9 * 4;

	QVector3D* pVertices = new QVector3D[size];
	QVector3D* pNormals = new QVector3D[size];
	//前面
	int indexV = 0;
	int indexN = 0;
	QVector3D normal = QVector3D(0, -1, 0);
	//0
	pVertices[indexV++] = m_rightcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[8]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = m_rightcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[7]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = m_rightcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[6]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = m_rightcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_rightcorrs[5]; pNormals[indexN++] = normal;

	//后面
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;

	//外侧面
	for (int i = 0; i < 9; i++)
	{
		int e = (i + 1) % 9;
		QVector3D v0 = m_rightcorrs[e] - boutpts[e];
		QVector3D v1 = boutpts[i] - boutpts[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = normal.normalized();

		pVertices[indexV++] = boutpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = boutpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_rightcorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_rightcorrs[i]; pNormals[indexN++] = normal;
	}
	
	bool re = coorRightBuf.create();
	re = coorRightBuf.bind();
	coorRightBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = normRightBuf.create();
	normRightBuf.bind();
	normRightBuf.allocate(pNormals, size * sizeof(QVector3D));

	m_pointsize = size;

	//左侧
	for (int i = 0; i < 9; i++)
	{
		boutpts[i] = m_leftcorrs[i];
		boutpts[i].setY(m_scbp.length);
	}

	for (int i = 0; i < 9; i++)
	{
		m_leftcorrs[i] = transMatrix * m_leftcorrs[i];
		boutpts[i] = transMatrix * boutpts[i];
	}
	indexV = 0;
	indexN = 0;
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = m_leftcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[8]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = m_leftcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[7]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = m_leftcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[6]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = m_leftcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_leftcorrs[5]; pNormals[indexN++] = normal;

	//后面
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;

	//外侧面
	for (int i = 0; i < 9; i++)
	{
		int e = (i + 1) % 9;
		QVector3D v0 = m_leftcorrs[e] - boutpts[e];
		QVector3D v1 = boutpts[i] - boutpts[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = -normal.normalized();

		pVertices[indexV++] = boutpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = boutpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_leftcorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_leftcorrs[i]; pNormals[indexN++] = normal;
	}

	re = coorLeftBuf.create();
	re = coorLeftBuf.bind();
	coorLeftBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = normLeftBuf.create();
	normLeftBuf.bind();
	normLeftBuf.allocate(pNormals, size * sizeof(QVector3D));

	delete[] pVertices;
	delete[] pNormals;
}

void rdSolidCrashBarrier::calMinMaxPt()
{
	/*QVector3D pos = m_lpos;
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
	m_maxRightPt = QVector3D(maxx, maxy, maxz);*/
}

bool rdSolidCrashBarrier::ptInRightBox(QVector3D pt)
{
	if (pt.x() > m_maxRightPt.x() || pt.x() < m_minRightPt.x()) return false;
	if (pt.y() > m_maxRightPt.y() || pt.y() < m_minRightPt.y()) return false;
	if (pt.z() > m_maxRightPt.z() || pt.z() < m_minRightPt.z()) return false;

	return true;
}

bool rdSolidCrashBarrier::ptIn(QVector3D pt)
{
	//bool binleft = ptInBox(pt);
	//bool binright = ptInRightBox(pt);
	////if (!ptInBox(pt)) return false;//包围盒测试
	//if (!binleft && !binright) return false;//包围盒测试

	////左测试
	//QVector3D center = m_lpos - QVector3D(0, 0, m_lheight / 2);
	//float zmin = m_lpos.z() ;
	//float zmax = m_lpos.z() + m_lheight;

	//float offset = 0.1;

	//if (!(pt.z() > (zmax + offset) || pt.z() < (zmin - offset)))
	//{
	//	float r = (m_lpos.x() - pt.x()) * (m_lpos.x() - pt.x()) + (m_lpos.y() - pt.y()) * (m_lpos.y() - pt.y());

	//	if (r <= (m_lr + offset) * (m_lr + offset)) return true;
	//}
	////右测试
	//center = m_rpos - QVector3D(0, 0, m_rheight / 2);
	//zmin = m_rpos.z();
	//zmax = m_rpos.z() + m_rheight;

	//if (!(pt.z() > (zmax + offset) || pt.z() < (zmin - offset)))
	//{
	//	float r = (m_rpos.x() - pt.x()) * (m_rpos.x() - pt.x()) + (m_rpos.y() - pt.y()) * (m_rpos.y() - pt.y());
	//	if (r <= (m_rr + offset) * (m_rr + offset)) return true;
	//}

	return false;
}

void rdSolidCrashBarrier::render(QOpenGLShaderProgram* program,int type)
{
	if (!m_bShow) return;
	//绘制  中间
	int size = m_pointsize; //m_mpointsize;
	//coorLeftBuf.bind();
	if (type == 1) coorLeftBuf.bind();
	else coorRightBuf.bind();

	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	
	if (type == 1) normLeftBuf.bind();
	else normRightBuf.bind();
	//normLeftBuf.bind();

	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	////size = 40+24;// 96 + 48;
	glDrawArrays(GL_QUADS, 0, size);
}

void rdSolidCrashBarrier::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdSolidCrashBarrier::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdSolidCrashBarrier::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdSolidCrashBarrier::copy(rdBridgeCrashBarrier* pSlab)
{
	//rdHollowSlab* pPile = (rdHollowSlab*)pPier;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}