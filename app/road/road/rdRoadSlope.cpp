#include "rdRoadSlope.h"

#include "rdRoadLine.h"
#include "rdRoadRamp.h"
#include "rdRoadDitch.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <math.h>

rdRoadSlope::rdRoadSlope()
{
	for (int i=0; i<14; i++)
		for (int j = 0; j < 8; j++)
		{
			m_quadFillCount[i][j] = 0;
			m_quadExcaveCount[i][j] = 0;
		}
}

rdRoadSlope::~rdRoadSlope()
{
	//vertsBuf.destroy()
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 8; j++)
		{
			if (m_quadFillCount[i][j] > 0)
			{
				for (int l = 0; l < 3; l++)  slopFillVertsBuf[i][j][l].destroy();
			}
			
			if (m_quadExcaveCount[i][j] > 0)
			{
				for (int l = 0; l < 3; l++)  slopExcaveVertsBuf[i][j][l].destroy();
			}
		}
}

void rdRoadSlope::loadTexture()
{
	QString filepath = QCoreApplication::applicationDirPath() + "/../../assets/images/";
	QString filename1 = filepath + "fill1.bmp";
	QImage image;
	if (!image.load(filename1)) return;

	QOpenGLTexture* pTexture = new QOpenGLTexture(image);
	if (pTexture)
	{
		//pTexture->setMinificationFilter(QOpenGLTexture::Linear);// Nearest);
		pTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapNearest);// Nearest);
		// Set bilinear filtering mode for texture magnification
		pTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);// ::Linear);
		// Wrap texture coordinates by repeating
		// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
		pTexture->setWrapMode(QOpenGLTexture::Repeat);

		m_texFill1 = pTexture;
	}
	filename1 = filepath + "fill2.bmp";
	QImage image1;
	if (!image1.load(filename1)) return;

	QOpenGLTexture* pTexture1 = new QOpenGLTexture(image1);
	if (pTexture1)
	{
		pTexture1->setMinificationFilter(QOpenGLTexture::LinearMipMapNearest); //QOpenGLTexture::Nearest);
		// Set bilinear filtering mode for texture magnification
		pTexture1->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);// QOpenGLTexture::Linear);
		// Wrap texture coordinates by repeating
		// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
		pTexture1->setWrapMode(QOpenGLTexture::Repeat);

		m_texFill2 = pTexture1;
	}
}
//读主线数据
bool rdRoadSlope::readData()
{
	//if (htNo < 1 || htNo>m_num) return false;
	
	QString filepath = QCoreApplication::applicationDirPath() + "/../../assets/data/Section";

	for (int i = 1; i <=1/* m_num*/; i++)
	{
		QString numstr = QString::number(i);
		QString snumstr = numstr;
		if (i < 10) snumstr = "0" + numstr;

		QString filename = filepath + snumstr + "/xp" + numstr + ".txt";
		QFile file(filename);
		if (!file.exists()) continue;

		bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
		if (ret)
		{
			QTextStream fin(&file);
			_RoadSlopeData rsd;
			m_RoadSlope[i - 1].clear();

    		QString str;
	 		int htno = i - 1;

	 		while (!fin.atEnd())
	 		{
				fin >> rsd._pileKNo;
				if (rsd._pileKNo == "" || fin.atEnd()) break;

	 			fin >> rsd.leftSign;
				fin >> rsd.rightSign;
				fin >> rsd.leftHeight;
				fin >> rsd.rightHeight;

				fin >> rsd.leftDistance;
				fin >> rsd.rightDistance;

				fin >> rsd.leftNum;
				fin >> rsd.rightNum;

				fin >> rsd.leftRnum;
				fin >> rsd.rightRnum;

				for (int i = 0; i < rsd.leftNum; i++)
				{
					fin >> rsd.left[i];
				}

				for (int i = 0; i < rsd.rightNum; i++)
				{
					fin >> rsd.right[i];
				}

				for (int i = 0; i < rsd.leftRnum; i++)
				{
					fin >> rsd.leftr[i];
				}

				for (int i = 0; i < rsd.rightRnum; i++)
				{
					fin >> rsd.rightr[i];
				}

				//计算pileNo
				str = rsd._pileKNo;
				int sindex = (int)str.indexOf('-');//0或-1
				int index = (int)str.indexOf('+');
				int kindex = (int)str.indexOf('K');

	            QString substr = str.mid(kindex + 1, index - kindex - 1);

				int kk = substr.toInt();

				int len = (int)str.length();
				QString substr1 = str.mid(index + 1, len - index);
				double  s = substr1.toDouble();
				double f = kk * 1000 + s;
				rsd.pileNo = f;

				if (sindex == 0) rsd.pileNo = -f;

				m_RoadSlope[htno].push_back(rsd);
			 }
		
			file.close();
		}
	}	

	return true;
}

void rdRoadSlope::computing(rdRoadLine& rline, rdRoadRamp& rramp, rdRoadDitch& rditch)
{
	_mp mp;
	_lrdis lr;
	_RoadSlopeData rsd;
	_lrdis d;
	QVector3D axis(0, 0, 1);

	memset(&mp.lpoints, 0, sizeof(mp.lpoints));
	memset(&mp.rpoints, 0, sizeof(mp.rpoints));
	
	//主线数据
	for (int no = 0; no <1 /*14*/; no++)
	{
		dis2[no].clear();
		for (int i = 0; i < m_RoadSlope[no].size(); i++)
		{
			rsd = m_RoadSlope[no][i];

			if (rsd.pileNo == 21730)
				rsd = rsd;

			_RoadCrossSection rcs = rline.getRCS(i, no + 1);

			float alpha = (float)rcs.alpha;
			float theta = 0;
			if (0 <= alpha && theta < 270) theta = -90 + alpha;
			else
			{
				theta = 270 + alpha;
			}
			float hl = rsd.leftHeight;      float hr = rsd.rightHeight;

			mp.ld = rsd.leftDistance;   mp.rd = rsd.rightDistance;
			mp.hl = hl; mp.hr = hr;     mp.leftsign = rsd.leftSign;   mp.rightsign = rsd.rightSign;

			////cgj
			mp.pile = rsd.pileNo;
			mp.lefttype = rcs.lefttype;
			mp.righttype = rcs.righttype;
			///cgj

			mp.leftNum = rsd.leftNum;  mp.rightNum = rsd.rightNum;

			QVector3D pt(rcs.center.x(), rcs.center.y(), rcs.center.z());
			QMatrix4x4 transformedMatrix;
			transformedMatrix.translate(pt);

			QMatrix4x4 transformedMatrix1;
			transformedMatrix1.rotate(theta, axis);
			QMatrix4x4 mm = transformedMatrix * transformedMatrix1;

			mp.cpoints = QVector3D(rcs.center.x(), rcs.center.y(), rcs.center.z());

			mp.theta = theta;
			mp._pileKNo = rcs._pileKNo;

			float w = rcs.lW1 + rcs.lW2 + rcs.lW3 + rcs.W0 / 2;//左幅路面宽度
			float w0 = rcs.rW1 + rcs.rW2 + rcs.rW3 + rcs.W0 / 2;//右幅路面宽度

			if (rsd.leftSign != 2)
			{
				lr.ldis = 0;//路面外侧点到斜坡长度（桥和填方）
			}
			else//挖方
			{
				if (rsd.leftNum == 0)
				{
					lr.ldis = rsd.leftDistance - w;
				}
				else
				{
					int a = rsd.leftNum - 1;
					int ii = 0;
					int suml = 0;
					while (a > ii)
					{
						float l = 8 * rsd.left[ii];
						suml += l;
						float l1 = rsd.leftr[ii];
						suml += l1;
						ii++;
					}
					float h = rsd.leftHeight - a * 8;
					float l = h * rsd.left[rsd.leftNum - 1];

					float ldis = rsd.leftDistance - w - suml - l;
					if (ldis >= 0) lr.ldis = ldis;
					else lr.ldis = 0;
				}
			}

			if (rsd.rightSign != 2)
			{
				lr.rdis = 0;

				dis2[no].push_back(lr);
			}
			else
			{
				if (rsd.rightNum == 0)
				{
					lr.rdis = rsd.rightDistance - w0;
				}
				else
				{
					int a = rsd.rightNum - 1;
					int ii = 0;
					int sumr = 0;
					while (a > ii)
					{
						float l = 8 * rsd.right[ii];
						sumr += l;
						float l1 = rsd.rightr[ii];
						sumr += l1;
						ii++;
					}
					float h = rsd.rightHeight - a * 8;
					float l = h * rsd.right[rsd.rightNum - 1];

					float rdis = rsd.rightDistance - w0 - sumr - l;

					//if (w < 5) rdis -= w;//只有右幅道路时，距离到左端//cgj
					if (rdis >= 0)lr.rdis = rdis;
					else lr.rdis = 0;
				}

				dis2[no].push_back(lr);
			}
			//........挖方的左右距离计算完成..........

			d = dis2[no][i];

			if (rsd.pileNo == 81591)
				d = d;

			//左路面
			QVector3D left(-w, 0, 0);
			QVector4D bl(left, 1);
			QVector4D al = mm * bl;

			mp.lsurface[0] = QVector3D(rcs.center.x(), rcs.center.y(), rcs.center.z());
			mp.lsurface[1] = QVector3D(al.x(), al.y(), al.z());

			//右路面
			QVector3D right(w0, 0, 0);
			QVector4D br(right, 1);
			QVector4D ar = mm * br;

			mp.rsurface[0] = QVector3D(rcs.center.x(), rcs.center.y(), rcs.center.z());
			mp.rsurface[1] = QVector3D(ar.x(), ar.y(), ar.z());
			///////////////

			if (rsd.leftSign == 0) //不存在斜坡
			{
				QVector3D left(-w, 0, 0);
				QVector4D bl(left, 1);
				QVector4D al = mm * bl;
				mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());
				mp.lpoints[1] = QVector3D(al.x(), al.y(), al.z());
				mp.lsign = 0;
				mp.leftNum = 1;
			}
			else if (rsd.leftSign == 1) //填方
			{
				int sh = 8;
				int h = 10;

				float hlm = hl;//左侧完成高度

				if (rsd.leftNum == 0)
				{
					QVector3D left(-w, 0, 0);

					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());
					mp.lpoints[1] = QVector3D(al.x(), al.y(), al.z());
					mp.leftNum = 1;
				}
				else if (rsd.leftNum == 1)
				{
					float l = w;

					QVector3D left(-l, 0, 0);

					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

					if (rsd.left[0] < 5)
					{
						float l1 = rsd.leftDistance;
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;

						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() - rsd.leftHeight);
						mp.leftNum = 1;
					}
					else
					{
						float l1 = rsd.leftDistance;
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;

						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z());
						mp.lpoints[2] = QVector3D(al1.x(), al1.y(), al.z());///cgj
						mp.lpoints[3] = QVector3D(al1.x(), al1.y(), al.z() - rsd.leftHeight);

						mp.leftNum = 2;
					}
				}
				else if (rsd.leftNum == 2)
				{
					float l = w;
					QVector3D left(-l, 0, 0);

					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

					if (rsd.left[1] < 5)
					{
						float l1 = w + sh * rsd.left[0];
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;
						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() - sh);

						mp.lpoints[2] = QVector3D(al1.x(), al1.y(), al.z() - sh);//cgj
						float l2 = rsd.leftDistance;
						QVector3D left2(-l2, 0, 0);
						QVector4D bl2(left2, 1);
						QVector4D al2 = mm * bl2;
						mp.lpoints[3] = QVector3D(al2.x(), al2.y(), al.z() - rsd.leftHeight);

						mp.leftNum = 2;
					}
					else
					{
						float hs = (rsd.leftDistance - w) / rsd.left[0];

						float l1 = rsd.leftDistance;
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;
						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() - hs);

						mp.lpoints[2] = QVector3D(al1.x(), al1.y(), al.z() - hs);//cgj
						mp.lpoints[3] = QVector3D(al1.x(), al1.y(), al.z() - rsd.leftHeight);
						mp.leftNum = 2;
					}

					mp.leftNum = 2;
				}
				else if (rsd.leftNum == 3)
				{
					//一级坡
					float l = w;
					QVector3D left(-l, 0, 0);
					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

					float l1 = w + sh * rsd.left[0];
					QVector3D left1(-l1, 0, 0);
					QVector4D bl1(left1, 1);
					QVector4D al1 = mm * bl1;
					mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() - sh);

					//二级坡
					float l2 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1];
					QVector3D left2(-l2, 0, 0);
					QVector4D bl2(left2, 1);
					QVector4D al2 = mm * bl2;
					mp.lpoints[2] = QVector3D(al1.x(), al1.y(), al.z() - sh);
					mp.lpoints[3] = QVector3D(al2.x(), al2.y(), al.z() - (2 * h));

					//三级坡，注意平台
					float l3 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1] + rsd.leftr[0];
					QVector3D left3(-l3, 0, 0);
					QVector4D bl3(left3, 1);
					QVector4D al3 = mm * bl3;
					mp.lpoints[4] = QVector3D(al3.x(), al3.y(), al.z() - (2 * h));

					float l4 = rsd.leftDistance;
					QVector3D left4(-l4, 0, 0);
					QVector4D bl4(left4, 1);
					QVector4D al4 = mm * bl4;
					mp.lpoints[5] = QVector3D(al4.x(), al4.y(), al.z() - rsd.leftHeight);
				}
				else if (rsd.leftNum == 4)
				{
					float l = w;
					QVector3D left(-l, 0, 0);
					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

					float l1 = w + sh * rsd.left[0];
					QVector3D left1(-l1, 0, 0);
					QVector4D bl1(left1, 1);
					QVector4D al1 = mm * bl1;
					mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() - sh);

					float l2 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1];
					QVector3D left2(-l2, 0, 0);
					QVector4D bl2(left2, 1);
					QVector4D al2 = mm * bl2;
					mp.lpoints[2] = QVector3D(al2.x(), al2.y(), al.z() - (2 * h));

					float l3 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1] + rsd.leftr[0];
					QVector3D left3(-l3, 0, 0);
					QVector4D bl3(left3, 1);
					QVector4D al3 = mm * bl3;
					mp.lpoints[3] = QVector3D(al3.x(), al3.y(), al.z() - (2 * h));

					float l4 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1] + rsd.leftr[0] + h * rsd.left[2];
					QVector3D left4(-l4, 0, 0);
					QVector4D bl4(left4, 1);
					QVector4D al4 = mm * bl4;
					mp.lpoints[4] = QVector3D(al4.x(), al4.y(), al.z() - (3 * h));

					float l5 = w + sh * rsd.left[0] + (2 * h - sh) * rsd.left[1] + rsd.leftr[0] + h * rsd.left[2] + rsd.leftr[1];
					QVector3D left5(-l5, 0, 0);
					QVector4D bl5(left5, 1);
					QVector4D al5 = mm * bl5;
					mp.lpoints[5] = QVector3D(al5.x(), al5.y(), al.z() - (3 * h));

					float l6 = rsd.leftDistance;
					QVector3D left6(-l6, 0, 0);
					QVector4D bl6(left6, 1);
					QVector4D al6 = mm * bl6;
					mp.lpoints[6] = QVector3D(al6.x(), al6.y(), al.z() - rsd.leftHeight);
				}

				mp.lsign = 1;

			}
			else //挖方
			{
				int sh = 8;

				float hlm = hl;

				for (int ck = 0; ck < 2 * rsd.leftNum; ck++)
					mp.lpoints[ck] = QVector3D(0, 0, 0);

				if (rsd.leftNum == 0)
				{
					QVector3D left(-w, 0, 0);
					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());
					mp.lpoints[1] = QVector3D(al.x(), al.y(), al.z());

					mp.leftNum = 1;
				}
				else if (rsd.leftNum == 1)
				{
					if (rsd.left[0] < 5)
					{
						float l = w + d.ldis;
						QVector3D left(-l, 0, 0);
						QVector4D bl(left, 1);
						QVector4D al = mm * bl;
						mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

						float l1 = rsd.leftDistance;
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;

						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z() + rsd.leftHeight);
					}
					else
					{
						float l = w;
						QVector3D left(-l, 0, 0);
						QVector4D bl(left, 1);
						QVector4D al = mm * bl;
						mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

						float l1 = rsd.leftDistance;
						QVector3D left1(-l1, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;

						mp.lpoints[1] = QVector3D(al1.x(), al1.y(), al.z());
					}

				}
				else if (rsd.leftNum >= 2)
				{
					float l = w + d.ldis;
					QVector3D left(-l, 0, 0);
					QVector4D bl(left, 1);
					QVector4D al = mm * bl;
					mp.lpoints[0] = QVector3D(al.x(), al.y(), al.z());

					float cw = w + d.ldis;
					float ccw = 0;
					float ccplat = 0;
					for (int ci = 0; ci < rsd.leftNum - 1; ci++)
					{
						ccw += sh * rsd.left[ci];

						ccplat = 0;
						for (int cj = 0; cj < ci; cj++)
							ccplat += rsd.leftr[cj];

						QVector3D left1(-ccw - cw - ccplat, 0, 0);
						QVector4D bl1(left1, 1);
						QVector4D al1 = mm * bl1;
						mp.lpoints[ci * 2 + 1] = QVector3D(al1.x(), al1.y(), al.z() + (ci + 1) * sh);

						ccplat += rsd.leftr[ci];
						QVector3D left2(-ccw - cw - ccplat, 0, 0);
						QVector4D bl2(left2, 1);
						QVector4D al2 = mm * bl2;
						mp.lpoints[ci * 2 + 2] = QVector3D(al2.x(), al2.y(), al.z() + (ci + 1) * sh);
					}

					float l15 = rsd.leftDistance;
					QVector3D left15(-l15, 0, 0);
					QVector4D bl15(left15, 1);
					QVector4D al15 = mm * bl15;
					int lastindex = rsd.leftNum * 2 - 1;
					mp.lpoints[lastindex] = QVector3D(al15.x(), al15.y(), al.z() + rsd.leftHeight);//15
				}

				mp.lsign = 1;
			}

			if (rsd.rightSign == 0) //不存在斜坡
			{
				QVector3D right(w0, 0, 0);
				QVector4D br(right, 1);
				QVector4D ar = mm * br;
				mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());
				mp.rpoints[1] = QVector3D(ar.x(), ar.y(), ar.z());

				mp.rsign = 0;
				mp.rightNum = 1;
			}
			else if (rsd.rightSign == 1) //填方
			{
				int sh = 8;
				int h = 10;

				float hrm = hr;

				if (rsd.rightNum == 0)
				{
					QVector3D right(w0, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());
					mp.rpoints[1] = QVector3D(ar.x(), ar.y(), ar.z());

					mp.rightNum = 1;
				}
				else if (rsd.rightNum == 1)
				{
					float l = w0;
					QVector3D right(l, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

					if (rsd.right[0] < 5)
					{
						float l1 = rsd.rightDistance;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() - rsd.rightHeight);
						mp.rightNum = 1;
					}
					else
					{
						float l1 = rsd.rightDistance;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;

						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z());
						mp.rpoints[2] = QVector3D(ar1.x(), ar1.y(), ar.z());//cgj
						mp.rpoints[3] = QVector3D(ar1.x(), ar1.y(), ar.z() - rsd.rightHeight);

						mp.rightNum = 2;
					}
				}
				else if (rsd.rightNum == 2)
				{
					float l = w0;
					QVector3D right(l, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

					if (rsd.right[1] < 5)
					{
						float l1 = w0 + rsd.right[0] * sh;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);

						float l2 = rsd.rightDistance;
						QVector3D right2(l2, 0, 0);
						QVector4D br2(right2, 1);
						QVector4D ar2 = mm * br2;
						mp.rpoints[2] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);
						mp.rpoints[3] = QVector3D(ar2.x(), ar2.y(), ar.z() - rsd.rightHeight);

						mp.rightNum = 2;
					}
					else
					{
						float hs = (rsd.rightDistance - w0) / rsd.right[0];

						float l1 = rsd.rightDistance;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() - hs);

						mp.rpoints[2] = QVector3D(ar1.x(), ar1.y(), ar.z() - hs);
						mp.rpoints[3] = QVector3D(ar1.x(), ar1.y(), ar.z() - rsd.rightHeight);
						mp.rightNum = 2;
					}
				}
				else if (rsd.rightNum == 3)
				{
					//一级坡
					float l = w0;
					QVector3D right(l, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

					float l1 = w0 + sh * rsd.right[0];
					QVector3D right1(l1, 0, 0);
					QVector4D br1(right1, 1);
					QVector4D ar1 = mm * br1;
					mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);

					//二级坡
					float l2 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1];
					QVector3D right2(l2, 0, 0);
					QVector4D br2(right2, 1);
					QVector4D ar2 = mm * br2;

					mp.rpoints[2] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);
					mp.rpoints[3] = QVector3D(ar2.x(), ar2.y(), ar.z() - (2 * h));

					//三级坡
					float l3 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1] + rsd.rightr[0];
					QVector3D right3(l3, 0, 0);
					QVector4D br3(right3, 1);
					QVector4D ar3 = mm * br3;
					mp.rpoints[4] = QVector3D(ar3.x(), ar3.y(), ar.z() - (2 * h));

					float l4 = rsd.rightDistance;
					QVector3D right4(l4, 0, 0);
					QVector4D br4(right4, 1);
					QVector4D ar4 = mm * br4;
					mp.rpoints[5] = QVector3D(ar4.x(), ar4.y(), ar.z() - rsd.rightHeight);

					mp.rightNum = 3;
				}
				else if (rsd.rightNum == 4)
				{
					float l = w0;
					QVector3D right(l, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

					float l1 = w0 + sh * rsd.right[0];
					QVector3D right1(l1, 0, 0);
					QVector4D br1(right1, 1);
					QVector4D ar1 = mm * br1;
					mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);

					//二级坡
					float l2 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1];
					QVector3D right2(l2, 0, 0);
					QVector4D br2(right2, 1);
					QVector4D ar2 = mm * br2;
					mp.rpoints[2] = QVector3D(ar1.x(), ar1.y(), ar.z() - sh);
					mp.rpoints[3] = QVector3D(ar2.x(), ar2.y(), ar.z() - (2 * h));

					//三级坡，中间有1级平台
					float l3 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1] + rsd.rightr[0];
					QVector3D right3(l3, 0, 0);
					QVector4D br3(right3, 1);
					QVector4D ar3 = mm * br3;
					mp.rpoints[4] = QVector3D(ar3.x(), ar3.y(), ar.z() - (2 * h));

					float l4 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1] + rsd.rightr[0] + h * rsd.right[2];
					QVector3D right4(l4, 0, 0);
					QVector4D br4(right4, 1);
					QVector4D ar4 = mm * br4;
					mp.rpoints[5] = QVector3D(ar4.x(), ar4.y(), ar.z() - (3 * h));

					//四级坡，中间有2级平台
					float l5 = w0 + sh * rsd.right[0] + (2 * h - sh) * rsd.right[1] + rsd.rightr[0] + h * rsd.right[2] + rsd.rightr[1];
					QVector3D right5(l5, 0, 0);
					QVector4D br5(right5, 1);
					QVector4D ar5 = mm * br5;
					mp.rpoints[6] = QVector3D(ar5.x(), ar5.y(), ar.z() - (3 * h));

					float l6 = rsd.rightDistance;
					QVector3D right6(l6, 0, 0);
					QVector4D br6(right6, 1);
					QVector4D ar6 = mm * br6;
					mp.rpoints[7] = QVector3D(ar6.x(), ar6.y(), ar.z() - rsd.rightHeight);

					mp.rightNum = 4;
				}

				mp.rsign = 1;

			}
			else //挖方
			{
				int sh = 8;

				float hrm = hr;

				if (rsd.rightNum == 0)
				{
					QVector3D right(w0, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());
					mp.rpoints[1] = QVector3D(ar.x(), ar.y(), ar.z());
					mp.rightNum = 1;
				}
				else if (rsd.rightNum == 1)
				{
					if (rsd.right[0] < 5)
					{
						float l = w0 + d.rdis;
						QVector3D right(l, 0, 0);
						QVector4D br(right, 1);
						QVector4D ar = mm * br;
						mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

						float l1 = rsd.rightDistance;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z() + rsd.rightHeight);
					}
					else
					{
						float l = w0;
						QVector3D right(l, 0, 0);
						QVector4D br(right, 1);
						QVector4D ar = mm * br;
						mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

						float l1 = rsd.rightDistance;
						QVector3D right1(l1, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[1] = QVector3D(ar1.x(), ar1.y(), ar.z());
					}
				}
				else if (rsd.rightNum >= 2)
				{
					float l = w0 + d.rdis;
					QVector3D right(l, 0, 0);
					QVector4D br(right, 1);
					QVector4D ar = mm * br;
					mp.rpoints[0] = QVector3D(ar.x(), ar.y(), ar.z());

					float cw = w0 + d.rdis;
					float ccw = 0;
					float ccplat = 0;
					for (int ci = 0; ci < rsd.rightNum - 1; ci++)
					{
						ccw += sh * rsd.right[ci];

						ccplat = 0;
						for (int cj = 0; cj < ci; cj++)
							ccplat += rsd.rightr[cj];

						QVector3D right1(ccw + cw + ccplat, 0, 0);
						QVector4D br1(right1, 1);
						QVector4D ar1 = mm * br1;
						mp.rpoints[ci * 2 + 1] = QVector3D(ar1.x(), ar1.y(), ar.z() + (ci + 1) * sh);

						ccplat += rsd.rightr[ci];
						QVector3D lright2(ccw + cw + ccplat, 0, 0);
						QVector4D br2(lright2, 1);
						QVector4D ar2 = mm * br2;
						mp.rpoints[ci * 2 + 2] = QVector3D(ar2.x(), ar2.y(), ar.z() + (ci + 1) * sh);
					}

					float lr5 = rsd.rightDistance;
					QVector3D right15(lr5, 0, 0);
					QVector4D br15(right15, 1);
					QVector4D ar15 = mm * br15;
					int lastindex = rsd.rightNum * 2 - 1;
					mp.rpoints[lastindex] = QVector3D(ar15.x(), ar15.y(), ar.z() + rsd.rightHeight);//15			
				}

				mp.rsign = 1;
			}

			//排水沟--护坡道
			if (rsd.pileNo == 21730)
				rsd = rsd;

			int blditch = rditch.m_ditchData[no][i].lditchSign;
			float blramp = rramp.m_rampData[no][i].lwidth;
			float width = rditch.m_ditchData[no][i].width;
			float height = rditch.m_ditchData[no][i].height;

			mp.bLDitch = false;
			mp.bLRamp = false;
			if (blramp > 0) mp.bLRamp = true;

			if (blramp > 0)//有
			{
				if (rsd.leftSign == 1)//填方
				{
					int num = mp.leftNum;
					QVector3D p0 = mp.lpoints[2 * num - 2];
					QVector3D p1 = mp.lpoints[2 * num - 1];//
					float rampwidth = rramp.m_rampData[no][i].lwidth;

					float dis = ptDis(p0, p1);
					float t1 = (dis + rampwidth) / dis;
					float t2 = (dis + rampwidth + 0.1) / dis;
					float t3 = (dis + rampwidth + 0.1 + width) / dis;
					float t4 = (dis + rampwidth + width + 0.2) / dis;
					QVector3D pp0 = p1;
					QVector3D pp1 = p0 + (p1 - p0) * t1;
					QVector3D pp2 = p0 + (p1 - p0) * t2;
					QVector3D pp3 = p0 + (p1 - p0) * t3;
					QVector3D pp4 = p0 + (p1 - p0) * t4;

					pp1.setZ(pp0.z());
					pp2.setZ(pp0.z());
					pp3.setZ(pp0.z());
					pp4.setZ(pp0.z());
					if (blditch > 0)
					{
						mp.bLDitch = true;
						mp.lditch[0] = pp1;
						mp.lditch[1] = pp2;
						mp.lditch[2] = pp2;
						//mp.lditch[2].z -= height;
						mp.lditch[2].setZ(mp.lditch[2].z() - height);
						mp.lditch[3] = pp3;
						//mp.lditch[3].z -= height;
						mp.lditch[3].setZ(mp.lditch[3].z() - height);
						mp.lditch[4] = pp3;
						mp.lditch[5] = pp4;

						mp.lramp[0] = pp0;
						mp.lramp[1] = pp1;
					}
					else
					{
						mp.lramp[0] = pp0;
						mp.lramp[1] = pp1;
					}
				}
				else if (rsd.leftSign == 2)//挖方
				{
					int num = mp.leftNum;
					QVector3D p1 = mp.lpoints[0];
					QVector3D p0 = mp.lsurface[1];//
					float dis = ptDis(p0, p1);
					float t1 = (0.1) / dis;
					float t2 = (width + 0.1) / dis;
					float t3 = (width + 0.2) / dis;

					QVector3D pp0 = p0;
					QVector3D pp1 = p0 + (p1 - p0) * t1;
					QVector3D pp2 = p0 + (p1 - p0) * t2;
					QVector3D pp3 = p0 + (p1 - p0) * t3;

					if (blditch > 0)
					{
						mp.bLDitch = true;
						mp.lditch[0] = pp0;
						mp.lditch[1] = pp1;
						mp.lditch[2] = pp1;
						//mp.lditch[2].z -= height;
						mp.lditch[2].setZ(mp.lditch[2].z() - height);
						mp.lditch[3] = pp2;
						//mp.lditch[3].z -= height;
						mp.lditch[3].setZ(mp.lditch[3].z() - height);
						mp.lditch[4] = pp2;
						mp.lditch[5] = pp3;

						mp.lramp[0] = pp3;
						mp.lramp[1] = p1;
					}
					else
					{
						mp.lramp[0] = p0;
						mp.lramp[1] = p1;
					}
				}
				else//没有坡
				{

				}
			}
			int brditch = rditch.m_ditchData[no][i].rditchSign;
			float brramp = rramp.m_rampData[no][i].rwidth;
			mp.bRDitch = false;
			mp.bRRamp = false;
			if (brramp > 0.0f) mp.bRRamp = true;
			if (brramp > 0)//有
			{
				if (rsd.rightSign == 1)//填方
				{
					int num = mp.rightNum;
					QVector3D p0 = mp.rpoints[2 * num - 2];
					QVector3D p1 = mp.rpoints[2 * num - 1];//
					float rampwidth = rramp.m_rampData[no][i].rwidth;

					float dis = ptDis(p0, p1);
					float t1 = (dis + rampwidth) / dis;
					float t2 = (dis + rampwidth + 0.1) / dis;
					float t3 = (dis + rampwidth + 0.1 + width) / dis;
					float t4 = (dis + rampwidth + width + 0.2) / dis;
					QVector3D pp0 = p1;
					QVector3D pp1 = p0 + (p1 - p0) * t1;
					QVector3D pp2 = p0 + (p1 - p0) * t2;
					QVector3D pp3 = p0 + (p1 - p0) * t3;
					QVector3D pp4 = p0 + (p1 - p0) * t4;

					/*pp1.z = pp0.z;
					pp2.z = pp0.z;
					pp3.z = pp0.z;
					pp4.z = p1.z;*/

					pp1.setZ(pp0.z());
					pp2.setZ(pp0.z());
					pp3.setZ(pp0.z());
					pp4.setZ(p1.z());

					if (brditch > 0)
					{
						mp.bRDitch = true;
						mp.rditch[0] = pp1;
						mp.rditch[1] = pp2;
						mp.rditch[2] = pp2;
						//mp.rditch[2].z -= height;
						mp.rditch[2].setZ(mp.rditch[2].z() - height);
						mp.rditch[3] = pp3;
						//mp.rditch[3].z -= height;
						mp.rditch[3].setZ(mp.rditch[3].z() - height);
						mp.rditch[4] = pp3;
						mp.rditch[5] = pp4;

						mp.rramp[0] = pp0;
						mp.rramp[1] = pp1;
					}
					else
					{
						mp.bRDitch = false;
						mp.rramp[0] = pp0;
						mp.rramp[1] = pp1;
					}
				}
				else if (rsd.rightSign == 2)//挖方
				{
					int num = mp.rightNum;
					QVector3D p1 = mp.rpoints[0];
					QVector3D p0 = mp.rsurface[1];//
					float dis = ptDis(p0, p1);
					float t1 = (0.1) / dis;
					float t2 = (width + 0.1) / dis;
					float t3 = (width + 0.2) / dis;

					QVector3D pp0 = p0;
					QVector3D pp1 = p0 + (p1 - p0) * t1;
					QVector3D pp2 = p0 + (p1 - p0) * t2;
					QVector3D pp3 = p0 + (p1 - p0) * t3;

					if (brditch > 0)
					{
						mp.rditch[0] = pp0;
						mp.rditch[1] = pp1;
						mp.rditch[2] = pp1;
						//mp.rditch[2].z -= height;
						mp.rditch[2].setZ(mp.rditch[2].z() - height);
						mp.rditch[3] = pp2;
						//mp.rditch[3].z -= height;
						mp.rditch[3].setZ(mp.rditch[3].z() - height);
						mp.rditch[4] = pp2;
						mp.rditch[5] = pp3;

						mp.rramp[0] = pp3;
						mp.rramp[1] = p1;

						mp.bRDitch = true;
					}
					else
					{
						mp.bRDitch = false;

						mp.rramp[0] = p0;
						mp.rramp[1] = p1;
					}
				}
				else//没有坡
				{

				}
			}

			//边沟--碎落台结束

			//平台
			mp.iLplatform = 0;
			mp.iRplatform = 0;
			if (rsd.pileNo == 29988)
				mp.iRplatform = 0;

			if (rsd.leftRnum > 0)//左
			{
				mp.iLplatform = rsd.leftRnum;
				if (rsd.leftSign == 1)//填方
				{
					mp.lplatform[0] = mp.lpoints[3];
					mp.lplatform[1] = mp.lpoints[4];

					if (rsd.leftRnum == 2)
					{
						mp.lplatform[2] = mp.lpoints[5];
						mp.lplatform[3] = mp.lpoints[6];
					}
				}
				else if (rsd.leftSign == 2)//挖方
				{
					for (int ll = 0; ll < rsd.leftRnum; ll++)
					{
						float pw = rsd.leftr[ll];//平台宽度
						float t0 = 0.3 / pw;
						float t1 = (0.3 + 0.6) / pw;
						QVector3D p0 = mp.lpoints[2 * (ll + 1)];
						QVector3D p1 = mp.lpoints[2 * (ll + 1) - 1];

						mp.lplatform[4 * ll] = p0;
						mp.lplatform[4 * ll + 1] = p0 + (p1 - p0) * t0;
						mp.lplatform[4 * ll + 2] = p0 + (p1 - p0) * t1;
						mp.lplatform[4 * ll + 3] = p1;

						//平台排水沟
						mp.lplatformditch[2 * ll] = p0 + (p1 - p0) * t0;
						mp.lplatformditch[2 * ll + 1] = p0 + (p1 - p0) * t1;
					}
				}
			}
			if (rsd.rightRnum > 0)
			{
				mp.iRplatform = rsd.rightRnum;
				if (rsd.rightSign == 1)//填方
				{
					mp.rplatform[0] = mp.rpoints[3];
					mp.rplatform[1] = mp.rpoints[4];

					if (rsd.rightRnum == 2)
					{
						mp.rplatform[2] = mp.rpoints[5];
						mp.rplatform[3] = mp.rpoints[6];
					}
				}
				else if (rsd.rightSign == 2)//挖方
				{
					for (int ll = 0; ll < rsd.rightRnum; ll++)
					{
						float pw = rsd.rightr[ll];//平台宽度
						float t0 = 0.3 / pw;
						float t1 = (0.3 + 0.6) / pw;

						QVector3D p0 = mp.rpoints[2 * (ll + 1)];
						QVector3D p1 = mp.rpoints[2 * (ll + 1) - 1];

						mp.rplatform[4 * ll] = p0;
						mp.rplatform[4 * ll + 1] = p0 + (p1 - p0) * t0;
						mp.rplatform[4 * ll + 2] = p0 + (p1 - p0) * t1;
						mp.rplatform[4 * ll + 3] = p1;

						//平台排水沟
						mp.rplatformditch[2 * ll] = p0 + (p1 - p0) * t0;
						mp.rplatformditch[2 * ll + 1] = p0 + (p1 - p0) * t1;
					}
				}
			}

			/////////////
			m_fiPoints2[no].push_back(mp);

			memset(&mp.lpoints, 0, sizeof(mp.lpoints));
			memset(&mp.rpoints, 0, sizeof(mp.rpoints));
		}
	}

	//生成四边形
	makeQuad();
	makeDitchQuad();
	makeRampQuad();
}

float rdRoadSlope::ptDis(QVector3D p0, QVector3D p1)
{
	float dx = p1.x() - p0.x();
	float dy = p1.y() - p0.y();

	return sqrt(dx * dx + dy * dy);
}

//生成长方形
void rdRoadSlope::makeQuad()
{
	//主线部分
	float length = 10.0;//纹理长度
	float height = 5.0f;
	float curLeftLength = 0;
	float curRightLength = 0;
	float lastLeftLength = 0;
	float lastRightLength = 0;

	for (int no = 0; no <1 /*14*/; no++)
	{
		if (m_fiPoints2[no].size() < 2) continue;

		for (int i = 0; i < m_fiPoints2[no].size() - 1; i++)
		{
			_mp mp0 = m_fiPoints2[no][i];
			_mp mp1 = m_fiPoints2[no][i + 1];

			_RoadSlopeQuad rsq;

			rsq.m_bLFillToExcave = false;
			rsq.m_bRFillToExcave = false;

			rsq.status = 0;
			rsq.m_bPure = false;
			rsq.m_bSoft = false;
			rsq.m_bRoll = false;

			QVector3D pp[4];
			//路面
			//左
			pp[0] = mp0.lsurface[0];
			pp[1] = mp0.lsurface[1];
			pp[2] = mp1.lsurface[1];
			pp[3] = mp1.lsurface[0];
			_QUAD quad;
			for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
			rsq.m_leftsurface = quad;

			//右
			pp[0] = mp0.rsurface[0];
			pp[1] = mp0.rsurface[1];
			pp[2] = mp1.rsurface[1];
			pp[3] = mp1.rsurface[0];
			for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];

			rsq.m_rightsurface = quad;

			///斜坡/////////////////////
			//left
			if (mp0.leftsign == 0)// 当前无斜坡
			{
				lastLeftLength = 0;
				curLeftLength = 0;
			}
			else if (mp0.leftsign == 1)// 当前是填方
			{
				//if (mp1.leftsign == 0 && mp1.) // 之前是填方
				{
					lastLeftLength = curLeftLength;
					float dis = mp1.pile - mp0.pile;
					//断链
					if (dis > 100)
					{
						dis = sqrt((mp0.lpoints[0].x() - mp1.lpoints[0].x()) * (mp0.lpoints[0].x() - mp1.lpoints[0].x())
							+ (mp0.lpoints[0].y() - mp1.lpoints[0].y()) * (mp0.lpoints[0].y() - mp1.lpoints[0].y()));
					}
					curLeftLength += dis;
				}
				//else
				//{
				//	lastLeftLength = 0;
				//	curLeftLength = 0;
				//}
			}
			else if (mp0.leftsign == 2)// 当前是挖方
			{
				lastLeftLength = curLeftLength;
				float dis = mp1.pile - mp0.pile;
				curLeftLength += dis;
			}
			rsq.leftNum = 0;
			if (mp0.leftsign == 0 && mp1.leftsign > 0)
			{
				rsq.leftNum = 0;
				rsq.leftSign = mp0.leftsign;
				//for (int k = 0; k < mp1.leftNum; k++)
				//{
				//	pp[0] = p;
				//	pp[1] = mp1.lpoints[2 * k];
				//	pp[2] = mp1.lpoints[2 * k + 1];
				//	pp[3] = p;// mp1.lpoints[2 * k]; //p;

				//	_QUAD quad;
				//	for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
				//	//纹理坐标
				//	lastLeftLength = 0;
				//	lastLeftLength = mp1.pile - mp0.pile;
				//	float s0 = lastLeftLength / length;
				//	float s1 = curLeftLength / length;
				//	float tl = (pp[2].z - pp[1].z) / height;
				//	float tr2 = 0;// (pp[2].z - pp[3].z) / height;
				//	float tr1 = 0;

				//	rsq.leftSign = 2;
				//	if (mp1.leftsign == 1)
				//	{
				//		//tl = -tl;
				//		tl = -tl;

				//		rsq.leftSign = 1;
				//	}

				//	quad.text[0].s = s0; quad.text[0].t = tr1;
				//	quad.text[1].s = s1; quad.text[3].t = tr2;
				//	//quad.text[1].s = s1; quad.text[1].t = tr1;
				//	quad.text[2].s = s1; quad.text[2].t = tl;
				//	//quad.text[3].s = s0; quad.text[3].t = tl;
				//	quad.text[3].s = s0; quad.text[3].t = tr1;

				//	rsq.m_left[k] = quad;
				//}
				lastLeftLength = 0;
				curLeftLength = 0;
			}
			else if (mp0.leftsign > 0 && mp1.leftsign == 0)
			{
				rsq.leftNum = 0;
				rsq.leftSign = mp1.leftsign;
				lastLeftLength = 0;
				curLeftLength = 0;
			}
			else if (mp0.leftsign > 0 && mp1.leftsign > 0 /*&& mp0.leftsign== mp1.leftsign*/)//不限制挖填方
			{
				rsq.leftSign = mp0.leftsign;
				if (mp1.leftNum == mp0.leftNum)
				{
					rsq.leftNum = mp0.leftNum;
					for (int k = 0; k < mp0.leftNum; k++)
					{
						pp[0] = mp0.lpoints[2 * k];
						pp[1] = mp1.lpoints[2 * k];
						pp[2] = mp1.lpoints[2 * k + 1];
						pp[3] = mp0.lpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastLeftLength / length;
						float s1 = curLeftLength / length;
						float tl = fabs(pp[3].z() - pp[0].z()) / height;
						float tr = fabs(pp[2].z() - pp[1].z()) / height;

						if (mp0.leftsign == 1)//填方
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}
						quad.text[0].setX(s0); /*= s0;*/ quad.text[0].setY(0);// t = 0;
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tr;
						quad.text[2].setX(s1); quad.text[2].setY(tr);
						//quad.text[3].s = s0; quad.text[3].t = tl;
						quad.text[3].setX(s0); quad.text[3].setY(tl);

						if (mp0.leftsign == 1)//填方
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0); quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}

						rsq.m_left[k] = quad;
					}
				}
				else if (mp1.leftNum > mp0.leftNum)
				{
					rsq.leftNum = mp1.leftNum;
					//	rsq.leftNum = mp0.leftNum;
					for (int k = 0; k < mp0.leftNum; k++)
					{
						pp[0] = mp0.lpoints[2 * k];
						pp[1] = mp1.lpoints[2 * k];
						pp[2] = mp1.lpoints[2 * k + 1];
						pp[3] = mp0.lpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];

						//纹理坐标
						float s0 = lastLeftLength / length;
						float s1 = curLeftLength / length;

						float tl = (pp[3].z() - pp[0].z()) / height;
						float tr = (pp[2].z() - pp[1].z()) / height;

						if (mp0.leftsign == 1)//填方
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0); quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tr;
						quad.text[2].setX(s1); quad.text[2].setY(tr);
						//quad.text[3].s = s0; quad.text[3].t = tl;
						quad.text[3].setX(s0); quad.text[3].setY(tl);

						if (mp0.leftsign == 1)//填方
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0); quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);

						}

						rsq.m_left[k] = quad;
					}
					QVector3D p = mp0.lpoints[2 * (mp0.leftNum) - 1];
					for (int k = mp0.leftNum; k < mp1.leftNum; k++)
					{
						pp[0] = p;
						pp[1] = mp1.lpoints[2 * k];
						pp[2] = mp1.lpoints[2 * k + 1];
						pp[3] = p;

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastLeftLength / length;
						float s1 = curLeftLength / length;

						float z = pp[2].z() - pp[1].z();
						float t = z / 8;

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0); quad.text[0].setY(0); 
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = t;//需要根据实际高度计算t
						quad.text[2].setX(s1); quad.text[2].setY(t);
						//quad.text[3].s = s0; quad.text[3].t = 0;
						quad.text[3].setX(s0); quad.text[3].setY(0);

						if (mp0.leftsign == 1)//填方
						{
							//quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[0].setX(s0); quad.text[0].setY(0);
							//quad.text[1].s = s1; quad.text[1].t = -t;
							quad.text[1].setX(s1); quad.text[1].setY(-t);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}

						rsq.m_left[k] = quad;
					}
				}
				else if (mp1.leftNum < mp0.leftNum)
				{
					rsq.leftNum = mp0.leftNum;
					rsq.leftSign = mp0.leftsign;
					//rsq.leftNum = mp1.leftNum;
					for (int k = 0; k < mp1.leftNum; k++)
					{
						pp[0] = mp0.lpoints[2 * k];
						pp[1] = mp1.lpoints[2 * k];
						pp[2] = mp1.lpoints[2 * k + 1];
						pp[3] = mp0.lpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastLeftLength / length;
						float s1 = curLeftLength / length;
						float tl = (pp[3].z() - pp[0].z()) / height;
						float tr = (pp[2].z() - pp[1].z()) / height;
						if (mp0.leftsign == 1)//填方
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tr;
						quad.text[2].setX(s1); quad.text[2].setY(tl);
						//quad.text[3].s = s0; quad.text[3].t = tl;
						quad.text[3].setX(s0); quad.text[3].setY(tr);


						if (mp0.leftsign == 1)//填方
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0); quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}
						rsq.m_left[k] = quad;
					}

					QVector3D p = mp1.lpoints[2 * (mp1.leftNum) - 1];
					for (int k = mp1.leftNum; k < mp0.leftNum; k++)
					{
						pp[0] = mp0.lpoints[2 * k];;
						pp[1] = p;
						pp[2] = p;
						pp[3] = mp0.lpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastLeftLength / length;
						float s1 = curLeftLength / length;

						float z = pp[3].z() - pp[0].z();
						float t = z / 8;
						if (mp0.leftsign == 1)//填方
							t = -t;
						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = 0;
						quad.text[2].setX(s1); quad.text[2].setY(0);
						//quad.text[3].s = s0; quad.text[3].t = t;
						quad.text[3].setX(s0); quad.text[3].setY(t);

						if (mp0.leftsign == 1)//填方
						{
							//quad.text[0].s = s0; quad.text[0].t = t;
							quad.text[0].setX(s0);   quad.text[0].setY(t);
							//quad.text[1].s = s1; quad.text[1].t = 0;
							quad.text[1].setX(s1); quad.text[1].setY(0);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}

						rsq.m_left[k] = quad;
					}
				}

				if (mp0.leftsign != mp1.leftsign)
				{
					lastLeftLength = 0;
					curLeftLength = 0;
				}
			}

			//right
			if (mp0.rightsign == 0)
			{
				lastRightLength = 0;
				curRightLength = 0;
			}
			else if (mp0.rightsign == 1)
			{
				//	if (mp1.rightsign == 1) // 当前是填方
				{
					lastRightLength = curRightLength;

					float dis = mp1.pile - mp0.pile;
					//断链
					if (dis > 100)
					{
						dis = sqrt((mp0.rpoints[0].x() - mp1.rpoints[0].x()) * (mp0.rpoints[0].x() - mp1.rpoints[0].x())
							+ (mp0.rpoints[0].y() - mp1.rpoints[0].y()) * (mp0.rpoints[0].y() - mp1.rpoints[0].y()));
					}
					curRightLength += dis;
				}
				/*else
				{
					lastRightLength = 0;
					curRightLength = 0;
				}*/
			}
			else if (mp0.rightsign == 2)
			{
				//	if (mp1.rightsign == 2) // 当前是挖方
				{
					lastRightLength = curRightLength;
					curRightLength += (mp1.pile - mp0.pile);
				}
				//	else 
				//	{
				//		lastRightLength = 0;
				//		curRightLength = 0;		
				//	}		
			}
			rsq.rightNum = 0;
			if (mp0.rightsign == 0 && mp1.rightsign > 0)
			{
				//vec3 p = mp0.rpoints[0];
				//rsq.rightNum = mp1.rightNum;
				rsq.rightNum = 0;// mp1.rightNum;
				rsq.rightSign = mp0.rightsign;
				//for (int k = 0; k < mp1.rightNum; k++)
				//{
				//	pp[0] = p;
				//	pp[1] = mp1.rpoints[2 * k];
				//	pp[2] = mp1.rpoints[2 * k + 1];
				//	pp[3] = p;

				//	_QUAD quad;
				//	for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
				//	//纹理坐标
				//	lastRightLength = 0;
				//	curRightLength = mp1.pile - mp0.pile;

				//	float s0 = lastRightLength / length;
				//	float s1 = curRightLength / length;
				//	float tl = (pp[3].z - pp[0].z) / height;
				//	float tr2 = (pp[2].z - pp[1].z) / height;
				//	float tr1 = 0;
				//	rsq.rightSign = 2;
				//	if (mp1.rightsign == 1)
				//	{
				//		//tl = -tl;
				//		tr2 = -tr2;
				//		rsq.rightSign = 1;
				//	}

				//	quad.text[0].s = s0; quad.text[0].t = tl;
				//	quad.text[1].s = s1; quad.text[1].t = tr1;
				//	quad.text[2].s = s1; quad.text[2].t = tr2;
				//	quad.text[3].s = s0; quad.text[3].t = tl;

				//	rsq.m_right[k] = quad;
				//}
				lastRightLength = 0;
				curRightLength = 0;
			}
			else if (mp0.rightsign > 0 && mp1.rightsign == 0)
			{
				rsq.rightNum = 0;
				rsq.rightSign = mp1.rightsign;
				//rsq.rightNum = mp0.rightNum;
				//for (int k = 0; k < mp0.rightNum; k++)
				//{
				//	vec3 p = mp1.rpoints[0];

				//	for (int k = 0; k < mp0.rightNum; k++)
				//	{
				//		pp[0] = mp0.rpoints[2 * k];
				//	//	pp[1] = mp0.rpoints[2 * k + 1];
				//		pp[1] = p;
				//		pp[2] = p;
				//		pp[3] = mp0.rpoints[2 * k + 1];

				//		_QUAD quad;
				//		for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
				//		//纹理坐标
				//		float s0 = lastRightLength / length;
				//		float s1 = curRightLength / length;
				//		float tr = (pp[3].z - pp[0].z) / height;
				//		//float tl1 = (pp[1].z - pp[2].z) / height;
				//		float tl2 = 0;

				//		if (mp0.rightsign == 1)
				//		{
				//			tr = -tr;
				//			//tl1 = -tl1;
				//		}

				//		quad.text[0].s = s0; quad.text[0].t = 0;
				//		//quad.text[1].s = s0; quad.text[1].t = tr;
				//		quad.text[1].s = s1; quad.text[1].t = tl2;
				//		quad.text[2].s = s1; quad.text[2].t = tl2;
				//		//quad.text[3].s = s1; quad.text[3].t = tl2;
				//		quad.text[3].s = s0; quad.text[3].t = tr;

				//		rsq.m_right[k] = quad;
				//	}
				//}
				lastRightLength = 0;
				curRightLength = 0;
			}

			if (mp0.rightsign > 0 && mp1.rightsign > 0/* && mp0.rightsign == mp1.rightsign*/)
			{
				rsq.rightSign = mp0.rightsign;
				if (mp0.pile == 20256)
					mp0.pile = 20256;

				if (mp1.rightNum == mp0.rightNum)
				{
					rsq.rightNum = mp0.rightNum;
					for (int k = 0; k < mp0.rightNum; k++)
					{
						pp[0] = mp0.rpoints[2 * k];
						pp[1] = mp1.rpoints[2 * k];
						pp[2] = mp1.rpoints[2 * k + 1];
						pp[3] = mp0.rpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastRightLength / length;
						float s1 = curRightLength / length;

						float tl = (pp[3].z() - pp[0].z()) / height;
						float tr = (pp[2].z() - pp[1].z()) / height;

						if (mp0.rightsign == 1)
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tr;
						quad.text[2].setX(s1); quad.text[2].setY(tr);
						//quad.text[3].s = s0; quad.text[3].t = tl;
						quad.text[3].setX(s0); quad.text[3].setY(tl);
						if (mp0.rightsign == 1)
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0);   quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}

						rsq.m_right[k] = quad;
					}
				}
				else if (mp1.rightNum > mp0.rightNum)
				{
					rsq.rightNum = mp1.rightNum;
					//rsq.rightNum = mp0.rightNum;
					if (mp0.pile == 32010)
						lastLeftLength = lastLeftLength;

					for (int k = 0; k < mp0.rightNum; k++)
					{
						pp[0] = mp0.rpoints[2 * k];
						pp[1] = mp1.rpoints[2 * k];
						pp[2] = mp1.rpoints[2 * k + 1];
						pp[3] = mp0.rpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastRightLength / length;
						float s1 = curRightLength / length;

						float tl = (pp[3].z() - pp[0].z()) / height;
						float tr = (pp[2].z() - pp[1].z()) / height;
						if (mp0.rightsign == 1)
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}
						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tr;
						quad.text[2].setX(s1); quad.text[2].setY(tr);
						//quad.text[3].s = s0; quad.text[3].t = tl;
						quad.text[3].setX(s0); quad.text[3].setY(tl);
						if (mp0.rightsign == 1)
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0);   quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}
						rsq.m_right[k] = quad;
					}
					QVector3D p = mp0.rpoints[2 * (mp0.rightNum) - 1];
					for (int k = mp0.rightNum; k < mp1.rightNum; k++)
					{
						pp[0] = p;
						pp[1] = mp1.rpoints[2 * k];
						pp[2] = mp1.rpoints[2 * k + 1];
						pp[3] = p;

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastRightLength / length;
						float s1 = curRightLength / length;

						float z = pp[2].z() - pp[1].z();
						float t = z / 8;

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = t;
						quad.text[2].setX(s1); quad.text[2].setY(t);
						//quad.text[3].s = s0; quad.text[3].t = 0;
						quad.text[3].setX(s0); quad.text[3].setY(0);
						if (mp0.rightsign == 1)
						{
							//quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							//quad.text[1].s = s1; quad.text[1].t = -t;
							quad.text[1].setX(s1); quad.text[1].setY(-t);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}

						rsq.m_right[k] = quad;
					}
				}
				else if (mp1.rightNum < mp0.rightNum)
				{
					rsq.rightNum = mp0.rightNum;
					rsq.rightSign = mp0.rightsign;

					//rsq.rightNum = mp1.rightNum;
					for (int k = 0; k < mp1.rightNum; k++)
					{
						pp[0] = mp0.rpoints[2 * k];
						pp[1] = mp1.rpoints[2 * k];
						pp[2] = mp1.rpoints[2 * k + 1];
						pp[3] = mp0.rpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastRightLength / length;
						float s1 = curRightLength / length;
						float tl = (pp[3].z() - pp[0].z()) / height;
						float tr = (pp[2].z() - pp[1].z()) / height;
						if (mp0.rightsign == 1)
						{
							tl = (pp[0].z() - pp[3].z()) / height;
							tr = (pp[1].z() - pp[2].z()) / height;
						}
						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = tl;
						quad.text[2].setX(s1); quad.text[2].setY(tl);
						//quad.text[3].s = s0; quad.text[3].t = tr;
						quad.text[3].setX(s0); quad.text[3].setY(tr);
						if (mp0.rightsign == 1)
						{
							//quad.text[0].s = s0; quad.text[0].t = tl;
							quad.text[0].setX(s0);   quad.text[0].setY(tl);
							//quad.text[1].s = s1; quad.text[1].t = tr;
							quad.text[1].setX(s1); quad.text[1].setY(tr);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}
						rsq.m_right[k] = quad;
					}
					QVector3D p = mp1.rpoints[2 * (mp1.rightNum) - 1];
					for (int k = mp1.rightNum; k < mp0.rightNum; k++)
					{
						pp[0] = mp0.rpoints[2 * k];;
						pp[1] = p;
						pp[2] = p;
						pp[3] = mp0.rpoints[2 * k + 1];

						_QUAD quad;
						for (int l = 0; l < 4; l++) quad.cord[l] = pp[l];
						//纹理坐标
						float s0 = lastRightLength / length;
						float s1 = curRightLength / length;

						float z = pp[3].z() - pp[0].z();
						float t = z / 8;
						if (mp0.rightsign == 1) t = -t;

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s1; quad.text[1].t = 0;
						quad.text[1].setX(s1); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = 0;
						quad.text[2].setX(s1); quad.text[2].setY(0);
						//quad.text[3].s = s0; quad.text[3].t = t;
						quad.text[3].setX(s0); quad.text[3].setY(t);

						if (mp0.rightsign == 1)
						{
							//quad.text[0].s = s0; quad.text[0].t = t;
							quad.text[0].setX(s0);   quad.text[0].setY(t);
							//quad.text[1].s = s1; quad.text[1].t = 0;
							quad.text[1].setX(s1); quad.text[1].setY(0);
							//quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[2].setX(s1); quad.text[2].setY(0);
							//quad.text[3].s = s0; quad.text[3].t = 0;
							quad.text[3].setX(s0); quad.text[3].setY(0);
						}
						rsq.m_right[k] = quad;
					}
				}

				if (mp0.rightsign != mp1.rightsign)
				{
					lastLeftLength = 0;
					curLeftLength = 0;
				}
			}

			rsq.pileNo = mp0.pile;
			rsq.pileEndNo = mp1.pile;
			rsq.lefttype = mp0.lefttype;
			rsq.righttype = mp0.righttype;

			m_quad[no].push_back(rsq);
		}
	}
}

//排水沟（挖方为边沟）
void rdRoadSlope::makeDitchQuad(bool bleft)
{
	bool bLDitch;
	bool bRDitch;
	QVector3D lditch[6];
	QVector3D rditch[6];

	//主线部分
	float length = 3.0;//纹理长度
	float height = 4.0f;
	float curLeftLength = 0;
	float curRightLength = 0;
	float lastLeftLength = 0;
	float lastRightLength = 0;

	for (int no = 0; no < 14; no++)
	{
		int size = m_fiPoints2[no].size();
		//if (bleft) size = m_fiLeftPoints2[no].size();
		if (size < 2) continue;

		for (int i = 0; i < size - 1; i++)
		{
			_mp mp0, mp1;

			/*if (bleft)
			{
				mp0 = m_fiLeftPoints2[no][i];
				mp1 = m_fiLeftPoints2[no][i + 1];
			}
			else*/
			{
				mp0 = m_fiPoints2[no][i];
				mp1 = m_fiPoints2[no][i + 1];
			}

			if (mp0.pile == 24275)
				mp0 = mp0;

			_RoadSlopeQuad* prsq = 0;

			/*if (bleft) prsq = &(m_leftquad[no][i]);
			else*/ prsq = &(m_quad[no][i]);
			_RoadSlopeQuad& rsq = *prsq;

			//////////////////////////
			//left
			if (!mp0.bLDitch || !mp1.bLDitch)// 当前没有排水沟
			{
				lastLeftLength = 0;
				curLeftLength = 0;

				rsq.m_bleftDitch = false;
			}
			else // 当前有排水沟
			{
				if (mp1.leftsign == mp0.leftsign)//同是挖方或填方
				{
					lastLeftLength = curLeftLength;
					curLeftLength += (mp1.pile - mp0.pile);
					rsq.m_bleftDitch = true;
				}
				else
				{
					rsq.m_bleftDitch = false;
					lastLeftLength = 0;
					curLeftLength = 0;
				}
			}
			rsq.m_bleftDitch = false;
			if (mp0.bLDitch && mp1.bLDitch && mp1.leftsign == mp0.leftsign)
			{
				QVector3D pp[4];
				float s0 = lastLeftLength / length;
				float s1 = curLeftLength / length;
				rsq.m_bleftDitch = true;

				for (int l = 0; l < 5; l++)
				{
					pp[0] = mp0.lditch[l];
					pp[1] = mp0.lditch[l + 1];
					pp[2] = mp1.lditch[l + 1];
					pp[3] = mp1.lditch[l];

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					//quad.text[0].s = s0; quad.text[0].t = 0;
					//quad.text[1].s = s0; quad.text[1].t = 1;
					//quad.text[2].s = s1; quad.text[2].t = 1;
					//quad.text[3].s = s1; quad.text[3].t = 0;

					quad.text[0].setX(s0); quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_leftDitch[l] = quad;
				}
			}

			//////////////////////////
			//right
			if (!mp0.bRDitch || !mp1.bRDitch)// 当前没有排水沟
			{
				lastRightLength = 0;
				curRightLength = 0;

				rsq.m_brightDitch = false;
			}
			else // 当前有排水沟
			{
				if (mp1.rightsign == mp0.rightsign)
				{
					lastRightLength = curRightLength;
					curRightLength += (mp1.pile - mp0.pile);
					rsq.m_brightDitch = true;
				}
				else
				{
					rsq.m_brightDitch = false;
					lastRightLength = 0;
					curRightLength = 0;
				}
			}
			rsq.m_brightDitch = false;
			if (mp0.bRDitch && mp1.bRDitch && mp1.rightsign == mp0.rightsign)
			{
				QVector3D pp[4];
				float s0 = lastRightLength / length;
				float s1 = curRightLength / length;
				rsq.m_brightDitch = true;
				for (int l = 0; l < 5; l++)
				{
					pp[0] = mp0.rditch[l];
					pp[1] = mp0.rditch[l + 1];
					pp[2] = mp1.rditch[l + 1];
					pp[3] = mp1.rditch[l];

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*quad.text[0].s = s0; quad.text[0].t = 0;
					quad.text[1].s = s0; quad.text[1].t = 1;
					quad.text[2].s = s1; quad.text[2].t = 1;
					quad.text[3].s = s1; quad.text[3].t = 0;*/

					quad.text[0].setX(s0); quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_rightDitch[l] = quad;
				}
			}
		}
	}
}

//护坡道（挖方为碎落台）
void rdRoadSlope::makeRampQuad(bool bleft)
{
	//主线部分
	float length = 3.0;//纹理长度
	float height = 4.0f;
	float curLeftLength = 0;
	float curRightLength = 0;
	float lastLeftLength = 0;
	float lastRightLength = 0;

	for (int no = 0; no < 14; no++)
	{
		/*if (bleft)
			if (m_fiLeftPoints2[no].size() < 2) continue;
			else*/
				if (m_fiPoints2[no].size() < 2) continue;

		int  size = m_fiPoints2[no].size() - 1;
		//if (bleft) size = m_fiLeftPoints2[no].size() - 1;

		for (int i = 0; i < size; i++)
		{
			_mp mp0;
			_mp mp1;
			/*if (bleft)
			{
				mp0 = m_fiLeftPoints2[no][i];
				mp1 = m_fiLeftPoints2[no][i + 1];
			}
			else*/
			{
				mp0 = m_fiPoints2[no][i];
				mp1 = m_fiPoints2[no][i + 1];
			}

			if (mp0.pile == 12130)
				mp0 = mp0;

			_RoadSlopeQuad* prsq = 0;
			/*if (bleft)
				prsq = &(m_leftquad[no][i]);
			else*/
				prsq = &(m_quad[no][i]);

			_RoadSlopeQuad& rsq = *prsq;
			//////////////////////////
			//left
			if (!mp0.bLRamp || !mp1.bLRamp)// 当前没有护坡道（挖方为碎落台）
			{
				lastLeftLength = 0;
				curLeftLength = 0;

				rsq.m_bleftRamp = false;
			}
			else // 当前有排水沟
			{
				if (mp1.leftsign == mp0.leftsign)
				{
					lastLeftLength = curLeftLength;
					curLeftLength += (mp1.pile - mp0.pile);
					rsq.m_bleftRamp = true;
				}
				else
				{
					rsq.m_bleftRamp = false;
					lastLeftLength = 0;
					curLeftLength = 0;
				}
			}

			rsq.m_bleftRamp = false;
			if (mp0.bLRamp && mp1.bLRamp && mp1.leftsign == mp0.leftsign)
			{
				QVector3D pp[4];
				float s0 = lastLeftLength / length;
				float s1 = curLeftLength / length;
				rsq.m_bleftRamp = true;
				//for (int l = 0; l < 5; l++)
				{
					pp[0] = mp0.lramp[0];
					pp[1] = mp0.lramp[1];
					pp[2] = mp1.lramp[1];
					pp[3] = mp1.lramp[0];

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					//quad.text[0].s = s0; quad.text[0].t = 0;
					//quad.text[1].s = s0; quad.text[1].t = 1;
					//quad.text[2].s = s1; quad.text[2].t = 1;
					//quad.text[3].s = s1; quad.text[3].t = 0;

					quad.text[0].setX(s0); quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_leftRamp = quad;
				}
			}

			//////////////////////////
			//right
			if (!mp0.bRRamp || !mp1.bRRamp)// 当前没有排水沟
			{
				lastRightLength = 0;
				curRightLength = 0;

				rsq.m_brightRamp = false;
			}
			else // 当前有排水沟
			{
				if (mp1.rightsign == mp0.rightsign)//同填方或挖分
				{
					lastRightLength = curRightLength;
					curRightLength += (mp1.pile - mp0.pile);
					rsq.m_brightRamp = true;
				}
				else
				{
					rsq.m_brightRamp = false;
					lastRightLength = 0;
					curRightLength = 0;
				}
			}
			rsq.m_brightRamp = false;
			if (mp0.bRRamp && mp1.bRRamp && mp1.rightsign == mp0.rightsign)
			{
				QVector3D pp[4];
				float s0 = lastRightLength / length;
				float s1 = curRightLength / length;
				rsq.m_brightRamp = true;
				//	for (int l = 0; l < 5; l++)
				{
					pp[0] = mp0.rramp[0];
					pp[1] = mp0.rramp[1];
					pp[2] = mp1.rramp[1];
					pp[3] = mp1.rramp[0];

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*quad.text[0].s = s0; quad.text[0].t = 0;
					quad.text[1].s = s0; quad.text[1].t = 1;
					quad.text[2].s = s1; quad.text[2].t = 1;
					quad.text[3].s = s1; quad.text[3].t = 0;*/

					quad.text[0].setX(s0); quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_rightRamp = quad;
				}
			}
		}
	}
}

void rdRoadSlope::makePlatformQuad(bool bleft)
{
	//主线部分
	float length = 3.0;//纹理长度
	float height = 4.0f;
	float curLeftLength = 0;
	float curRightLength = 0;
	float lastLeftLength = 0;
	float lastRightLength = 0;

	for (int no = 0; no < 14; no++)
	{
		int size = m_fiPoints2[no].size();
		//if (bleft) size = m_fiLeftPoints2[no].size();
		if (size < 2) continue;

		for (int i = 0; i < size - 1; i++)
		{
			_mp mp0, mp1;
			/*if (bleft)
			{
				mp0 = m_fiLeftPoints2[no][i];
				mp1 = m_fiLeftPoints2[no][i + 1];
			}
			else*/
			{
				mp0 = m_fiPoints2[no][i];
				mp1 = m_fiPoints2[no][i + 1];
			}

			if (mp0.pile == 90510)
				mp0 = mp0;

			_RoadSlopeQuad* prsq = 0;
			/*if (bleft)
				prsq = &(m_leftquad[no][i]);
			else*/
				prsq = &(m_quad[no][i]);
			_RoadSlopeQuad& rsq = *prsq;

			//////////////////////////
			//left
			rsq.m_iLeftPlatform = 0;
			rsq.m_iLeftPlatformDitch = 0;

			int  maxc = mp0.iLplatform;
			if (mp1.iLplatform > maxc) maxc = mp1.iLplatform;

			rsq.m_iLeftPlatform = maxc;
			if (maxc > 0)// 当前有平台
			{
				lastLeftLength = curLeftLength;
				curLeftLength += (mp1.pile - mp0.pile);
			}
			else //当前没有平台
			{
				lastLeftLength = 0;
				curLeftLength = 0;
			}

			if (rsq.m_iLeftPlatform > 0)
			{
				if (mp0.leftsign == 1 && mp1.leftsign == 2)
					rsq.m_bLFillToExcave = true;

				if (mp0.leftsign == 1 && !rsq.m_bLFillToExcave)//填方
				{
					QVector3D pp[4];
					float s0 = lastLeftLength / length;
					float s1 = curLeftLength / length;

					pp[0] = mp0.lplatform[0];
					pp[1] = mp0.lplatform[1];
					pp[2] = mp1.lplatform[1];
					pp[3] = mp1.lplatform[0];

					if (mp1.iLplatform == 0)//后桩号没有平台
					{
						int  sss = mp1.leftNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[2] = mp1.lpoints[2 * sss - 1];
							pp[3] = mp1.lpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[2] = mp1.lsurface[1];
							pp[3] = mp1.lsurface[1];
						}
					}

					if (mp0.iLplatform == 0)//前桩号没有平台
					{
						int  sss = mp0.leftNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[0] = mp0.lpoints[2 * sss - 1];
							pp[1] = mp0.lpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[0] = mp0.lsurface[1];
							pp[1] = mp0.lsurface[1];
						}
					}

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*quad.text[0].s = s0; quad.text[0].t = 0;
					quad.text[1].s = s0; quad.text[1].t = 1;
					quad.text[2].s = s1; quad.text[2].t = 1;
					quad.text[3].s = s1; quad.text[3].t = 0;*/
					quad.text[0].setX(s0); quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_leftPlatform[0] = quad;
				}
				else if (mp0.leftsign == 2 || rsq.m_bLFillToExcave)//挖方
				{
					QVector3D pp[4];
					float s0 = lastLeftLength / length;
					float s1 = curLeftLength / length;

					int count = mp0.iLplatform;
					if (mp1.iLplatform < count) count = mp1.iLplatform;

					//排水沟里侧的平台
					for (int ss = 0; ss < count; ss++)
					{
						pp[0] = mp0.lplatform[4 * ss];
						pp[1] = mp0.lplatform[4 * ss + 1];
						pp[2] = mp1.lplatform[4 * ss + 1];
						pp[3] = mp1.lplatform[4 * ss];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						//quad.text[0].s = s0; quad.text[0].t = 0;
						//quad.text[1].s = s0; quad.text[1].t = 1;
						//quad.text[2].s = s1; quad.text[2].t = 1;
						//quad.text[3].s = s1; quad.text[3].t = 0;
						quad.text[0].setX(s0); quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_leftPlatform[2 * ss] = quad;
					}

					if (mp0.iLplatform > count)
					{
						int sSlope = mp1.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.lpoints[2 * sSlope - 1];
						else  ppp = mp1.lsurface[1];

						for (int ss = count; ss < mp0.iLplatform; ss++)
						{
							pp[0] = mp0.lplatform[4 * ss];
							pp[1] = mp0.lplatform[4 * ss + 1];

							pp[2] = ppp; //mp1.lplatform[4 * ss + 1];
							pp[3] = ppp;// mp1.lplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/

							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatform[2 * ss] = quad;
						}
					}
					if (mp1.iLplatform > count)
					{
						int sSlope = mp0.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.lpoints[2 * sSlope - 1];
						else  ppp = mp0.lsurface[1];

						for (int ss = count; ss < mp1.iLplatform; ss++)
						{
							pp[0] = ppp;
							pp[1] = ppp;

							pp[2] = mp1.lplatform[4 * ss + 1];
							pp[3] = mp1.lplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/

							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatform[2 * ss] = quad;
						}
					}
					//排水沟外侧的平台
					for (int ss = 0; ss < count; ss++)
					{
						pp[0] = mp0.lplatform[4 * ss + 2];
						pp[1] = mp0.lplatform[4 * ss + 3];
						pp[2] = mp1.lplatform[4 * ss + 3];
						pp[3] = mp1.lplatform[4 * ss + 2];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/

						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_leftPlatform[2 * ss + 1] = quad;
					}

					if (mp0.iLplatform > count)
					{
						int sSlope = mp1.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.lpoints[2 * sSlope - 1];
						else  ppp = mp1.lsurface[1];

						for (int ss = count; ss < mp0.iLplatform; ss++)
						{
							pp[0] = mp0.lplatform[4 * ss + 2];
							pp[1] = mp0.lplatform[4 * ss + 3];

							pp[2] = ppp; //mp1.lplatform[4 * ss + 1];
							pp[3] = ppp;// mp1.lplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/

							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatform[2 * ss + 1] = quad;
						}
					}
					if (mp1.iLplatform > count)
					{
						int sSlope = mp0.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.lpoints[2 * sSlope - 1];
						else  ppp = mp0.lsurface[1];

						for (int ss = count; ss < mp1.iLplatform; ss++)
						{
							pp[0] = ppp;
							pp[1] = ppp;

							pp[2] = mp1.lplatform[4 * ss + 2];
							pp[3] = mp1.lplatform[4 * ss + 3];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/

							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatform[2 * ss + 1] = quad;
						}
					}

					//排水沟
					float ditchheight = 0.6f;
					//里侧面		
					rsq.m_iLeftPlatformDitch = maxc;

					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p0 = mp0.lplatformditch[2 * ss];
						QVector3D p1 = mp1.lplatformditch[2 * ss];
						//p0.z -= ditchheight;
						p0.setZ(p0.z() - ditchheight);
						//p1.z -= ditchheight;
						p1.setZ(p1.z() - ditchheight);

						pp[0] = mp0.lplatformditch[2 * ss];
						pp[1] = p0;
						pp[2] = p1;
						pp[3] = mp1.lplatformditch[2 * ss];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						//quad.text[0].s = s0; quad.text[0].t = 0;
						//quad.text[1].s = s0; quad.text[1].t = 1;
						//quad.text[2].s = s1; quad.text[2].t = 1;
						//quad.text[3].s = s1; quad.text[3].t = 0;

						quad.text[0].setX(s0); quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_leftPlatformDitch[3 * ss] = quad;
					}

					if (mp0.iLplatform > count)
					{
						int sSlope = mp1.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.lpoints[2 * sSlope - 1];
						else  ppp = mp1.lsurface[1];

						for (int ss = count; ss < mp0.iLplatform; ss++)
						{
							QVector3D p0 = mp0.lplatformditch[2 * ss];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = mp0.lplatformditch[2 * ss];
							pp[1] = p0;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*	quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/

							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatformDitch[3 * ss] = quad;
						}
					}
					if (mp1.iLplatform > count)
					{
						int sSlope = mp0.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.lpoints[2 * sSlope - 1];
						else  ppp = mp0.lsurface[1];

						for (int ss = count; ss < mp1.iLplatform; ss++)
						{
							QVector3D p0 = mp1.lplatformditch[2 * ss];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p0;
							pp[3] = mp1.lplatformditch[2 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);							
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_leftPlatformDitch[3 * ss] = quad;
						}
					}
					//底面
					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p00 = mp0.lplatformditch[2 * ss];
						QVector3D p01 = mp0.lplatformditch[2 * ss + 1];
						QVector3D p10 = mp1.lplatformditch[2 * ss + 1];
						QVector3D p11 = mp1.lplatformditch[2 * ss];
						//p00.z -= ditchheight;
						p00.setZ(p00.z() - ditchheight);

						//p01.z -= ditchheight;
						p01.setZ(p01.z() - ditchheight);
						//p10.z -= ditchheight;
						p10.setZ(p10.z() - ditchheight);
						//p11.z -= ditchheight;
						p11.setZ(p11.z() - ditchheight);

						pp[0] = p00;
						pp[1] = p01;
						pp[2] = p10;
						pp[3] = p11;

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						//quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						//quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[1].setX(s0); quad.text[1].setY(0);
						//quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[2].setX(s1); quad.text[2].setY(1);
						//quad.text[3].s = s1; quad.text[3].t = 0;
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_leftPlatformDitch[3 * ss + 1] = quad;

					}
					if (mp0.iLplatform > count)
					{
						int sSlope = mp1.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.lpoints[2 * sSlope - 1];
						else  ppp = mp1.lsurface[1];

						for (int ss = count; ss < mp0.iLplatform; ss++)
						{
							QVector3D p00 = mp0.lplatformditch[2 * ss];
							QVector3D p01 = mp0.lplatformditch[2 * ss + 1];
							//p00.z -= ditchheight;
							p00.setZ(p00.z() - ditchheight);
							//p01.z -= ditchheight;
							p01.setZ(p01.z() - ditchheight);

							pp[0] = p00;
							pp[1] = p01;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatformDitch[3 * ss + 1] = quad;
						}
					}
					if (mp1.iLplatform > count)
					{
						int sSlope = mp0.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.lpoints[2 * sSlope - 1];
						else  ppp = mp0.lsurface[1];

						for (int ss = count; ss < mp1.iLplatform; ss++)
						{
							QVector3D p10 = mp1.lplatformditch[2 * ss + 1];
							QVector3D p11 = mp1.lplatformditch[2 * ss];

							//p10.z -= ditchheight;
							p10.setZ(p10.z() - ditchheight);
							//p11.z -= ditchheight;
							p11.setZ(p11.z() - ditchheight);

							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p10;
							pp[3] = p11;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_leftPlatformDitch[3 * ss + 1] = quad;
						}
					}
					//外侧面
					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p0 = mp0.lplatformditch[2 * ss + 1];
						QVector3D p1 = mp1.lplatformditch[2 * ss + 1];
						//p0.z -= ditchheight;
						p0.setZ(p0.z() - ditchheight);
						//p1.z -= ditchheight;
						p1.setZ(p1.z() - ditchheight);

						pp[0] = p0;
						pp[1] = mp0.lplatformditch[2 * ss + 1];
						pp[2] = mp1.lplatformditch[2 * ss + 1];
						pp[3] = p1;

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_leftPlatformDitch[3 * ss + 2] = quad;
					}

					if (mp0.iLplatform > count)
					{
						int sSlope = mp1.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.lpoints[2 * sSlope - 1];
						else  ppp = mp1.lsurface[1];

						for (int ss = count; ss < mp0.iLplatform; ss++)
						{
							QVector3D p0 = mp0.lplatformditch[2 * ss + 1];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = mp0.lplatformditch[2 * ss + 1];
							pp[1] = p0;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_leftPlatformDitch[3 * ss + 2] = quad;
						}
					}
					if (mp1.iLplatform > count)
					{
						int sSlope = mp0.leftNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.lpoints[2 * sSlope - 1];
						else  ppp = mp0.lsurface[1];

						for (int ss = count; ss < mp1.iLplatform; ss++)
						{
							QVector3D p0 = mp1.lplatformditch[2 * ss + 1];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p0;
							pp[3] = mp1.lplatformditch[2 * ss + 1];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_leftPlatformDitch[3 * ss + 2] = quad;
						}
					}
				}
			}

			//////////////////////////
			//right
			rsq.m_iRightPlatform = 0;
			rsq.m_iRightPlatformDitch = 0;

			maxc = mp0.iRplatform;
			if (mp1.iRplatform > maxc) maxc = mp1.iRplatform;

			rsq.m_iRightPlatform = maxc;

			if (maxc > 0)// 当前没有平台
			{
				lastRightLength = curRightLength;
				curRightLength += (mp1.pile - mp0.pile);
			}
			else // 当前有平台
			{
				lastRightLength = 0;
				curRightLength = 0;
			}

			//if (mp0.pile == 90452)
			//		continue;
			//if (mp0.pile == 90627)
			//		continue;
			if (mp0.pile == 90615)
				maxc = maxc;

			if (rsq.m_iRightPlatform > 0)
			{
				if (mp0.rightsign == 1 && mp1.rightsign == 2)
					rsq.m_bRFillToExcave = true;

				if (mp0.rightsign == 1 && !rsq.m_bRFillToExcave)//填方
				{
					QVector3D pp[4];
					float s0 = lastRightLength / length;
					float s1 = curRightLength / length;

					pp[0] = mp0.rplatform[0];
					pp[1] = mp0.rplatform[1];
					pp[2] = mp1.rplatform[1];
					pp[3] = mp1.rplatform[0];

					if (mp1.iRplatform == 0)//后桩号没有平台
					{
						int  sss = mp1.rightNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[2] = mp1.rpoints[2 * sss - 1];
							pp[3] = mp1.rpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[2] = mp1.rsurface[1];
							pp[3] = mp1.rsurface[1];
						}
					}

					if (mp0.iRplatform == 0)//前桩号没有平台
					{
						int  sss = mp0.rightNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[0] = mp0.rpoints[2 * sss - 1];
							pp[1] = mp0.rpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[0] = mp0.rsurface[1];
							pp[1] = mp0.rsurface[1];
						}
					}

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*quad.text[0].s = s0; quad.text[0].t = 0;
					quad.text[1].s = s0; quad.text[1].t = 1;
					quad.text[2].s = s1; quad.text[2].t = 1;
					quad.text[3].s = s1; quad.text[3].t = 0;*/
					quad.text[0].setX(s0);   quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_rightPlatform[0] = quad;
				}
				if (mp0.rightsign == 1 && rsq.m_bRFillToExcave)//填方
				{
					QVector3D pp[4];
					float s0 = lastRightLength / length;
					float s1 = curRightLength / length;

					pp[0] = mp0.rplatform[0];
					pp[1] = mp0.rplatform[1];
					pp[2] = mp1.rplatform[1];
					pp[3] = mp1.rplatform[0];

					if (mp1.iRplatform == 0)//后桩号没有平台
					{
						int  sss = mp1.rightNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[2] = mp1.rpoints[2 * sss - 1];
							pp[3] = mp1.rpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[2] = mp1.rsurface[1];
							pp[3] = mp1.rsurface[1];
						}
					}

					if (mp0.iRplatform == 0)//前桩号没有平台
					{
						int  sss = mp0.rightNum;

						if (sss > 0) //连接到护坡最低点
						{
							pp[0] = mp0.rpoints[2 * sss - 1];
							pp[1] = mp0.rpoints[2 * sss - 1];
						}
						else //连接到路面点
						{
							pp[0] = mp0.rsurface[1];
							pp[1] = mp0.rsurface[1];
						}
					}

					_QUAD quad;
					for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*quad.text[0].s = s0; quad.text[0].t = 0;
					quad.text[1].s = s0; quad.text[1].t = 1;
					quad.text[2].s = s1; quad.text[2].t = 1;
					quad.text[3].s = s1; quad.text[3].t = 0;*/
					quad.text[0].setX(s0);   quad.text[0].setY(0);
					quad.text[1].setX(s0); quad.text[1].setY(1);
					quad.text[2].setX(s1); quad.text[2].setY(1);
					quad.text[3].setX(s1); quad.text[3].setY(0);

					rsq.m_rightPlatform[0] = quad;
				}
				else if (mp0.rightsign == 2 || rsq.m_bRFillToExcave)
				{
					QVector3D pp[4];
					float s0 = lastRightLength / length;
					float s1 = curRightLength / length;

					int count = mp0.iRplatform;

					//if (mp0.pile == 90452)
					//	continue;
					//if (mp0.pile == 90627)
					//	continue;

					if (mp1.iRplatform < count) count = mp1.iRplatform;

					//排水沟里侧的平台
					for (int ss = 0; ss < count; ss++)
					{
						pp[0] = mp0.rplatform[4 * ss];
						pp[1] = mp0.rplatform[4 * ss + 1];
						pp[2] = mp1.rplatform[4 * ss + 1];
						pp[3] = mp1.rplatform[4 * ss];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_rightPlatform[2 * ss] = quad;
					}

					if (mp0.iRplatform > count)
					{
						int sSlope = mp1.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.rpoints[2 * sSlope - 1];
						else  ppp = mp1.rsurface[1];

						for (int ss = count; ss < mp0.iRplatform; ss++)
						{
							pp[0] = mp0.rplatform[4 * ss];
							pp[1] = mp0.rplatform[4 * ss + 1];

							pp[2] = ppp; //mp1.lplatform[4 * ss + 1];
							pp[3] = ppp;// mp1.lplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0); quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatform[2 * ss] = quad;
						}
					}
					if (mp1.iRplatform > count)
					{
						int sSlope = mp0.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.rpoints[2 * sSlope - 1];
						else  ppp = mp0.rsurface[1];

						for (int ss = count; ss < mp1.iRplatform; ss++)
						{
							pp[0] = ppp;
							pp[1] = ppp;

							pp[2] = mp1.rplatform[4 * ss + 1];
							pp[3] = mp1.rplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatform[2 * ss] = quad;
						}
					}
					//排水沟外侧的平台
					for (int ss = 0; ss < count; ss++)
					{
						pp[0] = mp0.rplatform[4 * ss + 2];
						pp[1] = mp0.rplatform[4 * ss + 3];
						pp[2] = mp1.rplatform[4 * ss + 3];
						pp[3] = mp1.rplatform[4 * ss + 2];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_rightPlatform[2 * ss + 1] = quad;
					}

					if (mp0.iRplatform > count)
					{
						int sSlope = mp1.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.rpoints[2 * sSlope - 1];
						else  ppp = mp1.rsurface[1];

						for (int ss = count; ss < mp0.iRplatform; ss++)
						{
							pp[0] = mp0.rplatform[4 * ss + 2];
							pp[1] = mp0.rplatform[4 * ss + 3];

							pp[2] = ppp; //mp1.lplatform[4 * ss + 1];
							pp[3] = ppp;// mp1.lplatform[4 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatform[2 * ss + 1] = quad;
						}
					}
					if (mp1.iRplatform > count)
					{
						int sSlope = mp0.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.rpoints[2 * sSlope - 1];
						else  ppp = mp0.rsurface[1];

						for (int ss = count; ss < mp1.iRplatform; ss++)
						{
							pp[0] = ppp;
							pp[1] = ppp;

							pp[2] = mp1.rplatform[4 * ss + 2];
							pp[3] = mp1.rplatform[4 * ss + 3];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 1;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(1);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatform[2 * ss + 1] = quad;
						}
					}

					//排水沟
					float ditchheight = 0.6f;
					//里侧面
					rsq.m_iRightPlatformDitch = maxc;
					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p0 = mp0.rplatformditch[2 * ss];
						QVector3D p1 = mp1.rplatformditch[2 * ss];
						//p0.z -= ditchheight;
						p0.setZ(p0.z() - ditchheight);
						//p1.z -= ditchheight;
						p1.setZ(p1.z() - ditchheight);

						pp[0] = mp0.rplatformditch[2 * ss];
						pp[1] = p0;
						pp[2] = p1;
						pp[3] = mp1.rplatformditch[2 * ss];

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_rightPlatformDitch[3 * ss] = quad;
					}

					if (mp0.iRplatform > count)
					{
						int sSlope = mp1.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.rpoints[2 * sSlope - 1];
						else  ppp = mp1.rsurface[1];

						for (int ss = count; ss < mp0.iRplatform; ss++)
						{
							QVector3D p0 = mp0.rplatformditch[2 * ss];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = mp0.rplatformditch[2 * ss];
							pp[1] = p0;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatformDitch[3 * ss] = quad;
						}
					}
					if (mp1.iRplatform > count)
					{
						int sSlope = mp0.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.rpoints[2 * sSlope - 1];
						else  ppp = mp0.rsurface[1];

						for (int ss = count; ss < mp1.iRplatform; ss++)
						{
							QVector3D p0 = mp1.rplatformditch[2 * ss];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p0;
							pp[3] = mp1.rplatformditch[2 * ss];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_rightPlatformDitch[3 * ss] = quad;
						}
					}
					//底面
					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p00 = mp0.rplatformditch[2 * ss];
						QVector3D p01 = mp0.rplatformditch[2 * ss + 1];
						QVector3D p10 = mp1.rplatformditch[2 * ss + 1];
						QVector3D p11 = mp1.rplatformditch[2 * ss];
						//p00.z -= ditchheight;
						p00.setZ(p00.z() - ditchheight);
						//p01.z -= ditchheight;
						p01.setZ(p01.z() - ditchheight);
						//p10.z -= ditchheight;
						p10.setZ(p10.z() - ditchheight);
						//p11.z -= ditchheight;
						p11.setZ(p11.z() - ditchheight);

						pp[0] = p00;
						pp[1] = p01;
						pp[2] = p10;
						pp[3] = p11;

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

					/*	quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_rightPlatformDitch[3 * ss + 1] = quad;

					}
					if (mp0.iRplatform > count)
					{
						int sSlope = mp1.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.rpoints[2 * sSlope - 1];
						else  ppp = mp1.rsurface[1];

						for (int ss = count; ss < mp0.iRplatform; ss++)
						{
							QVector3D p00 = mp0.rplatformditch[2 * ss];
							QVector3D p01 = mp0.rplatformditch[2 * ss + 1];
							//p00.z -= ditchheight;
							p00.setZ(p00.z() - ditchheight);
							//p01.z -= ditchheight;
							p01.setZ(p01.z() - ditchheight);

							pp[0] = p00;
							pp[1] = p01;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatformDitch[3 * ss + 1] = quad;
						}
					}
					if (mp1.iRplatform > count)
					{
						int sSlope = mp0.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.rpoints[2 * sSlope - 1];
						else  ppp = mp0.rsurface[1];

						for (int ss = count; ss < mp1.iRplatform; ss++)
						{
							QVector3D p10 = mp1.rplatformditch[2 * ss + 1];
							QVector3D p11 = mp1.rplatformditch[2 * ss];

							//p10.z -= ditchheight;
							p10.setZ(p10.z() - ditchheight);
							//p11.z -= ditchheight;
							p11.setZ(p11.z() - ditchheight);
							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p10;
							pp[3] = p11;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_rightPlatformDitch[3 * ss + 1] = quad;
						}
					}
					//外侧面
					for (int ss = 0; ss < count; ss++)
					{
						QVector3D p0 = mp0.rplatformditch[2 * ss + 1];
						QVector3D p1 = mp1.rplatformditch[2 * ss + 1];
						//p0.z -= ditchheight;
						p0.setZ(p0.z() - ditchheight);
						//p1.z -= ditchheight;
						p1.setZ(p1.z() - ditchheight);

						pp[0] = p0;
						pp[1] = mp0.rplatformditch[2 * ss + 1];
						pp[2] = mp1.rplatformditch[2 * ss + 1];
						pp[3] = p1;

						_QUAD quad;
						for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

						/*quad.text[0].s = s0; quad.text[0].t = 0;
						quad.text[1].s = s0; quad.text[1].t = 1;
						quad.text[2].s = s1; quad.text[2].t = 1;
						quad.text[3].s = s1; quad.text[3].t = 0;*/
						quad.text[0].setX(s0);   quad.text[0].setY(0);
						quad.text[1].setX(s0); quad.text[1].setY(1);
						quad.text[2].setX(s1); quad.text[2].setY(1);
						quad.text[3].setX(s1); quad.text[3].setY(0);

						rsq.m_rightPlatformDitch[3 * ss + 2] = quad;
					}

					if (mp0.iRplatform > count)
					{
						int sSlope = mp1.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp1.rpoints[2 * sSlope - 1];
						else  ppp = mp1.rsurface[1];

						for (int ss = count; ss < mp0.iRplatform; ss++)
						{
							QVector3D p0 = mp0.rplatformditch[2 * ss + 1];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = mp0.rplatformditch[2 * ss + 1];
							pp[1] = p0;
							pp[2] = ppp;
							pp[3] = ppp;

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 1;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 0;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(1);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(0);

							rsq.m_rightPlatformDitch[3 * ss + 2] = quad;
						}
					}
					if (mp1.iRplatform > count)
					{
						int sSlope = mp0.rightNum;
						QVector3D ppp;
						if (sSlope > 0) ppp = mp0.rpoints[2 * sSlope - 1];
						else  ppp = mp0.rsurface[1];

						for (int ss = count; ss < mp1.iRplatform; ss++)
						{
							QVector3D p0 = mp1.rplatformditch[2 * ss + 1];
							//p0.z -= ditchheight;
							p0.setZ(p0.z() - ditchheight);

							pp[0] = ppp;
							pp[1] = ppp;
							pp[2] = p0;
							pp[3] = mp1.rplatformditch[2 * ss + 1];

							_QUAD quad;
							for (int lm = 0; lm < 4; lm++) quad.cord[lm] = pp[lm];

							/*quad.text[0].s = s0; quad.text[0].t = 0;
							quad.text[1].s = s0; quad.text[1].t = 0;
							quad.text[2].s = s1; quad.text[2].t = 0;
							quad.text[3].s = s1; quad.text[3].t = 1;*/
							quad.text[0].setX(s0);   quad.text[0].setY(0);
							quad.text[1].setX(s0); quad.text[1].setY(0);
							quad.text[2].setX(s1); quad.text[2].setY(0);
							quad.text[3].setX(s1); quad.text[3].setY(1);

							rsq.m_rightPlatformDitch[3 * ss + 2] = quad;
						}
					}
				}
			}
		}
	}
}

void rdRoadSlope::createVBO(bool bleft)
{
	initializeOpenGLFunctions();
	//主线部分
	QVector<_RoadSlopeQuad>* pquad = m_quad;
	//if (bleft) pquad = m_leftquad;
	int* pcount = m_quadCountSurface;
	//if (bleft) pcount = m_quadCountLeftSurface;
	//unsigned int* vaohandle = vaoHandleSurface;
	//if (bleft)vaohandle = vaoHandleLeftSurface;
	//unsigned int* vbohandle = vboHandleSurface;
	//if (bleft)vbohandle = vboHandleLeftSurface;
	for (int no = 0; no < 1; no++)
	{
		pcount[no] = 0;
		if (pquad[no].size() < 1) continue;
		//路面
		QVector3D* pVerts = 0;
		//v = new float[4 *3 * pquad[no].size() * 2];
		pVerts = new QVector3D[4 *  pquad[no].size() * 2];
		int cv = 0;
		for (int i = 0; i < pquad[no].size(); i++)
		{
			_RoadSlopeQuad& rsq = pquad[no][i];
			for (int l = 0; l < 4; l++)
			{
				pVerts[cv++] = QVector3D(rsq.m_leftsurface.cord[l].x(), rsq.m_leftsurface.cord[l].y(),
					                rsq.m_leftsurface.cord[l].z());
			//	v[cv++] = ;
			//	v[cv++] = rsq.m_leftsurface.cord[l].z();
			}
			for (int l = 0; l < 4; l++)
			{
				pVerts[cv++] = QVector3D(rsq.m_rightsurface.cord[l].x(), rsq.m_rightsurface.cord[l].y(),
					rsq.m_rightsurface.cord[l].z());
				/*v[cv++] = rsq.m_rightsurface.cord[l].x();
				v[cv++] = rsq.m_rightsurface.cord[l].y();
				v[cv++] = rsq.m_rightsurface.cord[l].z();*/
			}
		}
		pcount[no] = 4 * pquad[no].size() * 2;

		surfaceVertsBuf[no].create();
		surfaceVertsBuf[no].bind();
		surfaceVertsBuf[no].allocate(pVerts, pcount[no] * sizeof(QVector3D));


		//护坡
		delete[] pVerts;

		int sfill[8];
		int sexcave[8];
		for (int i = 0; i < 8; i++)
		{
			sfill[i] = 0;
			sexcave[i] = 0;
		}

		for (int i = 0; i < pquad[no].size(); i++)
		{
			_RoadSlopeQuad& rsq = pquad[no][i];

			for (int k = 0; k < rsq.leftNum; k++)
			{
				if (bleft && (rsq.pileNo == 32244)) continue;

				if (rsq.leftSign == 1) sfill[k]++;//左幅填方
				if (rsq.leftSign == 2) sexcave[k]++;//左幅挖方

			}
			for (int k = 0; k < rsq.rightNum; k++)
			{
				if (rsq.rightSign == 1) sfill[k]++;//右幅填方
				if (rsq.rightSign == 2) sexcave[k]++;//右幅挖方
			}
		}

		for (int j = 0; j < 8; j++)
		{
			//填方
			QVector3D* pVerts = 0;
			QVector3D* pNormal = 0;
			QVector2D* pTex = 0;
			//挖方
			QVector3D* pVerts2 = 0;
			QVector3D* pNormal2 = 0;
			QVector2D* pTex2 = 0;

			if (sfill[j] > 0)
			{
				/*v = new float[4 * 3 * sfill[j]];
				n = new float[4 * 3 * sfill[j]];
				tex = new float[4 * 2 * sfill[j]];*/
				pVerts = new QVector3D[4 * sfill[j]];
				pNormal = new QVector3D[4 * sfill[j]];
				pTex = new QVector2D[4 * sfill[j]];

			}
			if (sexcave[j] > 0)
			{
				/*v2 = new float[4 * 3 * sexcave[j]];
				n2 = new float[4 * 3 * sexcave[j]];
				tex2 = new float[4 * 2 * sexcave[j]];*/
				pVerts2 = new QVector3D[4 * sexcave[j]];
				pNormal2 = new QVector3D[4 * sexcave[j]];
				pTex2 = new QVector2D[4 * sexcave[j]];
			}
			int countv = 0;
			int countn = 0;
			int countt = 0;

			int countv2 = 0;//填方
			int countn2 = 0;
			int countt2 = 0;
			for (int i = 0; i < pquad[no].size(); i++)
			{
				_RoadSlopeQuad& rsq = pquad[no][i];

				if (rsq.pileNo == 28598)
					countv2 = countv2;

				if (rsq.leftNum >= (j + 1))//左幅
				{
					if (!(bleft && (rsq.pileNo == 32244)))
					{
						if (rsq.leftSign == 1)
						{
							for (int l = 0; l < 4; l++)
							{
								//v[countv++] = rsq.m_left[j].cord[l].x; v[countv++] = rsq.m_left[j].cord[l].y; v[countv++] = rsq.m_left[j].cord[l].z;
								pVerts[countv++] = QVector3D(rsq.m_left[j].cord[l].x(),rsq.m_left[j].cord[l].y(), rsq.m_left[j].cord[l].z());
 								//n[countn++] = rsq.m_left[j].normal.x; n[countn++] = rsq.m_left[j].normal.y; n[countn++] = rsq.m_left[j].normal.z;
								pNormal[countn++] = QVector3D(rsq.m_left[j].normal.x(), rsq.m_left[j].normal.y(), rsq.m_left[j].normal.z());
								//tex[countt++] = rsq.m_left[j].text[l].x; tex[countt++] = rsq.m_left[j].text[l].y;
								pTex[countt++] = QVector2D(rsq.m_left[j].text[l].x(), rsq.m_left[j].text[l].y());
							}
						}
						else if (rsq.leftSign == 2)
						{
							for (int l = 0; l < 4; l++)
							{
								/*v2[countv2++] = rsq.m_left[j].cord[l].x; v2[countv2++] = rsq.m_left[j].cord[l].y; v2[countv2++] = rsq.m_left[j].cord[l].z;
								n2[countn2++] = rsq.m_left[j].normal.x; n2[countn2++] = rsq.m_left[j].normal.y; n2[countn2++] = rsq.m_left[j].normal.z;
								tex2[countt2++] = rsq.m_left[j].text[l].x; tex2[countt2++] = rsq.m_left[j].text[l].y;*/
								pVerts2[countv2++] = QVector3D(rsq.m_left[j].cord[l].x(), rsq.m_left[j].cord[l].y(), rsq.m_left[j].cord[l].z());
								pNormal2[countn2++] = QVector3D(rsq.m_left[j].normal.x(), rsq.m_left[j].normal.y(), rsq.m_left[j].normal.z());
								pTex2[countt2++] = QVector2D(rsq.m_left[j].text[l].x(), rsq.m_left[j].text[l].y());
							}
						}
					}
				}


				if (rsq.rightNum >= (j + 1))//右幅
				{
					if (rsq.rightSign == 1)
					{
						for (int l = 0; l < 4; l++)
						{
							//v[countv++] = rsq.m_right[j].cord[l].x; v[countv++] = rsq.m_right[j].cord[l].y; v[countv++] = rsq.m_right[j].cord[l].z;
							pVerts[countv++] = QVector3D(rsq.m_right[j].cord[l].x(), rsq.m_right[j].cord[l].y(), rsq.m_right[j].cord[l].z());
							//n[countn++] = rsq.m_right[j].normal.x; n[countn++] = rsq.m_right[j].normal.y; n[countn++] = rsq.m_right[j].normal.z;
							pNormal[countn++] = QVector3D(rsq.m_right[j].normal.x(), rsq.m_right[j].normal.y(), rsq.m_right[j].normal.z());
							//tex[countt++] = rsq.m_right[j].text[l].x; tex[countt++] = rsq.m_right[j].text[l].y;
							pTex[countt++] = QVector2D(rsq.m_right[j].text[l].x(), rsq.m_right[j].text[l].y());
						}
					}
					else if (rsq.rightSign == 2)
					{
						for (int l = 0; l < 4; l++)
						{
							//v2[countv2++] = rsq.m_right[j].cord[l].x; v2[countv2++] = rsq.m_right[j].cord[l].y; v2[countv2++] = rsq.m_right[j].cord[l].z;
							pVerts2[countv2++] = QVector3D(rsq.m_right[j].cord[l].x(), rsq.m_right[j].cord[l].y(), rsq.m_right[j].cord[l].z());
							//n2[countn2++] = rsq.m_right[j].normal.x; n2[countn2++] = rsq.m_right[j].normal.y; n2[countn2++] = rsq.m_right[j].normal.z;
							pNormal2[countn2++] = QVector3D(rsq.m_right[j].normal.x(), rsq.m_right[j].normal.y(), rsq.m_right[j].normal.z());
							//tex2[countt2++] = rsq.m_right[j].text[l].x; tex2[countt2++] = rsq.m_right[j].text[l].y;
							pTex2[countt2++] = QVector2D(rsq.m_right[j].text[l].x(), rsq.m_right[j].text[l].y());
						}
					}
				}
			}

			if (sfill[j] > 0)//填方
			{
				/*if (bleft)
					m_quadCountLeft[no][j] = 4 * sfill[j];
				else*/
				//填方坡
				m_quadFillCount[no][j] = 4 * sfill[j];

				/*if (bleft)
				{
					glGenBuffers(3, vboHandleLeftFSlope2[no][j]);
					glGenVertexArrays(1, &(vaoHandleLeftFSlope2[no][j]));
					glBindVertexArray(vaoHandleLeftFSlope2[no][j]);

					glBindBuffer(GL_ARRAY_BUFFER, vboHandleLeftFSlope2[no][j][0]);
				}
				else*/
				{
					//surfaceVertsBuf[no].create();
					//surfaceVertsBuf[no].bind();
					//surfaceVertsBuf[no].allocate(pVerts, pcount[no] * sizeof(QVector3D));

					for (int m = 0; m<3; m++) slopFillVertsBuf[no][j][m].create();
				
					slopFillVertsBuf[no][j][0].bind();//顶点几何坐标
					slopFillVertsBuf[no][j][0].allocate(pVerts, m_quadFillCount[no][j] * sizeof(QVector3D));
					slopFillVertsBuf[no][j][1].bind();//顶点法向量
					slopFillVertsBuf[no][j][1].allocate(pNormal, m_quadFillCount[no][j] * sizeof(QVector3D));
					slopFillVertsBuf[no][j][2].bind();//顶点纹理坐标
					slopFillVertsBuf[no][j][2].allocate(pTex, m_quadFillCount[no][j] * sizeof(QVector2D));
				}
				{
					//	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sfill[j] * sizeof(float), v, GL_STATIC_DRAW);
					//	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
					//	glEnableVertexAttribArray(0);  // Vertex position

						//if ((4 * 3 * sfill[j]) < countv)
						//	countv = countv;
						//if (bleft)
						//	glBindBuffer(GL_ARRAY_BUFFER, vboHandleLeftFSlope2[no][j][1]);
						//else
						//	glBindBuffer(GL_ARRAY_BUFFER, vboHandleFSlope2[no][j][1]);
						//glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sfill[j] * sizeof(float), n, GL_STATIC_DRAW);
						//glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
						//glEnableVertexAttribArray(1);  // Vertex normal

						//if (bleft)
						//	glBindBuffer(GL_ARRAY_BUFFER, vboHandleLeftFSlope2[no][j][2]);
						//else
						//	glBindBuffer(GL_ARRAY_BUFFER, vboHandleFSlope2[no][j][2]);
						//glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sfill[j] * sizeof(float), tex, GL_STATIC_DRAW);
						//glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
						//glEnableVertexAttribArray(2);  // Texture coords

						//glBindVertexArray(0);
				}

				delete[]pVerts;
				delete[]pNormal;
				delete[]pTex;
			}

			if (sexcave[j] > 0)//挖方
			{
				//if (bleft)
				//{
				//	m_quadCountLeftExcave[no][j] = 4 * sexcave[j];
				//	glGenBuffers(3, vboExcaveHandleLeftFSlope2[no][j]);

				//	glGenVertexArrays(1, &(vaoExcaveHandleLeftFSlope2[no][j]));
				//	glBindVertexArray(vaoExcaveHandleLeftFSlope2[no][j]);

				//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleLeftFSlope2[no][j][0]);
				//}
				//else
				{
					m_quadExcaveCount[no][j] = 4 * sexcave[j];

					for (int m = 0; m < 3; m++) slopExcaveVertsBuf[no][j][m].create();

					{
						//	glGenBuffers(3, vboExcaveHandleFSlope2[no][j]);

						//	glGenVertexArrays(1, &(vaoExcaveHandleFSlope2[no][j]));
						//	glBindVertexArray(vaoExcaveHandleFSlope2[no][j]);

						//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleFSlope2[no][j][0]);
						//}
						//glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sexcave[j] * sizeof(float), v2, GL_STATIC_DRAW);
						//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
						//glEnableVertexAttribArray(0);  // Vertex position
						//if ((4 * 3 * sexcave[j]) != countv2)
						//	countv2 = countv2;

						//if (bleft)
						//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleLeftFSlope2[no][j][1]);
						//else
						//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleFSlope2[no][j][1]);
						//glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sexcave[j] * sizeof(float), n2, GL_STATIC_DRAW);
						//glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
						//glEnableVertexAttribArray(1);  // Vertex normal

						//if (bleft)
						//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleLeftFSlope2[no][j][2]);
						//else
						//	glBindBuffer(GL_ARRAY_BUFFER, vboExcaveHandleFSlope2[no][j][2]);
						//glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sexcave[j] * sizeof(float), tex2, GL_STATIC_DRAW);
						//glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
						//glEnableVertexAttribArray(2);  // Texture coords

						//glBindVertexArray(0);
					}

					slopExcaveVertsBuf[no][j][0].bind();//顶点几何坐标
					slopExcaveVertsBuf[no][j][0].allocate(pVerts2, m_quadExcaveCount[no][j] * sizeof(QVector3D));
					slopExcaveVertsBuf[no][j][1].bind();//顶点法向量
					slopExcaveVertsBuf[no][j][1].allocate(pNormal2, m_quadExcaveCount[no][j] * sizeof(QVector3D));
					slopExcaveVertsBuf[no][j][2].bind();//顶点纹理坐标
					slopFillVertsBuf[no][j][2].allocate(pTex2, m_quadExcaveCount[no][j] * sizeof(QVector2D));

					delete[]pVerts2;
					delete[]pNormal2;
					delete[]pTex2;
				}
			}
		}
	}
}

void rdRoadSlope::renderSurface(QOpenGLShaderProgram* program)
{
	/*program->bind();
	QMatrix4x4 modelM = view;
	program->setUniformValue("ProjectionM", proj);
	program->setUniformValue("ModelM", modelM);
	program->setUniformValue("Color", QVector3D(0.2, 0.2, 0.2));*/
    int vertexLocation = program->attributeLocation("VertexPos");

	for (int i = 0; i < 1; i++)
	{
		surfaceVertsBuf[i].bind();		
		program->enableAttributeArray(vertexLocation);
		program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);
		glDrawArrays(GL_QUADS, 0, m_quadCountSurface[i]);
	}
}

void rdRoadSlope::renderSlope(QOpenGLShaderProgram* program)
{
	//program->bind();
	/*QMatrix4x4 modelM = view;
	program->setUniformValue("ProjectionM", proj);
	program->setUniformValue("ModelM", modelM);*/
	
	int vertexLocation = program->attributeLocation("VertexPosition");
	int texcoordLocation = program->attributeLocation("VertexTexCoord");

	//纹理
//	int m_quadFillCount[14][8]; //填方
//	QOpenGLBuffer slopFillVertsBuf[14][8][3];//8级

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_quadFillCount[i][j] == 0) continue;

			QOpenGLTexture* texture = m_texFill1;
			if (j>=1 ) texture = m_texFill2;
			texture->bind();
			program->setUniformValue("Tex0", 0);

			slopFillVertsBuf[i][j][0].bind();
			program->enableAttributeArray(vertexLocation);
			program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);
			slopFillVertsBuf[i][j][2].bind();
			program->enableAttributeArray(texcoordLocation);
			program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2);
			glDrawArrays(GL_QUADS, 0, m_quadFillCount[i][j]);
		}
	}
}