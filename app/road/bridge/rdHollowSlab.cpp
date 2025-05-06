#include "rdHollowSlab.h"
#include "../common/common.h"
#include "common/orQStrLine.h"

rdHollowSlab::rdHollowSlab()
{

}

rdHollowSlab::~rdHollowSlab(void)
{
	
} 

bool rdHollowSlab::readFile(QTextStream& fin, _hollowSlabPara& hsp)
{
	QVector<QString>  words;

	//读长度和角度
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re <= 0) continue;
		if (re != 2) return false;
		hsp.length = words[0].toDouble();
		hsp.angle = words[1].toDouble();
		break;
	}
	//读中间和边参数
	bool bMiddle = false;
	bool bSide = true;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re <= 0) continue;
		QString qstr = words[0];
		if (qstr == "/") break;
		if (re == 17)//中间
		{
			int flag = words[0].toInt();
			if (flag == 1)
			{
				bMiddle = true;

				for (int i = 0; i < 8; i++) hsp.middle.w[i] = words[i + 1].toDouble();
				for (int i = 0; i < 8; i++) hsp.middle.h[i] = words[i + 9].toDouble();
			}

		}
		else if (re == 20)//边
		{
			bSide = true;
		}
		else break;
	}

	if (bMiddle && bSide) return true;

	return false;
}

void rdHollowSlab::SetPara(_hollowSlabPara hsp)
{
	m_hsp = hsp;
}

void rdHollowSlab::setLeftDir(QVector3D dir)
{
	m_ldir = dir;
}

void rdHollowSlab::setRightDir(QVector3D dir)
{
	m_rdir = dir;
}

void rdHollowSlab::calCoord()
{
	QVector3D outpts[10];
	QVector3D inpts[8];

	double halfwidth = m_hsp.middle.w[0] / 2.0;
	double height = 0;
	for (int i = 0; i < 4; i++) height += m_hsp.middle.h[i];
	outpts[0] = QVector3D(-halfwidth, 0, 0);
	outpts[1] = QVector3D( halfwidth, 0, 0);
	outpts[2] = QVector3D( halfwidth, 0, m_hsp.middle.h[0]);
	outpts[3] = QVector3D( halfwidth - m_hsp.middle.w[4], 0, m_hsp.middle.h[0] + m_hsp.middle.h[1]);
	outpts[4] = QVector3D( halfwidth - m_hsp.middle.w[3], 0, height - m_hsp.middle.h[3]);
	outpts[5] = QVector3D( halfwidth - m_hsp.middle.w[3] - m_hsp.middle.w[5], 0, height);
	outpts[6] = QVector3D(-halfwidth + m_hsp.middle.w[3] + m_hsp.middle.w[5], 0, height);
	outpts[7] = QVector3D(-halfwidth + m_hsp.middle.w[3], 0, height - m_hsp.middle.h[3]);
	outpts[8] = QVector3D(-halfwidth + m_hsp.middle.w[4], 0, m_hsp.middle.h[0] + m_hsp.middle.h[1]);
	outpts[9] = QVector3D(-halfwidth, 0, m_hsp.middle.h[0]);

	double w2 = m_hsp.middle.w[1];
	double w3 = m_hsp.middle.w[2];
	double w7 = m_hsp.middle.w[6];
	double w8 = m_hsp.middle.w[7];
	double ww = m_hsp.middle.w[0] - w2 - w3;
	double h5 = m_hsp.middle.h[4];
	double h6 = m_hsp.middle.h[5];
	double h7 = m_hsp.middle.h[6];
	double h8 = m_hsp.middle.h[7];
	inpts[0] = QVector3D(-halfwidth + w2 + w8, 0, h8);
	inpts[1] = QVector3D( halfwidth - ww - w8, 0, h8);
	inpts[2] = QVector3D( halfwidth - ww     , 0, h8+h7);
	double temph = height - h5 - h6;
	inpts[3] = QVector3D( halfwidth - ww, 0, temph);
	temph = height - h5;
	inpts[4] = QVector3D( halfwidth - ww - w7, 0, temph);

	inpts[5] = QVector3D( halfwidth - ww - w7, 0, temph);
	temph = height - h5 - h6;
	inpts[6] = QVector3D(-halfwidth + w2 + w7, 0, temph);

	inpts[7] = QVector3D(-halfwidth + w2 + w7, 0, h8 + h7);

	QVector3D center = QVector3D(-halfwidth, 0, -height / 2);
	QMatrix4x4 transMatrix;
	transMatrix.translate(center);

	for (int i = 0; i < 10; i++) m_moutcorrs[i] = transMatrix * outpts[i];
	for (int i = 0; i < 8; i++) m_mincorrs[i] = transMatrix * inpts[i];
}

void rdHollowSlab::initGeometry()
{
	QVector3D boutpts[10];
	QVector3D binpts[8];

	for (int i = 0; i < 10; i++)
	{
		boutpts[i] = m_moutcorrs[i];
		boutpts[i].setY(m_hsp.length);
	}
	for (int i = 0; i < 8; i++)
	{
		binpts[i] = m_mincorrs[i];
		binpts[i].setY(m_hsp.length);
	}

	QVector3D pt = QVector3D(0, -m_hsp.length/2, 0);
	QMatrix4x4 transMatrix;
	transMatrix.translate(pt);

	for (int i = 0; i < 10; i++)
	{
		m_moutcorrs[i] = transMatrix * m_moutcorrs[i];
		boutpts[i] = transMatrix * boutpts[i];
	}

	for (int i = 0; i < 8; i++)
	{
		m_mincorrs[i] = transMatrix * m_mincorrs[i];
		binpts[i] = transMatrix * binpts[i];
	}
	int size = 40*2+10*4+8*4;

	QVector3D *pVertices = new QVector3D[size];
	QVector3D *pNormals = new QVector3D[size];
	//前面
	int indexV = 0;
	int indexN = 0;
	QVector3D normal = QVector3D(0, -1, 0);
	//0
	pVertices[indexV++] = m_moutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[0]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = m_moutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;
	//2
	pVertices[indexV++] = m_moutcorrs[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[6]; pNormals[indexN++] = normal;

	//4
	pVertices[indexV++] = m_moutcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;

	//5
	pVertices[indexV++] = m_moutcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;

	//6
	pVertices[indexV++] = m_moutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[3]; pNormals[indexN++] = normal;

	//7
	pVertices[indexV++] = m_moutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;

	//8
	pVertices[indexV++] = m_moutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = m_moutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[1]; pNormals[indexN++] = normal;

	//后面
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	//2
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;

	//4
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;

	//5
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;

	//6
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;

	//7
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;

	//8
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;
	
	//外侧面
	for (int i = 0; i < 10; i++)
	{
		int e = (i + 1) % 10;
		QVector3D v0 = m_moutcorrs[e] - boutpts[e];
		QVector3D v1 = boutpts[i] - boutpts[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = normal.normalized();

		pVertices[indexV++] = boutpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = boutpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_moutcorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_moutcorrs[i]; pNormals[indexN++] = normal;
	}

	//内侧面
	for (int i = 0; i < 8; i++)
	{
		int e = (i + 1) % 8;
		QVector3D v0 = binpts[e] - m_mincorrs[e];
		QVector3D v1 = m_mincorrs[i] - m_mincorrs[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = normal.normalized();

		pVertices[indexV++] = m_mincorrs[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_mincorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = binpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = binpts[i]; pNormals[indexN++] = normal;
	}

	initializeOpenGLFunctions();
	bool re = coorMidBuf.create();
	re = coorMidBuf.bind();
	coorMidBuf.allocate(pVertices, size * sizeof(QVector3D));
	
	re = normMidBuf.create();
	normMidBuf.bind();
	normMidBuf.allocate(pNormals, size * sizeof(QVector3D));

	m_mpointsize = size;

	delete[] pVertices; 
	delete[] pNormals;
}

void rdHollowSlab::calMinMaxPt()
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

bool rdHollowSlab::ptInRightBox(QVector3D pt)
{
	if (pt.x() > m_maxRightPt.x() || pt.x() < m_minRightPt.x()) return false;
	if (pt.y() > m_maxRightPt.y() || pt.y() < m_minRightPt.y()) return false;
	if (pt.z() > m_maxRightPt.z() || pt.z() < m_minRightPt.z()) return false;

	return true;
}

bool rdHollowSlab::ptIn(QVector3D pt)
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

void rdHollowSlab::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	//绘制  中间
	int size = m_mpointsize;
	coorMidBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normMidBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);	
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_TRIANGLES, 0, size);

	/*coorrBuf.bind();	
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normrBuf.bind();	
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_TRIANGLES, 0, size);*/
}

void rdHollowSlab::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdHollowSlab::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdHollowSlab::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdHollowSlab::copy(rdBridgeGirder* pSlab)
{
	//rdHollowSlab* pPile = (rdHollowSlab*)pPier;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}

void rdHollowSlab::getEdgePoints(int type, QVector3D* pts)
{

}