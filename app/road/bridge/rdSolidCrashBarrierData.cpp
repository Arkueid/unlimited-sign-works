#include "rdSolidCrashBarrierData.h"
#include "../common/common.h"
#include "common/orQStrLine.h"

rdSolidCrashBarrierData::rdSolidCrashBarrierData()
{

}

rdSolidCrashBarrierData::~rdSolidCrashBarrierData(void)
{
	
} 

//bool rdBoxGirderData::readFile(QTextStream& fin, _boxGirderPara& bgp)
//{
//	QVector<QString>  words;
//
//	//读长度和角度
//	while (!fin.atEnd())
//	{
//		QString linestr = fin.readLine();
//		int re = orQStrLine::AnayStringLine(linestr, words);
//		if (re <= 0) continue;
//		if (re != 2) return false;
//		bgp.length = words[0].toDouble();
//		bgp.angle = words[1].toDouble();
//		break;
//	}
//	//读中间和边参数
//	bool bMiddle = false;
//	bool bSide = true;
//	while (!fin.atEnd())
//	{
//		QString linestr = fin.readLine();
//		int re = orQStrLine::AnayStringLine(linestr, words);
//		if (re <= 0) continue;
//		QString qstr = words[0];
//		if (qstr == "/") break;
//		if (re == 34)//中间
//		{
//			int flag = words[0].toInt();
//			if (flag == 1)
//			{
//				bMiddle = true;
//
//				for (int i = 0; i < 16; i++) bgp.middle.w[i] = words[i + 1].toDouble();
//				for (int i = 0; i < 14; i++) bgp.middle.h[i] = words[i + 17].toDouble();
//
//				bgp.middle.r = words[31].toDouble();
//				bgp.middle.ml = words[32].toDouble();
//				bgp.middle.mr = words[33].toDouble();
//			}
//
//		}
//		else if (re == 20)//边
//		{
//			bSide = true;
//		}
//		else break;
//	}
//
//	if (bMiddle && bSide) return true;
//
//	return false;
//}

//void rdBoxGirderData::SetPara(_boxGirderPara bgp)
//{
//	m_bgp = bgp;
//}

void rdSolidCrashBarrierData::setLeftDir(QVector3D dir)
{
	m_ldir = dir;
}

void rdSolidCrashBarrierData::setRightDir(QVector3D dir)
{
	m_rdir = dir;
}

void rdSolidCrashBarrierData::calCoord()
{
	
}

void rdSolidCrashBarrierData::initGeometry()
{
}

void rdSolidCrashBarrierData::calMinMaxPt()
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

bool rdSolidCrashBarrierData::ptInRightBox(QVector3D pt)
{
	/*if (pt.x() > m_maxRightPt.x() || pt.x() < m_minRightPt.x()) return false;
	if (pt.y() > m_maxRightPt.y() || pt.y() < m_minRightPt.y()) return false;
	if (pt.z() > m_maxRightPt.z() || pt.z() < m_minRightPt.z()) return false;*/

	return true;
}

bool rdSolidCrashBarrierData::ptIn(QVector3D pt)
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

void rdSolidCrashBarrierData::render(QOpenGLShaderProgram* program)
{

}

void rdSolidCrashBarrierData::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
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

bool rdSolidCrashBarrierData::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdSolidCrashBarrierData::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdSolidCrashBarrierData::copy(rdBridgeCrashBarrierData* pData)
{
	//rdHollowSlab* pPile = (rdHollowSlab*)pPier;

	//m_name = pPile->getName();
	//m_r = pPile->getRadius();
	//m_height = pPile->getHeight();
	//m_pos = pPile->getPos();
	//m_minPt = pPile->getMinPt();
	//m_maxPt = pPile->getMaxPt();
}