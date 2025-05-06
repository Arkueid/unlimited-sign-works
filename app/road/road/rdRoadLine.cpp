#include "rdRoadLine.h"
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <math.h>

rdRoadLine::rdRoadLine()
{
	initializeOpenGLFunctions();
	m_num = 14;
	_HtPos hp;

	hp.start = -920.f;	hp.end = 7900.f;	m_htpos[0] = hp;
	hp.start = 7900.f;	hp.end = 19490.f;	m_htpos[1] = hp;
	hp.start = 19490.f;	hp.end = 24320.f;	m_htpos[2] = hp;
	hp.start = 24320.f;	hp.end = 28608.f;	m_htpos[3] = hp;
	hp.start = 28608.f;	hp.end = 32800.f;	m_htpos[4] = hp;
	hp.start = 32800.f;	hp.end = 38249.f;	m_htpos[5] = hp;
	hp.start = 38249.f;	hp.end = 43410.f;	m_htpos[6] = hp;
	hp.start = 43410.f;	hp.end = 49300.f;	m_htpos[7] = hp;
	hp.start = 49300.f;	hp.end = 56500.f;	m_htpos[8] = hp;
	hp.start = 56500.f;	hp.end = 66500.f;	m_htpos[9] = hp;
	hp.start = 66500.f;	hp.end = 73469.f;	m_htpos[10] = hp;
	hp.start = 73469.f;	hp.end = 83400.f;	m_htpos[11] = hp;
	hp.start = 83400.f;	hp.end = 90350.f;	m_htpos[12] = hp;
	hp.start = 90350.f;	hp.end = 96543.329f;	m_htpos[13] = hp;
}

rdRoadLine::~rdRoadLine()
{
	vertsBuf.destroy();
	vertsLineBuf.destroy();
}

void rdRoadLine::releaseGL()
{
	vertsBuf.destroy();
}
//读主线数据
bool rdRoadLine::readData()
{
	//if (htNo < 1 || htNo>m_num) return false;
	
	QString filepath = QCoreApplication::applicationDirPath() + "/../../assets/data/Section";

	for (int i = 1; i <=1/* m_num*/; i++)
	{
		QString numstr = QString::number(i);
		if (i < 10) numstr = "0" + numstr;

		QString filename = filepath + numstr + "/mainline" + numstr + ".txt";
		QFile file(filename);
		if (!file.exists()) continue;

		bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
		if (ret)
		{
			QTextStream fin(&file);
			RoadLine[i-1].clear();
			_RoadCrossSection rcs;
			rcs.lefttype = rcs.righttype = 0;
			double x, y, z;

			QString str;
			int htno = i - 1;

			while (!fin.atEnd())
			{
				fin >> str;
				fin >> y;
				fin >> x;
				fin >> z;
				fin >> rcs.lW1;
				fin >> rcs.lW2;
				fin >> rcs.lW3;
				fin >> rcs.W0;
				fin >> rcs.rW3;
				fin >> rcs.rW2;
				fin >> rcs.rW1;

				rcs.center = QVector3D(x - StartX, y - StartY, z - StartZ);

				//计算pileNo
				rcs._pileKNo = str;
				int sindex = (int)str.indexOf('-');//0或-1
				int index = (int)str.indexOf('+');
				int kindex = (int)str.indexOf('K');

	            QString substr = str.mid(kindex + 1, index - kindex - 1);
	//	/*if (sindex == 0)
	//		substr = str.substr(2, index);*/

				int kk = substr.toInt();

				int len = (int)str.length();
				QString substr1 = str.mid(index + 1, len - index);
				double  s = substr1.toDouble();
				double f = kk * 1000 + s;
				rcs.pileNo = f;

				if (sindex == 0) rcs.pileNo = -f;

				RoadLine[htno].push_back(rcs);
			}
		
			file.close();
		}
	}	

	return true;
}

bool rdRoadLine::computing()
{
	for (int no = 0; no <1 /*m_num*/; no++)
	{
		if (RoadLine[no].size() < 2) continue;

		_RoadCrossSection rcs0, rcs1;
		rcs0 = RoadLine[no][0];
		double alpha, a, b, c;

		int s = 1;

		double minx, miny, maxx, maxy;
		minx = maxx = RoadLine[no][0].center.x();
		miny = maxy = RoadLine[no][0].center.y();

		for (int i = 1; i < RoadLine[no].size(); i++)
		{
			rcs1 = RoadLine[no][i];

			if (minx > rcs1.center.x()) minx = rcs1.center.x();
			if (miny > rcs1.center.y()) miny = rcs1.center.y();
			if (maxx < rcs1.center.x()) maxx = rcs1.center.x();
			if (maxy < rcs1.center.y()) maxy = rcs1.center.y();

			double dx = rcs1.center.x() - rcs0.center.x();
			double dy = rcs1.center.y() - rcs0.center.y();

			a = -dy;
			b = dx;
			c = rcs0.center.x() * rcs1.center.y() - rcs1.center.x() * rcs0.center.y();

			if (fabs(dx) < EPS)
			{
				alpha = 90;
				if (dy < 0) alpha = 270;
			}
			else
			{
				if (dx > 0 && dy > 0)
				{
					if (s != 1)
						s = s;
					s = 1;
				}
				else if (dx < 0 && dy > 0)
				{
					if (s != 2)
						s = s;
					s = 2;
				}
				else if (dx < 0 && dy < 0)
				{
					if (s != 3)
						s = s;
					s = 3;
				}
				else if (dx > 0 && dy < 0)
				{
					if (s != 4)
						s = s;
					s = 4;
				}

				alpha = atan(dy / dx) * 180.0 / PI;

				if (dx < 0 && dy < 0)
				{
					alpha += 180;
				}
				else if (dx < 0 && dy > 0)
				{
					alpha += 180;
				}
				else if (dx > 0 && dy < 0)
				{
					alpha += 360;
				}
			}

			RoadLine[no][i].a = a;
			RoadLine[no][i].b = b;
			RoadLine[no][i].c = c;
			RoadLine[no][i].alpha = alpha;

			if (a == 0)
			{
				RoadLine[no][i].a1 = 1;
				RoadLine[no][i].b1 = 0;
				RoadLine[no][i].c1 = -rcs1.center.x();
			}
			else if (b == 0)
			{
				RoadLine[no][i].a1 = 0;
				RoadLine[no][i].b1 = 1;
				RoadLine[no][i].c1 = -rcs1.center.y();
			}
			else
			{
				RoadLine[no][i].a1 = b;
				RoadLine[no][i].b1 = -a;
				RoadLine[no][i].c1 = a * rcs1.center.y() - b * rcs1.center.x();
			}

			rcs0 = rcs1;
		}

		RoadLine[no][0].a = RoadLine[no][1].a;
		RoadLine[no][0].b = RoadLine[no][1].b;
		RoadLine[no][0].c = RoadLine[no][1].c;
		RoadLine[no][0].alpha = RoadLine[no][1].alpha;

		RoadLine[no][0].a1 = RoadLine[no][1].a1;
		RoadLine[no][0].b1 = RoadLine[no][1].b1;
		RoadLine[no][0].c1 = -(RoadLine[no][0].center.x()* RoadLine[no][0].a1 +
			RoadLine[no][0].center.y() * RoadLine[no][0].b1);

		//计算路面左右侧顶点
		float lastz = 0;
		for (int i = 0; i < RoadLine[no].size()-1; i++)
		{			
			_RoadCrossSection& rcs0 = RoadLine[no][i];
			_RoadCrossSection& rcs1 = RoadLine[no][i+1];
			float llength = rcs0.W0 / 2 + rcs0.lW1 + rcs0.lW2 + rcs0.lW3;
			float rlength = rcs0.W0 / 2 + rcs0.rW1 + rcs0.rW2 + rcs0.rW3;

			//float ds = rcs0.center.x() * rcs0.a1 + rcs0.center.y() * rcs.b1 + rcs.c1;
			float x0 = rcs0.center.x();
			float y0 = rcs0.center.y();
			float x1 = rcs1.center.x();
			float y1 = rcs1.center.y();

			QVector3D p0p1 = QVector3D(x1 - x0, y1 - y0, 0);
			if (rcs0.a1 == 0 || rcs0.b1 == 0)
			{
				int a = 0;
			}
			else if (fabs(rcs0.a1) > 0)
			{
				float y = rcs0.center.y() + 1;
				float x = -(y * rcs0.b1 + rcs0.c1) / rcs0.a1;
				float ds0 = sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
				
				float lt = llength / ds0;					
				float lx = x0 + (x - x0) * lt;
				float ly = y0 + (y - y0) * lt;

				float rt = - rlength / ds0;
				float rx = x0 + (x - x0) * rt;
				float ry = y0 + (y - y0) * rt;
				//float dslr = sqrt((rx - lx) * (rx - lx) + (ry - ly) * (ry - ly));	
				QVector3D vl = QVector3D(lx - x0, ly - y0, 0);
				QVector3D vr = QVector3D(rx - x0, ry - y0, 0);
				QVector3D clv = QVector3D::crossProduct(p0p1, vl);
				QVector3D crv = QVector3D::crossProduct(p0p1, vr);
				/*QVector3D ll = QVector3D(1, 0, 0);
				QVector3D llvl = QVector3D(0, 1, 0);
				QVector3D llvr = QVector3D(0, -1, 0);
				QVector3D llclv = QVector3D::crossProduct(ll, llvl);
				QVector3D llcrv = QVector3D::crossProduct(ll, llvr);*/
				lastz = clv.z();
				if (clv.z() > 0)//左侧
				{
					rcs0.left = QVector3D(lx, ly, rcs0.center.z());
					rcs0.right = QVector3D(rx, ry, rcs0.center.z());
				}
				else//右侧
				{
					rcs0.right = QVector3D(lx, ly, rcs0.center.z());
					rcs0.left  = QVector3D(rx, ry, rcs0.center.z());
				}	
			}
			else if (fabs(rcs0.b1) > 0)
			{
				int a = 0;
			}
		}

		//最后一个点
		int ii = RoadLine[no].size() - 1;
		_RoadCrossSection& rcs = RoadLine[no][ii];
		float llength = rcs.W0 / 2 + rcs.lW1 + rcs.lW2 + rcs.lW3;
		float rlength = rcs.W0 / 2 + rcs.rW1 + rcs.rW2 + rcs.rW3;
		float x0 = rcs.center.x();
		float y0 = rcs.center.y();
		if (rcs.a1 == 0 || rcs.b1 == 0)
		{

		}
		else if (fabs(rcs.a1) > 0)
		{
			float y = rcs.center.y() + 1;
			float x = -(y * rcs.b1 + rcs.c1) / rcs.a1;
			float ds0 = sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
		
			float lt = llength / ds0;
			float lx = x0 + (x - x0) * lt;
			float ly = y0 + (y - y0) * lt;

			float rt = -rlength / ds0;
			float rx = x0 + (x - x0) * rt;
			float ry = y0 + (y - y0) * rt;
			if (lastz > 0)//左侧
			{
				rcs.left = QVector3D(lx, ly, rcs.center.z());
				rcs.right = QVector3D(rx, ry, rcs.center.z());
			}
			else//右侧
			{
				rcs.right = QVector3D(lx, ly, rcs.center.z());
				rcs.left = QVector3D(rx, ry, rcs.center.z());
			}
		}
		else if (fabs(rcs.b1) > 0)
		{

		}
		//
		//初始应该用上一合同段的结尾
		

		if (no > 0)
		{
			int no1 = no - 1;
			int size1 = RoadLine[no1].size() - 1;

			RoadLine[no][0].a = RoadLine[no1][size1].a;
			RoadLine[no][0].b = RoadLine[no1][size1].b;
			RoadLine[no][0].c = RoadLine[no1][size1].c;
			RoadLine[no][0].alpha = RoadLine[no1][size1].alpha;
		}

		if (RoadLine[no][0].a == 0)
		{
			RoadLine[no][0].a1 = 1;
			RoadLine[no][0].b1 = 0;
			RoadLine[no][0].c1 = -RoadLine[no][0].center.x();
		}
		else if (b == 0)
		{
			RoadLine[no][0].a1 = 0;
			RoadLine[no][0].b1 = 1;
			RoadLine[no][0].c1 = -RoadLine[no][0].center.y();
		}
		else
		{
			RoadLine[no][0].a1 = RoadLine[no][0].b;
			RoadLine[no][0].b1 = -RoadLine[no][0].a;
			RoadLine[no][0].c1 = RoadLine[no][0].a * RoadLine[no][0].center.y() - RoadLine[no][0].b * RoadLine[no][0].center.x();
		}

		m_htpos[no].minx = minx;
		m_htpos[no].maxx = maxx;
		m_htpos[no].miny = miny;
		m_htpos[no].maxy = maxy;		
	}
	
	return true;
}

_RoadCrossSection rdRoadLine::getRCS(int index, int htNo)
{
	_RoadCrossSection cs;
	cs.pileNo = -9999;

	if (index < 0 && index >= RoadLine[htNo - 1].size()) return cs;

	return RoadLine[htNo - 1][index];
}

QVector2D rdRoadLine::getPeg(float x, float y)//返回值x是桩号，y是合同段
{
	QVector2D re(-2000, -1);

	bool find = false;
	/*double s0 = RoadLine[3][407].a1*x + RoadLine[3][407].b1*y + RoadLine[3][407].c1;
	double s1 = RoadLine[3][408].a1*x + RoadLine[3][408].b1*y + RoadLine[3][408].c1;*/
	for (int i = 0; i < 1/*m_num*/; i++)
	{
		//		if (x >= m_htpos[i].minx-50 && x <= m_htpos[i].maxx+50)
		{
			int size = (int)RoadLine[i].size();

			double s0 = RoadLine[i][0].a1 * x + RoadLine[i][0].b1 * y + RoadLine[i][0].c1;
			for (int j = 1; j < size; j++)
			{
				double s1 = RoadLine[i][j].a1 * x + RoadLine[i][j].b1 * y + RoadLine[i][j].c1;

				if (s1 * s0 <= 0)
				{
					//在i-1和i间计算桩号
					double peg1 = RoadLine[i][j - 1].pileNo;
					double peg2 = RoadLine[i][j].pileNo;

					//double a = RoadLine[i][j].a;
					//double b = RoadLine[i][j].b;
					//double c = RoadLine[i][j].c;

					double a00 = RoadLine[i][j].a1;//j-1
					double b00 = RoadLine[i][j].b1;
					double c00 = RoadLine[i][j].c1;

					c00 = -(a00 * x + b00 * y);

					float a11 = RoadLine[i][j - 1].center.y() - RoadLine[i][j].center.y();
					float b11 = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
					float c11 = RoadLine[i][j - 1].center.x() * RoadLine[i][j].center.y() - RoadLine[i][j].center.x() * RoadLine[i][j - 1].center.y();

					double m = a00 * b11 - a11 * b00;
					if (fabs(m) > 0.001)//m != 0)
					{
						double x = (c11 * b00 - c00 * b11) / m;
						double y = (c00 * a11 - c11 * a00) / m;

						double dx, t, peg;
						dx = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
						if (fabs(dx) < 0.01)
						{
							dx = RoadLine[i][j].center.y() - RoadLine[i][j - 1].center.y();
							t = (y - RoadLine[i][j - 1].center.y()) / dx;
						}
						else
						{
							t = (x - RoadLine[i][j - 1].center.x()) / dx;
						}

						peg = peg1 + (peg2 - peg1) * t;

						re = QVector2D(peg, i + 1);

						find = true;
						break;
					}
				}

				s0 = s1;
			}

			if (find) break;
		}
	}

	//是否最后一段
	//if (!find)
	//{
	//	if (x < m_htpos[13].minx) //
	//	{
	//		re = QVector2D(97000, 14);
	//	}
	//	else if (x > m_htpos[0].maxx)
	//	{
	//		re = QVector2D(-999, 1);
	//	}
	//}

	return re;
}

QVector3D rdRoadLine::getNearestPeg(float x, float y)
{
	QVector3D re(-1, -1, -1);

	bool find = false;
	for (int i = 0; i < m_num; i++)
	{
		if (x >= m_htpos[i].minx - 50 && x <= m_htpos[i].maxx + 50)//偏差
		{
			int size = (int)RoadLine[i].size();

			double s0 = RoadLine[i][0].a1 * x + RoadLine[i][0].b1 * y + RoadLine[i][0].c1;
			for (int j = 1; j < size; j++)
			{
				double s1 = RoadLine[i][j].a1 * x + RoadLine[i][j].b1 * y + RoadLine[i][j].c1;

				if (s1 * s0 <= 0)
				{
					//在i-1和i间计算桩号
					double peg1 = RoadLine[i][j - 1].pileNo;
					double peg2 = RoadLine[i][j].pileNo;

					/*double a = RoadLine[i][j].a;
					double b = RoadLine[i][j].b;
					double c = RoadLine[i][j].c;*/

					double a00 = RoadLine[i][j - 1].a1;
					double b00 = RoadLine[i][j - 1].b1;
					double c00 = RoadLine[i][j - 1].c1;

					c00 = -(a00 * x + b00 * y);

					float a11 = RoadLine[i][j - 1].center.y() - RoadLine[i][j].center.y();
					float b11 = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
					float c11 = RoadLine[i][j - 1].center.x() * RoadLine[i][j].center.y() - RoadLine[i][j].center.x() * RoadLine[i][j - 1].center.y();

					double m = a00 * b11 - a11 * b00;
					if (fabs(m) > 0.001)//m != 0)
					{
						double xx = (c11 * b00 - c00 * b11) / m;
						double yy = (c00 * a11 - c11 * a00) / m;

						double dx, t, peg;
						dx = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
						if (fabs(dx) < 0.01)
						{
							dx = RoadLine[i][j].center.y() - RoadLine[i][j - 1].center.y();
							t = (yy - RoadLine[i][j - 1].center.y()) / dx;
						}
						else
						{
							t = (xx - RoadLine[i][j - 1].center.x()) / dx;
						}

						peg = peg1 + (peg2 - peg1) * t;
						float dis = sqrt((x - xx) * (x - xx) + (y - yy) * (y - yy));

						re = QVector3D(peg, i + 1, dis);

						find = true;
						break;
					}
				}

				s0 = s1;
			}

			if (find) break;
		}
	}

	return re;
}

//返回：桩号，合同段序号（开始1）、到路线的垂直距离及左右侧（1左，2右）
QVector4D rdRoadLine::getMainLinePeg(float x, float y)
{
	QVector4D re(-1, -1, -1, 0);

	bool find = false;
	for (int i = 0; i < m_num; i++)
	{
		if (x >= m_htpos[i].minx - 50 && x <= m_htpos[i].maxx + 50)//偏差
		{
			int size = (int)RoadLine[i].size();

			double s0 = RoadLine[i][0].a1 * x + RoadLine[i][0].b1 * y + RoadLine[i][0].c1;
			for (int j = 1; j < size; j++)
			{
				double s1 = RoadLine[i][j].a1 * x + RoadLine[i][j].b1 * y + RoadLine[i][j].c1;

				if (s1 * s0 <= 0)
				{
					//在i-1和i间计算桩号
					double peg1 = RoadLine[i][j - 1].pileNo;
					double peg2 = RoadLine[i][j].pileNo;

					//double a = RoadLine[i][j].a;
					//double b = RoadLine[i][j].b;
					//double c = RoadLine[i][j].c;

					double a00 = RoadLine[i][j].a1;
					double b00 = RoadLine[i][j].b1;
					double c00 = RoadLine[i][j].c1;

					c00 = -(a00 * x + b00 * y);

					float a11 = RoadLine[i][j - 1].center.y() - RoadLine[i][j].center.y();
					float b11 = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
					float c11 = RoadLine[i][j - 1].center.x() * RoadLine[i][j].center.y() - RoadLine[i][j].center.x() * RoadLine[i][j - 1].center.y();

					double m = a00 * b11 - a11 * b00;
					if (fabs(m) > 0.001)//m != 0)
					{
						double xx = (c11 * b00 - c00 * b11) / m;
						double yy = (c00 * a11 - c11 * a00) / m;

						double dx, t, peg;
						dx = RoadLine[i][j].center.x() - RoadLine[i][j - 1].center.x();
						if (fabs(dx) < 0.01)
						{
							dx = RoadLine[i][j].center.y() - RoadLine[i][j - 1].center.y();
							t = (yy - RoadLine[i][j - 1].center.y()) / dx;
						}
						else
						{
							t = (xx - RoadLine[i][j - 1].center.x()) / dx;
						}

						peg = peg1 + (peg2 - peg1) * t;
						float dis = sqrt((x - xx) * (x - xx) + (y - yy) * (y - yy));
						//	float dis1 = fabs(a*x + b*y + c) / (sqrt(a*a + b*b));

						QVector3D p1(x, y, 0);
						QVector3D p0(RoadLine[i][j - 1].center.x(), RoadLine[i][j - 1].center.y(), 0);
						QVector3D p2(RoadLine[i][j].center.x(), RoadLine[i][j].center.y(), 0);
						QVector3D p01 = p1 - p0;
						QVector3D p02 = p2 - p0;
						QVector3D p12 = QVector3D::crossProduct(p01,p02);// glm::cross(p1 - p0, p2 - p0);
						int ss = 2;
						if (p12.z() < 0) ss = 1;

						re = QVector4D(peg, i + 1, dis, ss);

						find = true;
						break;
					}
				}

				s0 = s1;
			}

			if (find) break;
		}
	}

	if (!find)
	{
		if (x < m_htpos[13].minx) //
		{
			re.setY(14);
			re.setX(97000);
			re.setZ(0);
		}
		if (x > m_htpos[0].maxx)
		{
			re.setY(1);
			re.setX(-999);
			re.setZ(0);
		}
	}

	return re;
}

QVector4D rdRoadLine::getLeftLinePeg(float x, float y)
{
	QVector4D re(-1, -1, -1, 0);

	return re;
}

int rdRoadLine::getHT(float peg)
{
	for (int i = 0; i < m_num; i++)
	{
		if (peg >= m_htpos[i].start && peg <= m_htpos[i].end)
		{
			return i + 1;
		}
	}

	return -1;
}

struct _PosAng rdRoadLine::findPos(float peg)
{
	struct _PosAng pos;
	pos.angle = -99;

	int htno = 0;
	for (int i = 0; i < m_num; i++)
	{
		if (peg >= m_htpos[i].start && peg <= m_htpos[i].end)
		{
			htno = i;
			break;
		}
	}

	for (int i = 0; i < RoadLine[htno].size() - 1; i++)
	{
		if (peg >= RoadLine[htno][i].pileNo && peg <= RoadLine[htno][i + 1].pileNo)
		{

			pos.x = RoadLine[htno][i].center.x();
			pos.y = RoadLine[htno][i].center.y();
			pos.z = RoadLine[htno][i].center.z();

			pos.angle = 450 - RoadLine[htno][i].alpha;

			if (peg == RoadLine[htno][i + 1].pileNo)
			{
				pos.x = RoadLine[htno][i + 1].center.x();
				pos.y = RoadLine[htno][i + 1].center.y();
				pos.z = RoadLine[htno][i + 1].center.z();

				pos.angle = 450 - RoadLine[htno][i + 1].alpha;
			}

			pos.section = i;

			break;
		}
	}

	return pos;
}

void rdRoadLine::initGeometry()
{
	initializeOpenGLFunctions();
	vertsBuf.create();

	QVector<_RoadCrossSection>& roadline1= RoadLine[0];
	int size = roadline1.size();

	int allpts = (size - 1) * 2 * 4;
	QVector3D* pVerts = new QVector3D[allpts];
	m_allpts = allpts;
	long index = 0;
	for (int i = 0; i < size-1; i++)
	{
		//左
		pVerts[index++] = roadline1[i].center;
		pVerts[index++] = roadline1[i+1].center;
		pVerts[index++] = roadline1[i + 1].left;
		pVerts[index++] = roadline1[i].left;

		//右
		pVerts[index++] = roadline1[i].center;
		pVerts[index++] = roadline1[i + 1].center;
		pVerts[index++] = roadline1[i + 1].right;
		pVerts[index++] = roadline1[i].right;
	}

	vertsBuf.bind();
	vertsBuf.allocate(pVerts, allpts * sizeof(QVector3D));

	delete[] pVerts;

	//中心线
	allpts = size;
	pVerts = new QVector3D[allpts];
	m_linepts = allpts;
	index = 0;
	for (int i = 0; i < size; i++) pVerts[index++] = roadline1[i].center;
	
	vertsLineBuf.create();
	vertsLineBuf.bind();
	vertsLineBuf.allocate(pVerts, allpts * sizeof(QVector3D));
	delete[] pVerts;
}

void rdRoadLine::render(QOpenGLShaderProgram* program)// , QMatrix4x4& proj, QMatrix4x4& view)
{
	//program->bind();
	/*QMatrix4x4 modelM = view;
	program->setUniformValue("ProjectionM", proj);
	program->setUniformValue("ModelM", modelM);*/
	//program->setUniformValue("Color", QVector3D(0.2, 0.2, 0.2));

	//vertsBuf.bind();
	vertsLineBuf.bind();
	int vertexLocation = program->attributeLocation("VertexPos");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);
	//glDrawArrays(GL_QUADS, 0, m_allpts);
	glDrawArrays(GL_LINE_STRIP, 0, m_linepts);
}