#include "rdBridgeConicalSlope.h"
#include "../common/common.h"
#include "../scene/rdTextures.h"

rdBridgeConicalSlope::rdBridgeConicalSlope()
{
	m_name = "1-1";
	m_conicalSlopeData.h0 = 1.4;
	m_conicalSlopeData.h = 4;//;
	m_conicalSlopeData.scale = 1.5;
	m_conicalSlopeData.platwidth = 1.2;
	m_conicalSlopeData.middlewidth = 14;
	m_conicalSlopeData.wallheight = 2.0f;
	m_conicalSlopeData.leftwidth = 0.5;
	m_conicalSlopeData.rightwidth = 0.3;
	m_conicalSlopeData.downwidth = 0.3;
	m_conicalSlopeData.downheight = 0.5f;
	m_conicalSlopeData.heightscale1 = 0.2;
	m_conicalSlopeData.heightscale2 = 0.25;
	m_conicalSlopeData.downscale = 0.2;

	m_conicalSlopeData.basewidth = 2.8;
	m_conicalSlopeData.baseheight = 0.8;
	m_conicalSlopeData.baseleft = 0.8;//
	m_conicalSlopeData.baseright = 0.8;//
}

rdBridgeConicalSlope::~rdBridgeConicalSlope(void)
{
	
} 

void rdBridgeConicalSlope::setPara(_conicalSlopePara csp)
{
	m_conicalSlopeData = csp;
}

_conicalSlopePara rdBridgeConicalSlope::getPara()
{
	return m_conicalSlopeData;
}
//void rdBridgeConicalSlope::seFlag(int flag)
//{
//	m_flag = flag;
//}
//
//int rdBridgeConicalSlope::getFlag()
//{
//	return m_flag;
//}

void rdBridgeConicalSlope::setPos(QVector3D pos)
{
	//m_pos = pos;
}

QVector3D rdBridgeConicalSlope::getPos()
{
	return QVector3D(0,0,0);
}

bool rdBridgeConicalSlope::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdBridgeConicalSlope::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

bool rdBridgeConicalSlope::ptIn(QVector3D pt)
{
	return false;
}

void rdBridgeConicalSlope::calMinMaxPt()
{

}

void rdBridgeConicalSlope::initTopSlopeGeometry(float h1, QVector<QVector3D>& topCircle, QVector<QVector3D>& slopePts,
	                                            QVector<QVector2D>& slopeTextures)
{
	QVector3D topPt;
	topPt = QVector3D(0, 0, 0);
	float h0 = m_conicalSlopeData.h0;
	float h = h1 + h0;
	float scale = m_conicalSlopeData.scale;
	float r = h * scale;
	float len = 4;
	int m = 20;

	float hh = sqrt(h * h + r * r);
	float t = hh / len;
	float lens = 0;
	//右侧
	for (int i = 0; i < m; i++)
	{
		slopePts.push_back(topPt);
		slopeTextures.push_back(QVector2D(0, 0));
		QVector3D bottomPt1 = topCircle[i];
		slopePts.push_back(bottomPt1);
		float s = lens / len;
		slopeTextures.push_back(QVector2D(s, t));
		QVector3D bottomPt2 = topCircle[i + 1];
		slopePts.push_back(bottomPt2);
		lens += bottomPt1.distanceToPoint(bottomPt2);
		s = lens / len;
		slopeTextures.push_back(QVector2D(s, t));
	}

	//中间
	QVector3D topP0 = topPt;
	QVector3D topP1 = topPt;
	topP1.setX(-m_conicalSlopeData.middlewidth);
	QVector3D bottomP0 = topCircle[m];
	QVector3D bottomP1 = bottomP0;
	bottomP1.setX(-m_conicalSlopeData.middlewidth);
	float s0 = 0;
	float t0 = 0;
	float s1 = m_conicalSlopeData.middlewidth / len;
	float t1 = h / len;

	slopePts.push_back(topP0);
	slopeTextures.push_back(QVector2D(s0, t0));
	slopePts.push_back(bottomP0);
	slopeTextures.push_back(QVector2D(s0, t1));
	slopePts.push_back(bottomP1);
	slopeTextures.push_back(QVector2D(s1, t1));

	slopePts.push_back(bottomP1);
	slopeTextures.push_back(QVector2D(s1, t1));
	slopePts.push_back(topP1);
	slopeTextures.push_back(QVector2D(s1, t0));
	slopePts.push_back(topP0);
	slopeTextures.push_back(QVector2D(s0, t0));

	//左侧
	float middlewidth = m_conicalSlopeData.middlewidth;
	QVector<QVector3D> lefttopCircle;
	int size = topCircle.size();
	for (int i = 0; i < size; i++)
	{
		lefttopCircle.push_back(topCircle[size-i-1]);
		lefttopCircle[i].setX(-lefttopCircle[i].x() - middlewidth);
	}

	lens = 0;
	topPt.setX(-middlewidth);
	for (int i = 0; i < m; i++)
	{
		slopePts.push_back(topPt);
		slopeTextures.push_back(QVector2D(0, 0));
		QVector3D bottomPt1 = lefttopCircle[i];
		slopePts.push_back(bottomPt1);
		float s = lens / len;
		slopeTextures.push_back(QVector2D(s, t));
		QVector3D bottomPt2 = lefttopCircle[i + 1];
		slopePts.push_back(bottomPt2);
		lens += bottomPt1.distanceToPoint(bottomPt2);
		s = lens / len;
		slopeTextures.push_back(QVector2D(s, t));
	}
}

void rdBridgeConicalSlope::initPlatformGeometry(QVector<QVector3D>& circle1, QVector<QVector3D>& circle2,
	QVector<QVector3D>& platformPts, QVector<QVector2D>& platformTextures)
{
	float inlens = 0;
	float outlens = 0;
	float platformlen = 2;
	float t = m_conicalSlopeData.platwidth / platformlen;
	int m = 20;

	//右侧
	for (int i = 0; i < m; i++)
	{
		float outs1 = outlens / platformlen;
		float ins1 = inlens / platformlen;

		QVector3D pt11 = circle1[i];
		QVector3D pt12 = circle1[i + 1];
		QVector3D pt21 = circle2[i];
		QVector3D pt22 = circle2[i + 1];

		inlens += pt11.distanceToPoint(pt12);
		outlens += pt21.distanceToPoint(pt22);

		float outs2 = outlens / platformlen;
		float ins2 = inlens / platformlen;

		platformPts.push_back(pt11);
		platformTextures.push_back(QVector2D(ins1, 0));
		platformPts.push_back(pt21);
		platformTextures.push_back(QVector2D(outs1, t));
		platformPts.push_back(pt22);
		platformTextures.push_back(QVector2D(outs2, t));

		platformPts.push_back(pt22);
		platformTextures.push_back(QVector2D(outs2, t));
		platformPts.push_back(pt12);
		platformTextures.push_back(QVector2D(ins2, 0));
		platformPts.push_back(pt11);
		platformTextures.push_back(QVector2D(ins1, 0));
	}

	//横向平台
	QVector3D pt11 = circle1[m];
	QVector3D pt12 = circle1[m];
	pt12.setX(-m_conicalSlopeData.middlewidth);
	QVector3D pt21 = circle2[m];
	QVector3D pt22 = circle2[m];
	pt22.setX(-m_conicalSlopeData.middlewidth);
	float outs1 = outlens / platformlen;
	float ins1 = inlens / platformlen;
	float outs2 = (outlens + m_conicalSlopeData.middlewidth) / platformlen;
	float ins2 = (inlens + m_conicalSlopeData.middlewidth) / platformlen;
	float t1 = 0;
	float t2 = t;

	platformPts.push_back(pt11);
	platformTextures.push_back(QVector2D(ins1, 0));
	platformPts.push_back(pt21);
	platformTextures.push_back(QVector2D(outs1, t));
	platformPts.push_back(pt22);
	platformTextures.push_back(QVector2D(outs2, t));

	platformPts.push_back(pt22);
	platformTextures.push_back(QVector2D(outs2, t));
	platformPts.push_back(pt12);
	platformTextures.push_back(QVector2D(ins2, 0));
	platformPts.push_back(pt11);
	platformTextures.push_back(QVector2D(ins1, 0));

	//左侧
	outlens = 0;
	inlens = 0;
	float middlewidth = m_conicalSlopeData.middlewidth;
	QVector<QVector3D> leftcircle1, leftcircle2;
	for (int i = 0; i < circle1.size(); i++)
	{
		leftcircle1.push_back(circle1[m-i]);
		leftcircle1[i].setX(-leftcircle1[i].x() - middlewidth);
		leftcircle2.push_back(circle2[m-i]);
		leftcircle2[i].setX(-leftcircle2[i].x() - middlewidth);
	}

	for (int i = 0; i < m; i++)
	{
		float outs1 = outlens / platformlen;
		float ins1 = inlens / platformlen;

		QVector3D pt11 = leftcircle1[i];
		QVector3D pt12 = leftcircle1[i + 1];
		QVector3D pt21 = leftcircle2[i];
		QVector3D pt22 = leftcircle2[i + 1];

		inlens += pt11.distanceToPoint(pt12);
		outlens += pt21.distanceToPoint(pt22);

		float outs2 = outlens / platformlen;
		float ins2 = inlens / platformlen;

		platformPts.push_back(pt11);
		platformTextures.push_back(QVector2D(ins1, 0));
		platformPts.push_back(pt21);
		platformTextures.push_back(QVector2D(outs1, t));
		platformPts.push_back(pt22);
		platformTextures.push_back(QVector2D(outs2, t));

		platformPts.push_back(pt22);
		platformTextures.push_back(QVector2D(outs2, t));
		platformPts.push_back(pt12);
		platformTextures.push_back(QVector2D(ins2, 0));
		platformPts.push_back(pt11);
		platformTextures.push_back(QVector2D(ins1, 0));
	}
}

void rdBridgeConicalSlope::initBottomSlopeGeometry(QVector<QVector3D>& midCircle, QVector<QVector3D>& downCircle,
	QVector<QVector3D>& slopePts, QVector<QVector2D>& slopeTextures)
{
	float h0 = m_conicalSlopeData.h0;
	float h = m_conicalSlopeData.h + h0;
	float scale = m_conicalSlopeData.scale;
	float z1 = -1.5 - h0;
	float r1 = -z1 * scale;
	
	float len = 4;
	float t = (h * scale - r1) / len;
	float inlens = 0;
	float outlens = 0;

	int m = 20;
	//右侧
	for (int i = 0; i < m; i++)
	{
		float outs1 = outlens / len;
		float ins1 = inlens / len;

		QVector3D pt11 = midCircle[i];
		QVector3D pt12 = midCircle[i + 1];
		QVector3D pt21 = downCircle[i];
		QVector3D pt22 = downCircle[i + 1];

		inlens += pt11.distanceToPoint(pt12);
		outlens += pt21.distanceToPoint(pt22);

		float outs2 = outlens / len;
		float ins2 = inlens / len;

		slopePts.push_back(pt11);
		slopeTextures.push_back(QVector2D(ins1, 0));
		slopePts.push_back(pt21);
		slopeTextures.push_back(QVector2D(outs1, t));
		slopePts.push_back(pt22);
		slopeTextures.push_back(QVector2D(outs2, t));

		slopePts.push_back(pt22);
		slopeTextures.push_back(QVector2D(outs2, t));
		slopePts.push_back(pt12);
		slopeTextures.push_back(QVector2D(ins2, 0));
		slopePts.push_back(pt11);
		slopeTextures.push_back(QVector2D(ins1, 0));
	}

	//中间
	QVector3D pt11 = midCircle[m];
	QVector3D pt12 = midCircle[m];
	pt12.setX(-m_conicalSlopeData.middlewidth);
	QVector3D pt21 = downCircle[m];
	QVector3D pt22 = downCircle[m];
	pt22.setX(-m_conicalSlopeData.middlewidth);
	float outs1 = 0 / len;
	float ins1 = 0 / len;
	float outs2 = (m_conicalSlopeData.middlewidth) / len;
	float ins2 = (m_conicalSlopeData.middlewidth) / len;
	float t1 = 0;
	float t2 = t;

	slopePts.push_back(pt11);
	slopeTextures.push_back(QVector2D(ins1, 0));
	slopePts.push_back(pt21);
	slopeTextures.push_back(QVector2D(outs1, t));
	slopePts.push_back(pt22);
	slopeTextures.push_back(QVector2D(outs2, t));

	slopePts.push_back(pt22);
	slopeTextures.push_back(QVector2D(outs2, t));
	slopePts.push_back(pt12);
	slopeTextures.push_back(QVector2D(ins2, 0));
	slopePts.push_back(pt11);
	slopeTextures.push_back(QVector2D(ins1, 0));

	//左侧
	inlens = 0;
	outlens = 0;
	QVector<QVector3D> leftmidCircle;
	QVector<QVector3D> leftdownCircle;
	float middlewidth = m_conicalSlopeData.middlewidth;
	for (int i = 0; i < midCircle.size(); i++)
	{
		leftmidCircle.push_back(midCircle[m-i]);
		leftmidCircle[i].setX(-leftmidCircle[i].x() - middlewidth);
		leftdownCircle.push_back(downCircle[m-i]);
		leftdownCircle[i].setX(-leftdownCircle[i].x() - middlewidth);
	}
	
	for (int i = 0; i < m; i++)
	{
		float outs1 = outlens / len;
		float ins1 = inlens / len;

		QVector3D pt11 = leftmidCircle[i];
		QVector3D pt12 = leftmidCircle[i + 1];
		QVector3D pt21 = leftdownCircle[i];
		QVector3D pt22 = leftdownCircle[i + 1];

		inlens += pt11.distanceToPoint(pt12);
		outlens += pt21.distanceToPoint(pt22);

		float outs2 = outlens / len;
		float ins2 = inlens / len;

		slopePts.push_back(pt11);
		slopeTextures.push_back(QVector2D(ins1, 0));
		slopePts.push_back(pt21);
		slopeTextures.push_back(QVector2D(outs1, t));
		slopePts.push_back(pt22);
		slopeTextures.push_back(QVector2D(outs2, t));

		slopePts.push_back(pt22);
		slopeTextures.push_back(QVector2D(outs2, t));
		slopePts.push_back(pt12);
		slopeTextures.push_back(QVector2D(ins2, 0));
		slopePts.push_back(pt11);
		slopeTextures.push_back(QVector2D(ins1, 0));
	}
}

void rdBridgeConicalSlope::initWallGeometry(QVector<QVector3D>* pBottomCircle, QVector<QVector3D>& wallpts,
	QVector<QVector2D>& walltexts, QVector<QVector3D>& basepts, QVector<QVector2D>& basetexts)
{
	float len = 4;
	//围墙
	QVector3D wallPts[6];
	QVector3D wallbasePts[7];
	wallPts[0] = QVector3D(-m_conicalSlopeData.leftwidth, 0, 0);
	wallPts[5] = QVector3D(m_conicalSlopeData.rightwidth, 0, 0);
	float temph = m_conicalSlopeData.wallheight - m_conicalSlopeData.downheight;
	float x = temph * m_conicalSlopeData.heightscale2;
	QVector3D p4 = QVector3D(m_conicalSlopeData.rightwidth + x, 0,
		-(m_conicalSlopeData.wallheight - m_conicalSlopeData.downheight));
	wallPts[4] = p4;
	QVector3D p3 = QVector3D(p4.x() + m_conicalSlopeData.downwidth, 0, p4.z());
	x = m_conicalSlopeData.downheight * m_conicalSlopeData.heightscale2;
	wallPts[3] = p3;
	QVector3D p2 = QVector3D(p3.x() + x, 0, -m_conicalSlopeData.wallheight);
	x = m_conicalSlopeData.wallheight * m_conicalSlopeData.heightscale1;
	wallPts[2] = p2;
	float tempX = -m_conicalSlopeData.leftwidth + x;
	float tempX1 = p2.x();
	float tempZ = (tempX1 - tempX) * m_conicalSlopeData.downscale;
	float p1Z = -m_conicalSlopeData.wallheight - tempZ;
	float p1X = -m_conicalSlopeData.leftwidth - p1Z * m_conicalSlopeData.heightscale1;
	wallPts[1] = QVector3D(p1X, 0, p1Z);
	//围墙底座
	wallbasePts[0] = QVector3D(tempX, 0, -m_conicalSlopeData.wallheight);
	wallbasePts[1] = wallbasePts[0];
	wallbasePts[1].setX(wallbasePts[0].x() - m_conicalSlopeData.baseleft);
	wallbasePts[2] = wallbasePts[1];
	wallbasePts[2].setZ(wallbasePts[0].z() - m_conicalSlopeData.baseheight);
	wallbasePts[3] = wallbasePts[2];
	wallbasePts[3].setX(wallbasePts[2].x() + m_conicalSlopeData.basewidth);
	wallbasePts[4] = wallbasePts[3];
	wallbasePts[4].setZ(wallbasePts[1].z());
	wallbasePts[5] = wallPts[2];
	wallbasePts[6] = wallPts[1];

	QVector2D wallPtsText[6];
	QVector2D wallbaseText[7];
	wallPtsText[0] = QVector2D(0, 0);
	for (int i = 1; i < 6; i++)
	{
		float temps, tempt;
		temps = fabs((wallPts[i].x() - wallPts[0].x())) / len;
		tempt = fabs((wallPts[i].z() - wallPts[0].z())) / len;
		wallPtsText[i] = QVector2D(temps, tempt);
	}

	wallbaseText[1] = QVector2D(0, 0);
	float temps, tempt;
	temps = fabs((wallbasePts[0].x() - wallbasePts[1].x())) / len;
	tempt = fabs((wallbasePts[0].z() - wallbasePts[1].z())) / len;
	wallbaseText[0] = QVector2D(temps, tempt);
	for (int i = 2; i < 7; i++)
	{
		float temps, tempt;
		temps = fabs((wallbasePts[i].x() - wallbasePts[1].x())) / len;
		tempt = fabs((wallbasePts[i].z() - wallbasePts[1].z())) / len;
		wallbaseText[i] = QVector2D(temps, tempt);
	}

	QVector3D zaxes = QVector3D(0, 0, 1);

	int m = 20;
	QVector3D pt = (*pBottomCircle)[0];
	QMatrix4x4 moveMatrix;
	moveMatrix.translate(pt);

	QVector<QVector3D> pts1;
	QVector<QVector3D> basepts1;
	QVector<QVector3D> pts2;
	QVector<QVector3D> basepts2;

	for (int i = 0; i < 6; i++)
	{
		pt = wallPts[i];
		QVector4D pt4 = QVector4D(pt, 1);
		QVector4D newpt4 = moveMatrix * pt4;
		QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
		pts1.push_back(newpt);
	}

	QVector2D walltexture[6];
	for (int i = 0; i < 6; i++)
	{
		pt = wallPts[i];
		float temps = fabs(pt.x() - wallPts[0].x()) / len;
		float tempt = fabs(pt.z() - wallPts[0].z()) / len;
		walltexture[i] = QVector2D(temps, tempt);
	}

	for (int i = 0; i < 7; i++)
	{
		pt = wallbasePts[i];
		QVector4D pt4 = QVector4D(pt, 1);
		QVector4D newpt4 = moveMatrix * pt4;
		QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
		basepts1.push_back(newpt);	
	}

	QVector2D wallbasetexture[7];
	for (int i = 0; i < 7; i++)
	{
		pt = wallbasePts[i];
		float temps = fabs(pt.x() - wallbasePts[1].x()) / len;
		float tempt = fabs(pt.z() - wallbasePts[1].z()) / len;
		wallbasetexture[i] = QVector2D(temps, tempt);
	}

	float dalpha = 90.0f / m;	
	float alpha = 0;
	if (!m_conicalSlopeData.bHead)
	{
		dalpha = -90.0f / m;
	}

	float lens = 0;
	float s1 = 0;
	//右侧
	for (int i = 1; i <= m; i++)
	{
		alpha += dalpha;
		QVector3D pt = (*pBottomCircle)[i];
		QMatrix4x4 rotateMatrix;
		rotateMatrix.rotate(alpha, zaxes);
		QMatrix4x4 moveMatrix;
		moveMatrix.translate(pt);

		QMatrix4x4 mat = moveMatrix * rotateMatrix;

		float templen = pt.distanceToPoint((*pBottomCircle)[i - 1]);
		lens += templen;
		float s2 = lens / len;
		pts2.clear();
		for (int j = 0; j < 6; j++)
		{
			pt = wallPts[j];
			QVector4D pt4 = QVector4D(pt, 1);
			QVector4D newpt4 = mat * pt4;
			QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
			pts2.push_back(newpt);
		}
		//生成三角形
		for (int j = 0; j < 6; j++)
		{
			int s = j;
			int e = (j + 1) % 6;
			QVector3D pt11 = pts1[s];
			QVector3D pt12 = pts1[e];
			QVector3D pt21 = pts2[s];
			QVector3D pt22 = pts2[e];

			float t1, t2;
			t1 = 0;
			switch (j)
			{
			case 0:
			case 2:
			case 4:
				t2 = fabs(pt11.z() - pt12.z()) / len;
				break;
			case 1:
			case 3:
			case 5:
				t2 = fabs(pt11.x() - pt12.x()) / len;
				break;
			}

			wallpts.push_back(pt11);
			walltexts.push_back(QVector2D(s1, t1));
			wallpts.push_back(pt21);
			walltexts.push_back(QVector2D(s2, t1));
			wallpts.push_back(pt22);
			walltexts.push_back(QVector2D(s2, t2));

			wallpts.push_back(pt22);
			walltexts.push_back(QVector2D(s2, t2));
			wallpts.push_back(pt12);
			walltexts.push_back(QVector2D(s1, t2));
			wallpts.push_back(pt11);
			walltexts.push_back(QVector2D(s1, t1));
		}

		//////
		pts1.clear();
		for (int j = 0; j < 6; j++)pts1.push_back(pts2[j]);
		pts2.clear();

		//底座
		basepts2.clear();
		for (int j = 0; j < 7; j++)
		{
			pt = wallbasePts[j];
			QVector4D pt4 = QVector4D(pt, 1);
			QVector4D newpt4 = mat * pt4;
			QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
			basepts2.push_back(newpt);
		}

		//生成三角形
		for (int j = 0; j < 5; j++)
		{
			int s = j;
			int e = j + 1;
			QVector3D pt11 = basepts1[s];
			QVector3D pt12 = basepts1[e];
			QVector3D pt21 = basepts2[s];
			QVector3D pt22 = basepts2[e];

			float t1, t2;
			switch (j)
			{
			case 0:
				t2 = 0;
				t1 = fabs(pt12.x() - pt11.x()) / len;
				break;
			case 1:
				t1 = 0;
				t2 = fabs(pt12.z() - basepts1[1].z()) / len;
				break;
			case 2:
				t1 = 0;
				t2 = fabs(pt12.x() - basepts1[1].x()) / len;
				break;
			case 3:
				t1 = 0;
				t2 = fabs(pt11.z() - basepts1[1].z()) / len;
				break;
			case 4:
				t1 = fabs(pt12.x() - pt12.x()) / len;
				t2 = fabs(pt11.x() - pt12.x()) / len;
				break;
			}

			basepts.push_back(pt11);
			basetexts.push_back(QVector2D(s1, t1));
			basepts.push_back(pt21);
			basetexts.push_back(QVector2D(s2, t1));
			basepts.push_back(pt22);
			basetexts.push_back(QVector2D(s2, t2));

			basepts.push_back(pt22);
			basetexts.push_back(QVector2D(s2, t2));
			basepts.push_back(pt12);
			basetexts.push_back(QVector2D(s1, t2));
			basepts.push_back(pt11);
			basetexts.push_back(QVector2D(s1, t1));
		}
		basepts1.clear();
		for (int j = 0; j < 7; j++)basepts1.push_back(basepts2[j]);
		basepts2.clear();

		s1 = s2;
	}

	//中间
	s1 = 0;
	float s2 = m_conicalSlopeData.middlewidth / len;
	//围墙
	QVector<QVector3D> tempfirstpts;
	for (int i = 0; i < 6; i++)
	{
		pt = pts1[i];
		tempfirstpts.push_back(pt);
	}

	QVector<QVector3D> tempfirstpts2;
	for (int i = 0; i < 6; i++)
	{
		pt = tempfirstpts[i];
		QVector3D newpt = QVector3D(pt.x()- m_conicalSlopeData.middlewidth, pt.y(), pt.z());
		tempfirstpts2.push_back(newpt);
	}

	for (int j = 0; j < 6; j++)
	{
		int s = j;
		int e = (j + 1) % 6;
		QVector3D pt11 = tempfirstpts[s];
		QVector3D pt12 = tempfirstpts[e];
		QVector3D pt21 = tempfirstpts2[s];
		QVector3D pt22 = tempfirstpts2[e];

		float t1, t2;
		t1 = 0;
		switch (j)
		{
		case 0:
		case 2:
		case 4:
			t2 = fabs(pt11.z() - pt12.z()) / len;
			break;
		case 1:
		case 3:
		case 5:
			t2 = fabs(pt11.y() - pt12.y()) / len;
			break;
		}

		wallpts.push_back(pt11);
		walltexts.push_back(QVector2D(s1, t1));
		wallpts.push_back(pt21);
		walltexts.push_back(QVector2D(s2, t1));
		wallpts.push_back(pt22);
		walltexts.push_back(QVector2D(s2, t2));

		wallpts.push_back(pt22);
		walltexts.push_back(QVector2D(s2, t2));
		wallpts.push_back(pt12);
		walltexts.push_back(QVector2D(s1, t2));
		wallpts.push_back(pt11);
		walltexts.push_back(QVector2D(s1, t1));
	}
	//围墙底座
	QVector<QVector3D> tempfirstbasepts;
	for (int i = 0; i < 7; i++)
	{
		pt = basepts1[i];
		tempfirstbasepts.push_back(pt);
	}
	QVector<QVector3D> tempfirstbasepts2;
	for (int i = 0; i < 7; i++)
	{
		pt = tempfirstbasepts[i];
		QVector3D newpt = QVector3D(pt.x()-m_conicalSlopeData.middlewidth, pt.y(), pt.z());
		tempfirstbasepts2.push_back(newpt);
	}
	//生成三角形
	for (int j = 0; j < 5; j++)
	{
		int s = j;
		int e = j + 1;
		QVector3D pt11 = tempfirstbasepts[s];
		QVector3D pt12 = tempfirstbasepts[e];
		QVector3D pt21 = tempfirstbasepts2[s];
		QVector3D pt22 = tempfirstbasepts2[e];

		float t1, t2;
		float dis;
		switch (j)
		{
		case 0:
			t1 = 0;
			dis = pt12.distanceToPoint(pt11);
			t2 = dis / len;// fabs(pt12.y() - pt11.y()) / len;
			break;
		case 1:
			t1 = 0;
			t2 = fabs(pt12.z() - basepts1[1].z()) / len;
			break;
		case 2:
			t1 = 0;
			t2 = fabs(pt12.x() - basepts1[1].x()) / len;
			break;
		case 3:
			t1 = 0;
			t2 = fabs(pt11.z() - basepts1[1].z()) / len;
			break;
		case 4:
			t1 = 0;// fabs(pt12.x() - pt12.x()) / len;
			dis = pt12.distanceToPoint(pt11);
			t2 = dis / len;// fabs(pt11.x() - pt12.x()) / len;
			break;
		}

		basepts.push_back(pt11);
		basetexts.push_back(QVector2D(s1, t1));
		basepts.push_back(pt21);
		basetexts.push_back(QVector2D(s2, t1));
		basepts.push_back(pt22);
		basetexts.push_back(QVector2D(s2, t2));

		basepts.push_back(pt22);
		basetexts.push_back(QVector2D(s2, t2));
		basepts.push_back(pt12);
		basetexts.push_back(QVector2D(s1, t2));
		basepts.push_back(pt11);
		basetexts.push_back(QVector2D(s1, t1));
	}

	//左侧
	QVector<QVector3D> leftpts1;
	for (int i = 0; i < tempfirstpts2.size(); i++)leftpts1.push_back(tempfirstpts2[i]);
	QVector<QVector3D> leftbasepts1;
	for (int i = 0; i < tempfirstbasepts2.size(); i++)leftbasepts1.push_back(tempfirstbasepts2[i]);

	float middlewidth = m_conicalSlopeData.middlewidth;
	QVector<QVector3D> leftcircle;
	for (int i = 0; i < (*pBottomCircle).size(); i++)
	{
		leftcircle.push_back((*pBottomCircle)[m - i]);
		leftcircle[i].setX(-leftcircle[i].x() - middlewidth);
	}

	dalpha = 90.0f / m;
	alpha = 90;
	if (!m_conicalSlopeData.bHead)
	{
		dalpha = -90.0f / m;
		alpha = -90;
	}
	lens = 0;
	QVector<QVector3D> leftpts2;
	QVector<QVector3D> leftbasepts2;
	s1 = 0;
	for (int i = 1; i <= m; i++)
	{
		alpha += dalpha;

		QVector3D pt = leftcircle[i];
		float templen = pt.distanceToPoint(leftcircle[i - 1]);
		lens += templen;

		QMatrix4x4 rotateMatrix;
		rotateMatrix.rotate(alpha, zaxes);
		QMatrix4x4 moveMatrix;
		moveMatrix.translate(pt);

		QMatrix4x4 mat = moveMatrix * rotateMatrix;

		float s2 = lens / len;
		leftpts2.clear();
		for (int j = 0; j < 6; j++)
		{
			pt = wallPts[j];
			QVector4D pt4 = QVector4D(pt, 1);
			QVector4D newpt4 = mat * pt4;
			QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
			leftpts2.push_back(newpt);
		}
		//生成三角形
		for (int j = 0; j < 6; j++)
		{
			int s = j;
			int e = (j + 1) % 6;
			QVector3D pt11 = leftpts1[s];
			QVector3D pt12 = leftpts1[e];
			QVector3D pt21 = leftpts2[s];
			QVector3D pt22 = leftpts2[e];

			float t1, t2;
			t1 = 0;
			float dis;
			switch (j)
			{
			case 0:
			case 2:
			case 4:
				t2 = fabs(pt11.z() - pt12.z()) / len;
				break;
			case 1:
			case 3:
			case 5:
				t2 = fabs(pt11.y() - pt12.y()) / len;
				break;
			}

			wallpts.push_back(pt11);
			walltexts.push_back(QVector2D(s1, t1));
			wallpts.push_back(pt21);
			walltexts.push_back(QVector2D(s2, t1));
			wallpts.push_back(pt22);
			walltexts.push_back(QVector2D(s2, t2));

			wallpts.push_back(pt22);
			walltexts.push_back(QVector2D(s2, t2));
			wallpts.push_back(pt12);
			walltexts.push_back(QVector2D(s1, t2));
			wallpts.push_back(pt11);
			walltexts.push_back(QVector2D(s1, t1));
		}

		//////
		leftpts1.clear();
		for (int j = 0; j < 6; j++)leftpts1.push_back(leftpts2[j]);
		leftpts2.clear();

		//底座
		leftbasepts2.clear();
		for (int j = 0; j < 7; j++)
		{
			pt = wallbasePts[j];
			QVector4D pt4 = QVector4D(pt, 1);
			QVector4D newpt4 = mat * pt4;
			QVector3D newpt = QVector3D(newpt4.x(), newpt4.y(), newpt4.z());
			leftbasepts2.push_back(newpt);
		}
		//生成三角形
		for (int j = 0; j < 5; j++)
		{
			int s = j;
			int e = j + 1;
			QVector3D pt11 = leftbasepts1[s];
			QVector3D pt12 = leftbasepts1[e];
			QVector3D pt21 = leftbasepts2[s];
			QVector3D pt22 = leftbasepts2[e];

			float t1, t2;
			float dis;
			switch (j)
			{
			case 0:
				t2 = 0;
				t1 = fabs(pt12.x() - pt11.x()) / len;
				break;
			case 1:
				t1 = 0;
				t2 = fabs(pt12.z() - basepts1[1].z()) / len;
				break;
			case 2:
				t1 = 0;
				t2 = fabs(pt12.x() - basepts1[1].x()) / len;
				break;
			case 3:
				t1 = 0;
				t2 = fabs(pt11.z() - basepts1[1].z()) / len;
				break;
			case 4:
				/*t1 = fabs(pt12.x() - pt12.x()) / len;
				t2 = fabs(pt11.x() - pt12.x()) / len;*/
				t1 = 0;// fabs(pt12.x() - pt12.x()) / len;
				dis = pt12.distanceToPoint(pt11);
				t2 = dis / len;// fabs(pt11.x() - pt12.x()) / len;
				break;
			}

			basepts.push_back(pt11);
			basetexts.push_back(QVector2D(s1, t1));
			basepts.push_back(pt21);
			basetexts.push_back(QVector2D(s2, t1));
			basepts.push_back(pt22);
			basetexts.push_back(QVector2D(s2, t2));

			basepts.push_back(pt22);
			basetexts.push_back(QVector2D(s2, t2));
			basepts.push_back(pt12);
			basetexts.push_back(QVector2D(s1, t2));
			basepts.push_back(pt11);
			basetexts.push_back(QVector2D(s1, t1));
		}
		leftbasepts1.clear();
		for (int j = 0; j < 7; j++)leftbasepts1.push_back(leftbasepts2[j]);
		leftbasepts2.clear();
		/// ////

		s1 = s2;
	}
	////两头侧面	
	////围墙右侧面
	////045
	//wallpts.push_back(pts1[0]);
	//walltexts.push_back(walltexture[0]);
	//wallpts.push_back(pts1[4]);
	//walltexts.push_back(walltexture[4]);
	//wallpts.push_back(pts1[5]);
	//walltexts.push_back(walltexture[5]);
	////014
	//wallpts.push_back(pts1[0]);
	//walltexts.push_back(walltexture[0]);
	//wallpts.push_back(pts1[1]);
	//walltexts.push_back(walltexture[1]);
	//wallpts.push_back(pts1[4]);
	//walltexts.push_back(walltexture[4]);
	////124
	//wallpts.push_back(pts1[1]);
	//walltexts.push_back(walltexture[1]);
	//wallpts.push_back(pts1[2]);
	//walltexts.push_back(walltexture[2]);
	//wallpts.push_back(pts1[4]);
	//walltexts.push_back(walltexture[4]);
	////234
	//for (int i = 2; i <= 4; i++)
	//{
	//	wallpts.push_back(pts1[i]);
	//	walltexts.push_back(walltexture[i]);
	//}
	////围墙左侧面
	////045
	//wallpts.push_back(leftpts1[0]);
	//walltexts.push_back(walltexture[0]);
	//wallpts.push_back(leftpts1[4]);
	//walltexts.push_back(walltexture[4]);
	//wallpts.push_back(leftpts1[5]);
	//walltexts.push_back(walltexture[5]);
	////014
	//wallpts.push_back(leftpts1[0]);
	//walltexts.push_back(walltexture[0]);
	//wallpts.push_back(leftpts1[1]);
	//walltexts.push_back(walltexture[1]);
	//wallpts.push_back(leftpts1[4]);
	//walltexts.push_back(walltexture[4]);
	////124
	//wallpts.push_back(leftpts1[1]);
	//walltexts.push_back(walltexture[1]);
	//wallpts.push_back(leftpts1[2]);
	//walltexts.push_back(walltexture[2]);
	//wallpts.push_back(leftpts1[4]);
	//walltexts.push_back(walltexture[4]);
	////234
	//for (int i = 2; i <= 4; i++)
	//{
	//	wallpts.push_back(leftpts1[i]);
	//	walltexts.push_back(walltexture[i]);
	//}

	////围墙底座生成三角形
	//// 右
	////016
	//basepts.push_back(basepts1[0]);
	//basetexts.push_back(wallbasetexture[0]);
	//basepts.push_back(basepts1[1]);
	//basetexts.push_back(wallbasetexture[1]);
	//basepts.push_back(basepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////126
	//basepts.push_back(basepts1[1]);
	//basetexts.push_back(wallbasetexture[1]);
	//basepts.push_back(basepts1[2]);
	//basetexts.push_back(wallbasetexture[2]);
	//basepts.push_back(basepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////236
	//basepts.push_back(basepts1[2]);
	//basetexts.push_back(wallbasetexture[2]);
	//basepts.push_back(basepts1[3]);
	//basetexts.push_back(wallbasetexture[3]);
	//basepts.push_back(basepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////356
	//basepts.push_back(basepts1[3]);
	//basetexts.push_back(wallbasetexture[3]);
	//basepts.push_back(basepts1[5]);
	//basetexts.push_back(wallbasetexture[5]);
	//basepts.push_back(basepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////345
	//for (int i = 3; i <= 5; i++)
	//{
	//	basepts.push_back(basepts1[i]);
	//	basetexts.push_back(wallbasetexture[i]);
	//}

	//// 左
	////016
	//basepts.push_back(leftbasepts1[0]);
	//basetexts.push_back(wallbasetexture[0]);
	//basepts.push_back(leftbasepts1[1]);
	//basetexts.push_back(wallbasetexture[1]);
	//basepts.push_back(leftbasepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////126
	//basepts.push_back(leftbasepts1[1]);
	//basetexts.push_back(wallbasetexture[1]);
	//basepts.push_back(leftbasepts1[2]);
	//basetexts.push_back(wallbasetexture[2]);
	//basepts.push_back(leftbasepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////236
	//basepts.push_back(leftbasepts1[2]);
	//basetexts.push_back(wallbasetexture[2]);
	//basepts.push_back(leftbasepts1[3]);
	//basetexts.push_back(wallbasetexture[3]);
	//basepts.push_back(leftbasepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////356
	//basepts.push_back(leftbasepts1[3]);
	//basetexts.push_back(wallbasetexture[3]);
	//basepts.push_back(leftbasepts1[5]);
	//basetexts.push_back(wallbasetexture[5]);
	//basepts.push_back(leftbasepts1[6]);
	//basetexts.push_back(wallbasetexture[6]);
	////345
	//for (int i = 3; i <= 5; i++)
	//{
	//	basepts.push_back(leftbasepts1[i]);
	//	basetexts.push_back(wallbasetexture[i]);
	//}
}

void rdBridgeConicalSlope::initGeometry()
{
	float h0 = m_conicalSlopeData.h0;
	float h = m_conicalSlopeData.h + h0;
	float scale = m_conicalSlopeData.scale;
	float platwidth = m_conicalSlopeData.platwidth;
	//h>3m,梁下1.5米处设置1.2m宽维修平台
	
	int m = 20;
	float dalpha = 0.5 * 3.14159/m;
	if (!m_conicalSlopeData.bHead) dalpha = -0.5 * 3.14159 / m;

	QVector<QVector3D> circle1;
	QVector<QVector3D> circle2;
	QVector<QVector3D> circle3;

	int size = 0;
	QVector<QVector3D> slopePts;
	QVector<QVector2D> slopeTextures;
	QVector<QVector3D> platformPts;
	QVector<QVector2D> platformTextures;

	float len = 4;
	float l = 0;
	
	if (m_conicalSlopeData.h <= 3)
	{
		float z = - h;
		float r = h * scale;
		float alpha = 0;
		QVector3D pt;
		pt.setZ(z);
		for (int i = 0; i <= m; i++)
		{			
			pt.setX(r * cos(alpha));
			pt.setY(r * sin(alpha));
			circle1.push_back(pt);

			alpha += dalpha;
		}		
		
		initTopSlopeGeometry(m_conicalSlopeData.h,circle1, slopePts, slopeTextures);
	}
	else
	{
		float z1 = -1.5 - h0;
		float r1 = -z1 * scale;
		float z2 = -h;
		float r2 = r1 + platwidth;
		float r3 = h * scale + platwidth;
		float alpha = 0;
		QVector3D pt;
		
		for (int i = 0; i <= m; i++)
		{
			pt.setZ(z1);
			pt.setX(r1 * cos(alpha));
			pt.setY(r1 * sin(alpha));
			circle1.push_back(pt);

			pt.setX(r2 * cos(alpha));
			pt.setY(r2 * sin(alpha));
			circle2.push_back(pt);

			pt.setZ(z2);
			pt.setX(r3 * cos(alpha));
			pt.setY(r3 * sin(alpha));
			circle3.push_back(pt);

			alpha += dalpha;
		}

		//上锥坡
		initTopSlopeGeometry(1.5, circle1, slopePts, slopeTextures);		
		//平台
		initPlatformGeometry(circle1, circle2, platformPts, platformTextures);
		//下坡
		initBottomSlopeGeometry(circle2, circle3, slopePts, slopeTextures);	
	}

	float alpha = 0;
	QVector<QVector3D>* pData;
	pData = &circle1;
	if (m_conicalSlopeData.h > 3) pData = &circle3;
	
	QVector<QVector3D> wallpts;
	QVector<QVector2D> walltexts;
	QVector<QVector3D> basepts;
	QVector<QVector2D> basetexts;
	initWallGeometry(pData, wallpts, walltexts, basepts, basetexts);
	
	//生成VBO
	initializeOpenGLFunctions();	
	//锥坡
	size = slopePts.size();
	m_slopPoints = size;
	QVector3D* pVertices = new QVector3D[size];
	QVector2D* pTextures = new QVector2D[size];
	for (int i = 0; i < size; i++)
	{
		pVertices[i] = slopePts[i];
		pTextures[i] = slopeTextures[i];
	}
	bool re = m_coorSlopeBuf.create();
	re = m_coorSlopeBuf.bind();
	m_coorSlopeBuf.allocate(pVertices, size * sizeof(QVector3D));

	re = m_textSlopeBuf.create();
	m_textSlopeBuf.bind();
	m_textSlopeBuf.allocate(pTextures, size * sizeof(QVector2D));

	slopePts.clear();
	slopeTextures.clear();
	delete[] pVertices;
	delete[] pTextures;

	//平台
	m_platformPoints = platformPts.size();
	size = platformPts.size();
	if (size > 0)
	{
		QVector3D* pVertices = new QVector3D[size];
		QVector2D* pTextures = new QVector2D[size];
		for (int i = 0; i < size; i++)
		{
			pVertices[i] = platformPts[i];
			pTextures[i] = platformTextures[i];
		}
		bool re = m_coorPlatformBuf.create();
		re = m_coorPlatformBuf.bind();
		m_coorPlatformBuf.allocate(pVertices, size * sizeof(QVector3D));

		re = m_textPlatformBuf.create();
		m_textPlatformBuf.bind();
		m_textPlatformBuf.allocate(pTextures, size * sizeof(QVector2D));

		//释放空间
		platformPts.clear();
		platformTextures.clear();
		delete[] pVertices;
		delete[] pTextures;
	}	
	//围墙
	m_wallPoints = wallpts.size();
	size = m_wallPoints;
	if (size > 0)
	{
		QVector3D* pVertices = new QVector3D[size];
		QVector2D* pTextures = new QVector2D[size];
		for (int i = 0; i < size; i++)
		{
			pVertices[i] = wallpts[i];
			pTextures[i] = walltexts[i];
		}
		bool re = m_coorWallBuf.create();
		re = m_coorWallBuf.bind();
		m_coorWallBuf.allocate(pVertices, size * sizeof(QVector3D));

		re = m_textWallBuf.create();
		m_textWallBuf.bind();
		m_textWallBuf.allocate(pTextures, size * sizeof(QVector2D));

		//释放空间
		wallpts.clear();
		walltexts.clear();
		delete[] pVertices;
		delete[] pTextures;
	}

	//围墙底座
	m_basePoints = basepts.size();
	size = m_basePoints;
	if (size > 0)
	{
		QVector3D* pVertices = new QVector3D[size];
		QVector2D* pTextures = new QVector2D[size];
		for (int i = 0; i < size; i++)
		{
			pVertices[i] = basepts[i];
			pTextures[i] = basetexts[i];
		}
		bool re = m_coorBaseBuf.create();
		re = m_coorBaseBuf.bind();
		m_coorBaseBuf.allocate(pVertices, size * sizeof(QVector3D));

		re = m_textBaseBuf.create();
		m_textBaseBuf.bind();
		m_textBaseBuf.allocate(pTextures, size * sizeof(QVector2D));

		//释放空间
		basepts.clear();
		basetexts.clear();
		delete[] pVertices;
		delete[] pTextures;
	}

}

void rdBridgeConicalSlope::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	if (!m_pTextures) return;
	QOpenGLTexture* pStoneTexture = m_pTextures->getTextureByName("STONE");
	QOpenGLTexture* pCobbleStoneTexture = m_pTextures->getTextureByName("COBBLESTONE");

	if (!pStoneTexture || !pCobbleStoneTexture) return;

	//锥坡
	pStoneTexture->bind();
	program->setUniformValue("texture", 0);

	m_coorSlopeBuf.bind();
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

	m_textSlopeBuf.bind();
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
	glDrawArrays(GL_TRIANGLES, 0, m_slopPoints);

	//平台
	if (m_platformPoints > 0)
	{
		pCobbleStoneTexture->bind();
		program->setUniformValue("texture", 0);
		m_coorPlatformBuf.bind();
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

		m_textPlatformBuf.bind();
		program->enableAttributeArray(texcoordLocation);
		program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
		glDrawArrays(GL_TRIANGLES, 0, m_platformPoints);
	}

	//围墙
	if (m_wallPoints > 0)
	{
		pStoneTexture->bind();
		program->setUniformValue("texture", 0);
		m_coorWallBuf.bind();
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

		m_textWallBuf.bind();
		program->enableAttributeArray(texcoordLocation);
		program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
		glDrawArrays(GL_TRIANGLES, 0, m_wallPoints);
	}

	//围墙底座
	if (m_basePoints > 0)
	{
		pCobbleStoneTexture->bind();
		program->setUniformValue("texture", 0);
		m_coorBaseBuf.bind();
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

		m_textBaseBuf.bind();
		program->enableAttributeArray(texcoordLocation);
		program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
		glDrawArrays(GL_TRIANGLES, 0, m_basePoints);
	}
}

void rdBridgeConicalSlope::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
{

}