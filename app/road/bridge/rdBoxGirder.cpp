#include "rdBoxGirder.h"
#include "../common/common.h"
#include "common/orQStrLine.h"

rdBoxGirder::rdBoxGirder()
{
}

rdBoxGirder::~rdBoxGirder(void)
{

} 

bool rdBoxGirder::readFile(QTextStream& fin, _boxGirderPara& bgp)
{
	QVector<QString>  words;

	//读长度和角度
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re <= 0) continue;
		if (re != 2) return false;
		bgp.length = words[0].toDouble();
		bgp.angle = words[1].toDouble();
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
		if (re == 34)//中间
		{
			int flag = words[0].toInt();
			if (flag == 1)
			{
				bMiddle = true;

				for (int i = 0; i < 16; i++) bgp.middle.w[i] = words[i + 1].toDouble();
				for (int i = 0; i < 14; i++) bgp.middle.h[i] = words[i + 17].toDouble();

				bgp.middle.r = words[31].toDouble();
				bgp.middle.ml = words[32].toDouble();
				bgp.middle.mr = words[33].toDouble();
			}

		}
		else if (re == 29)//边
		{
			int flag = words[0].toInt();
			if (flag == 0)
			{
				bSide = true;

				for (int i = 0; i < 13; i++) bgp.side.w[i] = words[i + 1].toDouble();
				for (int i = 0; i < 12; i++) bgp.side.h[i] = words[i + 14].toDouble();

				bgp.side.r = words[26].toDouble();
				bgp.side.ml = words[27].toDouble();
				bgp.side.mr = words[28].toDouble();
			}
		}
		else break;
	}

	if (bMiddle && bSide) return true;

	return false;
}

void rdBoxGirder::SetPara(_boxGirderPara bgp)
{
	m_bgp = bgp;
}

void rdBoxGirder::setLeftDir(QVector3D dir)
{
	m_ldir = dir;
}

void rdBoxGirder::setRightDir(QVector3D dir)
{
	m_rdir = dir;
}

void rdBoxGirder::calMidCoord()
{
	QVector3D outpts[12];
	QVector3D inpts[8];

	double heightL = 0;
	for (int i = 0; i < 3; i++) heightL += m_bgp.middle.h[i];
	double heightR = 0;
	for (int i = 0; i < 3; i++) heightR += m_bgp.middle.h[i + 6];

	outpts[0] = QVector3D(-m_bgp.middle.w[0], 0, 0);
	outpts[1] = QVector3D(m_bgp.middle.w[1], 0, 0);
	outpts[2] = QVector3D(m_bgp.middle.w[1] + m_bgp.middle.r, 0, m_bgp.middle.r);
	outpts[3] = QVector3D(m_bgp.middle.w[1] + m_bgp.middle.w[2], 0, m_bgp.middle.h[8]);
	outpts[4] = QVector3D(m_bgp.middle.w[1] + m_bgp.middle.w[2] + m_bgp.middle.w[3], 0,
		m_bgp.middle.h[8] + m_bgp.middle.h[7]);
	outpts[5] = QVector3D(m_bgp.middle.w[5], 0, m_bgp.middle.h[8] + m_bgp.middle.h[7]);
	outpts[6] = QVector3D(m_bgp.middle.w[5], 0, heightR);

	outpts[7] = QVector3D(-m_bgp.middle.w[6], 0, heightL);
	outpts[8] = QVector3D(-m_bgp.middle.w[6], 0, heightL - m_bgp.middle.h[2]);
	outpts[9] = QVector3D(-m_bgp.middle.w[6] + m_bgp.middle.w[7], 0, heightL - m_bgp.middle.h[2]);
	outpts[10] = QVector3D(-m_bgp.middle.w[6] + m_bgp.middle.w[7] + m_bgp.middle.w[8], 0,
		m_bgp.middle.h[0]);
	outpts[11] = QVector3D(-m_bgp.middle.w[1] - m_bgp.middle.r, 0, m_bgp.middle.r);

	double w = m_bgp.middle.w[5] + m_bgp.middle.w[6];
	double t = m_bgp.middle.w[5] / w;
	double hh = heightL + (heightR - heightL) * t;

	/*double wl = (hh - m_bgp.middle.w[4] - m_bgp.middle.w[13]) / m_bgp.middle.ml;
	double wr = (hh - m_bgp.middle.w[4] - m_bgp.middle.w[13]) / m_bgp.middle.mr;*/

	double sw1 = (m_bgp.middle.h[13] + m_bgp.middle.h[11]) / m_bgp.middle.ml;
	double x7 = outpts[11].x() + m_bgp.middle.w[14] - sw1;
	double x0 = x7 + m_bgp.middle.w[12];

	double sw2 = (m_bgp.middle.h[13] + m_bgp.middle.h[12]) / m_bgp.middle.mr;
	double x2 = outpts[2].x() - m_bgp.middle.w[15] + sw2;
	double x1 = x2 - m_bgp.middle.w[13];

	inpts[0] = QVector3D(x0, 0, m_bgp.middle.h[13]);
	inpts[7] = QVector3D(x7, 0, m_bgp.middle.h[13] + m_bgp.middle.h[11]);
	double lh = hh - m_bgp.middle.h[4] - m_bgp.middle.h[13] - m_bgp.middle.h[9] - m_bgp.middle.h[11];
	double lw = lh / m_bgp.middle.ml;
	inpts[6] = QVector3D(x7 - lw, 0, lh + m_bgp.middle.h[13] + m_bgp.middle.h[11]);
	inpts[5] = QVector3D(x7 - lw + m_bgp.middle.w[10], 0, lh + m_bgp.middle.h[13] + m_bgp.middle.h[11] + m_bgp.middle.h[9]);

	inpts[1] = QVector3D(x1, 0, m_bgp.middle.h[13]);
	inpts[2] = QVector3D(x2, 0, m_bgp.middle.h[13] + m_bgp.middle.h[12]);
	double rw = lh / m_bgp.middle.mr;
	inpts[3] = QVector3D(x2 + rw, 0, lh + m_bgp.middle.h[13] + m_bgp.middle.h[12]);
	inpts[4] = QVector3D(x2 + rw - m_bgp.middle.w[11], 0, lh + m_bgp.middle.h[13] + m_bgp.middle.h[12] + m_bgp.middle.h[10]);

	for (int i = 0; i < 12; i++) m_moutcorrs[i] = outpts[i];
	for (int i = 0; i < 8; i++) m_mincorrs[i] = inpts[i];
}

void rdBoxGirder::calSideCoord()
{
	QVector3D outpts[11];
	QVector3D inpts[8];

	double heightL = 0;
	for (int i = 0; i < 3; i++) heightL += m_bgp.side.h[i];
	double heightR = 0;
	for (int i = 0; i < 3; i++) heightR += m_bgp.side.h[i + 3];

	outpts[0] = QVector3D(-m_bgp.side.w[2], 0, 0);
	outpts[1] = QVector3D(m_bgp.side.w[3], 0, 0);
	outpts[2] = QVector3D(m_bgp.side.w[3] + m_bgp.side.r, 0, m_bgp.side.r);
	outpts[3] = QVector3D(m_bgp.side.w[3] + m_bgp.side.w[4], 0, m_bgp.side.h[3]);

	outpts[4] = QVector3D(m_bgp.side.w[3] + m_bgp.side.w[4] + m_bgp.side.w[5], 0,
		m_bgp.side.h[3] + m_bgp.side.h[4]);

	double rightw = m_bgp.side.w[3] + m_bgp.side.w[4] + m_bgp.side.w[5] + m_bgp.side.w[6];
	outpts[5] = QVector3D(rightw, 0, m_bgp.side.h[3] + m_bgp.side.h[4]);
	outpts[6] = QVector3D(rightw, 0, heightR);

	double leftw = m_bgp.side.w[0] + m_bgp.side.w[1] + m_bgp.side.w[2];
	outpts[7] = QVector3D(-leftw, 0, heightL);
	outpts[8] = QVector3D(-leftw, 0, heightL - m_bgp.side.h[2]);
	outpts[9] = QVector3D(-m_bgp.side.w[1] - m_bgp.side.w[2], 0, m_bgp.side.h[0]);
	outpts[10] = QVector3D(-m_bgp.side.w[2] - m_bgp.side.r, 0,m_bgp.side.r);


	double w = leftw + rightw;
	double t = leftw / w;
	//中心高度
	double mh = heightL + (heightR - heightL) * t;

	double sw1 = (m_bgp.side.h[11] + m_bgp.side.h[9]) / m_bgp.side.ml;
	double x7 = outpts[10].x() + m_bgp.side.w[7] - sw1;
	double x0 = x7 + m_bgp.side.w[11];

	double sw2 = (m_bgp.side.h[11] + m_bgp.side.h[10]) / m_bgp.side.mr;
	double x2 = outpts[2].x() - m_bgp.side.w[8] + sw2;
	double x1 = x2 - m_bgp.side.w[12];

	inpts[0] = QVector3D(x0, 0, m_bgp.side.h[11]);
	inpts[7] = QVector3D(x7, 0, m_bgp.side.h[11] + m_bgp.side.h[9]);
	double lh = mh - m_bgp.side.h[6] - m_bgp.side.h[7] - m_bgp.side.h[9] - m_bgp.side.h[11];
	double lw = lh / m_bgp.side.ml;
	inpts[6] = QVector3D(x7 - lw, 0, lh + m_bgp.side.h[11] + m_bgp.side.h[9]);
	inpts[5] = QVector3D(x7 - lw + m_bgp.side.w[9], 0, lh + m_bgp.side.h[7] + m_bgp.side.h[9] + m_bgp.side.h[11]);

	inpts[1] = QVector3D(x1, 0, m_bgp.side.h[11]);
	inpts[2] = QVector3D(x2, 0, m_bgp.side.h[11] + m_bgp.side.h[10]);
	double rw = lh / m_bgp.side.mr;
	inpts[3] = QVector3D(x2 + rw, 0, lh + m_bgp.side.h[11] + m_bgp.side.h[10]);
	inpts[4] = QVector3D(x2 + rw - m_bgp.side.w[10], 0, lh + m_bgp.side.h[11] + m_bgp.side.h[10] + m_bgp.side.h[8]);

	for (int i = 0; i < 11; i++) m_soutcorrs[i] = outpts[i];
	for (int i = 0; i < 8; i++) m_sincorrs[i] = inpts[i];
}

void rdBoxGirder::calCoord()
{
	calMidCoord();
	calSideCoord();
}

void rdBoxGirder::initMidGeometry()
{
	QVector3D boutpts[12];
	QVector3D binpts[8];

	for (int i = 0; i < 12; i++)
	{
		boutpts[i] = m_moutcorrs[i];
		boutpts[i].setY(m_bgp.length);
	}
	for (int i = 0; i < 8; i++)
	{
		binpts[i] = m_mincorrs[i];
		binpts[i].setY(m_bgp.length);
	}

	QVector3D pt = QVector3D(0, -m_bgp.length / 2, 0);
	QMatrix4x4 transMatrix;
	transMatrix.translate(pt);

	for (int i = 0; i < 12; i++)
	{
		m_moutcorrs[i] = transMatrix * m_moutcorrs[i];
		boutpts[i] = transMatrix * boutpts[i];
	}

	for (int i = 0; i < 8; i++)
	{
		m_mincorrs[i] = transMatrix * m_mincorrs[i];
		binpts[i] = transMatrix * binpts[i];
	}

	int size = 12 * 4 * 2 + 12 * 4 + 8 * 4;

	QVector3D* pVertices = new QVector3D[size];
	QVector3D* pNormals = new QVector3D[size];
	//前面
	int indexV = 0;
	int indexN = 0;
	QVector3D normal = QVector3D(0, -1, 0);
	//0
	pVertices[indexV++] = m_moutcorrs[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = m_moutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[0]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = m_moutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[1]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = m_moutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[2]; pNormals[indexN++] = normal;

	//4
	pVertices[indexV++] = m_moutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[3]; pNormals[indexN++] = normal;
	//5
	pVertices[indexV++] = m_moutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;

	//6
	pVertices[indexV++] = m_moutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;

	//7
	pVertices[indexV++] = m_moutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[8]; pNormals[indexN++] = normal;

	//8
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = m_moutcorrs[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;

	//10
	pVertices[indexV++] = m_moutcorrs[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_moutcorrs[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[6]; pNormals[indexN++] = normal;

	//11
	pVertices[indexV++] = m_moutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_mincorrs[4]; pNormals[indexN++] = normal;

	//后面
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = boutpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;

	//4
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	//5
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;

	//6
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;

	//7
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;

	//8
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = boutpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;

	//10
	pVertices[indexV++] = boutpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[11]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;

	//11
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;

	//外侧面
	for (int i = 0; i < 12; i++)
	{
		int e = (i + 1) % 12;
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

void rdBoxGirder::initSideGeometry()
{
	QVector3D boutpts[11];
	QVector3D binpts[8];

	for (int i = 0; i < 11; i++)
	{
		boutpts[i] = m_soutcorrs[i];
		boutpts[i].setY(m_bgp.length);
	}
	for (int i = 0; i < 8; i++)
	{
		binpts[i] = m_sincorrs[i];
		binpts[i].setY(m_bgp.length);
	}

	QVector3D pt = QVector3D(0, -m_bgp.length / 2, 0);
	QMatrix4x4 transMatrix;
	transMatrix.translate(pt);

	for (int i = 0; i < 11; i++)
	{
		m_soutcorrs[i] = transMatrix * m_soutcorrs[i];
		boutpts[i] = transMatrix * boutpts[i];
	}

	for (int i = 0; i < 8; i++)
	{
		m_sincorrs[i] = transMatrix * m_sincorrs[i];
		binpts[i] = transMatrix * binpts[i];
	}

	int size = 10 * 4 * 2 + 11 * 4 + 8 * 4;

	QVector3D* pVertices = new QVector3D[size];
	QVector3D* pNormals = new QVector3D[size];
	//前面
	int indexV = 0;
	int indexN = 0;
	QVector3D normal = QVector3D(0, -1, 0);
	//0
	pVertices[indexV++] = m_soutcorrs[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[7]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = m_soutcorrs[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[0]; pNormals[indexN++] = normal;

	//2
	pVertices[indexV++] = m_soutcorrs[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[1]; pNormals[indexN++] = normal;

	//3
	pVertices[indexV++] = m_soutcorrs[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[2]; pNormals[indexN++] = normal;

	//4
	pVertices[indexV++] = m_soutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[7]; pNormals[indexN++] = normal;

	//5
	pVertices[indexV++] = m_soutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[9]; pNormals[indexN++] = normal;

	//6
	pVertices[indexV++] = m_soutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[4]; pNormals[indexN++] = normal;

	//7
	pVertices[indexV++] = m_soutcorrs[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[3]; pNormals[indexN++] = normal;

	//8
	pVertices[indexV++] = m_soutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[5]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = m_soutcorrs[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_soutcorrs[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = m_sincorrs[6]; pNormals[indexN++] = normal;

	

	//后面
	normal = QVector3D(0, 1, 0);
	//0
	pVertices[indexV++] = boutpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	//1
	pVertices[indexV++] = boutpts[0]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[0]; pNormals[indexN++] = normal;
	//2
	pVertices[indexV++] = boutpts[1]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[1]; pNormals[indexN++] = normal;
	//3
	pVertices[indexV++] = boutpts[2]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[2]; pNormals[indexN++] = normal;
	//4
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	//5
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[8]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	//6
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;
	//7
	pVertices[indexV++] = boutpts[3]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[4]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[3]; pNormals[indexN++] = normal;
	//8
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[5]; pNormals[indexN++] = normal;

	//9
	pVertices[indexV++] = boutpts[9]; pNormals[indexN++] = normal;
	pVertices[indexV++] = boutpts[10]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[7]; pNormals[indexN++] = normal;
	pVertices[indexV++] = binpts[6]; pNormals[indexN++] = normal;

	//外侧面
	for (int i = 0; i < 11; i++)
	{
		int e = (i + 1) % 11;
		QVector3D v0 = m_soutcorrs[e] - boutpts[e];
		QVector3D v1 = boutpts[i] - boutpts[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = normal.normalized();

		pVertices[indexV++] = boutpts[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = boutpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_soutcorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_soutcorrs[i]; pNormals[indexN++] = normal;
	}

	//内侧面
	for (int i = 0; i < 8; i++)
	{
		int e = (i + 1) % 8;
		QVector3D v0 = binpts[e] - m_sincorrs[e];
		QVector3D v1 = m_sincorrs[i] - m_sincorrs[e];
		normal = QVector3D::crossProduct(v0, v1);
		normal = normal.normalized();

		pVertices[indexV++] = m_sincorrs[i]; pNormals[indexN++] = normal;
		pVertices[indexV++] = m_sincorrs[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = binpts[e]; pNormals[indexN++] = normal;
		pVertices[indexV++] = binpts[i]; pNormals[indexN++] = normal;
	}


	bool re = coorSideBuf.create();
	re = coorSideBuf.bind();
	coorSideBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = normSideBuf.create();
	normSideBuf.bind();
	normSideBuf.allocate(pNormals, size * sizeof(QVector3D));

	m_spointsize = size;

	delete[] pVertices;
	delete[] pNormals;
}

void rdBoxGirder::initGeometry()
{
	initializeOpenGLFunctions();

	initMidGeometry();
	initSideGeometry();
}

void rdBoxGirder::calMinMaxPt()
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

bool rdBoxGirder::ptInRightBox(QVector3D pt)
{
	if (pt.x() > m_maxRightPt.x() || pt.x() < m_minRightPt.x()) return false;
	if (pt.y() > m_maxRightPt.y() || pt.y() < m_minRightPt.y()) return false;
	if (pt.z() > m_maxRightPt.z() || pt.z() < m_minRightPt.z()) return false;

	return true;
}

bool rdBoxGirder::ptIn(QVector3D pt)
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

void rdBoxGirder::render(QOpenGLShaderProgram* program,int type)
{
	if (!m_bShow) return;
	//绘制  中间
	int size = m_spointsize; //m_mpointsize;
	if (type == 0) coorMidBuf.bind();
	else coorSideBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPosition");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	if (type == 0) normMidBuf.bind();
	else normSideBuf.bind();
	int normalLocation = program->attributeLocation("VertexNormal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	//size = 40+24;// 96 + 48;
	glDrawArrays(GL_QUADS, 0, size);

	/*coorrBuf.bind();
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	normrBuf.bind();
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
	glDrawArrays(GL_TRIANGLES, 0, size);*/
}

void rdBoxGirder::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdBoxGirder::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdBoxGirder::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdBoxGirder::copy(rdBridgeGirder* pSlab)
{
	//rdHollowSlab* pPile = (rdHollowSlab*)pPier;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}

void rdBoxGirder::getEdgePoints(int type, QVector3D* pts)
{
	switch (type)
	{
	case 0:
		pts[0] = m_moutcorrs[6];
		pts[1] = m_moutcorrs[7];
		pts[2] = m_moutcorrs[7];
		pts[2].setY(pts[2].y() + m_bgp.length);
		pts[3] = m_moutcorrs[6];
		pts[3].setY(pts[3].y() + m_bgp.length);
		break;
	case 1:
	case 3:
		pts[0] = m_soutcorrs[7];
		pts[1] = m_soutcorrs[6];
		pts[2] = m_soutcorrs[6];
		pts[2].setY(pts[2].y() + m_bgp.length);
		pts[3] = m_soutcorrs[7];
		pts[3].setY(pts[3].y() + m_bgp.length);
		break;
	case 2:
	case 4:
		pts[0] = m_soutcorrs[6];
		pts[1] = m_soutcorrs[7];
		pts[2] = m_soutcorrs[7];
		pts[2].setY(pts[2].y() + m_bgp.length);
		pts[3] = m_soutcorrs[6];
		pts[3].setY(pts[3].y() + m_bgp.length);
		for (int i = 0; i < 4; i++)
			pts[i].setX(-pts[i].x());
		break;
	}	
}