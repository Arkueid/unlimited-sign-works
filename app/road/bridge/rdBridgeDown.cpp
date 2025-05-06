#include "rdBridgeDown.h"
#include "rdBridgeCylinderPile.h"
#include "common/orQStrLine.h"

rdBridgeDown::rdBridgeDown()
{
}

rdBridgeDown::~rdBridgeDown(void)
{
	
} 

//桥墩
QVector<rdBridgePier*> * rdBridgeDown::getPiers()
{
	return &m_piers;
}

rdBridgePier* rdBridgeDown::getPier(QString name)
{
	rdBridgePier* pSelectedPier = 0;
	for (int i = 0; i < m_piers.size(); i++)
	{
		if (m_piers[i]->getName() == name)
		{
			pSelectedPier = m_piers[i];
			break;
		}
    }

	return pSelectedPier;
}

void rdBridgeDown::addPier(rdBridgePier* pier)
{
	m_piers.push_back(pier);
}

//桥台
QVector<rdBridgeAbutment*>* rdBridgeDown::getAbutments()
{
	return &m_abutments;
}

void rdBridgeDown::addAbutment(rdBridgeAbutment* pAbutment)
{
	m_abutments.push_back(pAbutment);
}

rdBridgeAbutment* rdBridgeDown::getAbutment(QString name)
{
	rdBridgeAbutment* pSelectedAbutment = 0;
	for (int i = 0; i < m_abutments.size(); i++)
	{
		if (m_abutments[i]->getName() == name)
		{
			pSelectedAbutment = m_abutments[i];
			break;
		}
	}

	return pSelectedAbutment;
}


//盖梁
QVector <rdBridgeBentCap*>* rdBridgeDown::getBentCaps()
{
	return &m_bentcaps;
}

void rdBridgeDown::addBentCap(rdBridgeBentCap* pBentCap)
{
	m_bentcaps.push_back(pBentCap);
}

rdBridgeBentCap* rdBridgeDown::getBentCap(QString name)
{
	rdBridgeBentCap* pSelectedBentCap = 0;
	for (int i = 0; i < m_abutments.size(); i++)
	{
		if (m_bentcaps[i]->getName() == name)
		{
			pSelectedBentCap = m_bentcaps[i];
			break;
		}
	}

	return pSelectedBentCap;
}

//台帽
QVector <rdBridgeAbutmentCap*>* rdBridgeDown::getAbutmentCaps()
{
	return &m_abutmentcaps;
}

void rdBridgeDown::addAbutmentCap(rdBridgeAbutmentCap* pAbutmentCap)
{
	m_abutmentcaps.push_back(pAbutmentCap);
}

rdBridgeAbutmentCap* rdBridgeDown::getAbutmentCap(QString name)
{
	rdBridgeAbutmentCap* pSelectedAbutmentCap = 0;
	for (int i = 0; i < m_abutments.size(); i++)
	{
		if (m_bentcaps[i]->getName() == name)
		{
			pSelectedAbutmentCap = m_abutmentcaps[i];
			break;
		}
	}

	return pSelectedAbutmentCap;
}

void rdBridgeDown::clear()
{
	while (m_piers.size())
	{
		rdBridgePier* p = m_piers[0];
		delete p;
		m_piers.erase(m_piers.begin());
	}
}

rdBridgeObject* rdBridgeDown::PtIn(QVector3D pt)
{
	for (int i = 0; i < m_piers.size(); i++)
		if (m_piers[i]->ptIn(pt)) return m_piers[i];

	return 0;
}

void rdBridgeDown::SetAllPiles(bool bShow)
{
	for (int i = 0; i < m_piers.size(); i++) m_piers[i]->setShow(bShow);
}

//最大最小值
QVector3D rdBridgeDown::getMinPt()
{
	return m_minPt;
}

QVector3D rdBridgeDown::getMaxPt()
{
	return m_maxPt;
}

void rdBridgeDown::modifyMinMaxPt(QVector3D minpt, QVector3D maxpt)
{
	if (minpt.x() < m_minPt.x()) m_minPt.setX(minpt.x());
	if (minpt.y() < m_minPt.y()) m_minPt.setY(minpt.y());
	if (minpt.z() < m_minPt.z()) m_minPt.setZ(minpt.z());

	if (maxpt.x() > m_maxPt.x()) m_maxPt.setX(maxpt.x());
	if (maxpt.y() > m_maxPt.y()) m_maxPt.setY(maxpt.y());
	if (maxpt.z() > m_maxPt.z()) m_maxPt.setZ(maxpt.z());
}

void rdBridgeDown::initMinMaxPt()
{
	m_minPt = QVector3D(99999999, 99999999, 99999999);
	m_maxPt = QVector3D(-99999999, -99999999, -99999999);
}

void rdBridgeDown::calMinMaxPt()
{
	initMinMaxPt();
	//桩
	for (int i = 0; i < m_piers.size(); i++)
	{
		rdBridgePier* pPile = m_piers[i];
		QVector3D minPt = pPile->getMinPt();
		QVector3D maxPt = pPile->getMaxPt();

		modifyMinMaxPt(minPt, maxPt);
	}
}

void rdBridgeDown::save(QDataStream& outStream)
{
	//if (m_piers.size() > 0) //||承台
	//{
	//	QString str = "DOWN";
	//	outStream.writeRawData(str.toLocal8Bit(), 8);
	//}

	//if (m_piles.size() > 0) //||承台
	//{
	//	//统计类型 暂时不考虑
	//	QString str = "PILE";
	//	outStream.writeRawData(str.toLocal8Bit(), 8);
	//	int type = 0;
	//	int size = m_piles.size();
	//	outStream.writeRawData((char*)&type, sizeof(int));
	//	outStream.writeRawData((char*)&size, sizeof(int));

	//	for (int i = 0; i < m_piles.size(); i++)
	//	{
	//		rdBridgeCylinderPile* pPile = (rdBridgeCylinderPile*)m_piles[i];
	//		outStream.writeRawData(pPile->getName().toLocal8Bit(), 8);

	//		QVector3D pos = pPile->getPos();			
	//		float x = pos.x();
	//		outStream.writeRawData((char*)&x, sizeof(float));
	//		float y = pos.y();
	//		outStream.writeRawData((char*)&y, sizeof(float));
	//		float z = pos.z();
	//		outStream.writeRawData((char*)&z, sizeof(float));
	//		float height = pPile->getHeight();
	//		outStream.writeRawData((char*)&height, sizeof(float));
	//		float radius = pPile->getRadius();
	//		outStream.writeRawData((char*)&radius, sizeof(float));
	//	}
	//}

	////其它类型
	//
	////写Base结束符
	//QString str = "/";
	//outStream.writeRawData(str.toLocal8Bit(), 8);
}

void rdBridgeDown::read(QDataStream& inStream)
{
	////初始化最小最大值
	//initMinMaxPt();
	////读8个字符,判断是关键字还是结束符
	//char bname[64];
	//while (!inStream.atEnd())
	//{
	//	inStream.readRawData(bname, 8);
	//	QString key = QString::fromLocal8Bit(bname, 8);
	//	key = orQStrLine::clean(key);

	//	if (key == "/") break;
	//	if (key == "PILE")
	//	{
	//		//读类型及个数
	//		int type = 0;
	//		inStream.readRawData((char*)&type, sizeof(int));
	//		int count = 0;
	//		inStream.readRawData((char*)&count, sizeof(int));

	//		if (type == 0)
	//		{
	//			for (int i = 0; i < count; i++)
	//			{
	//				//读名称
	//				inStream.readRawData(bname, 8);
	//				QString name = QString::fromLocal8Bit(bname, 8);
	//				name = orQStrLine::clean(name);

	//				float x,y, z,height,radius;
	//				inStream.readRawData((char*)&x, sizeof(float));
	//				inStream.readRawData((char*)&y, sizeof(float));
	//				inStream.readRawData((char*)&z, sizeof(float));
	//				inStream.readRawData((char*)&height, sizeof(float));
	//				inStream.readRawData((char*)&radius, sizeof(float));

	//				rdBridgeCylinderPile* pPile = new rdBridgeCylinderPile;
	//				pPile->setName(name);
	//				pPile->setHeight(height);
	//				pPile->setRadius(radius);
	//				pPile->setPos(QVector3D(x, y, z));
	//				pPile->calMinMaxPt();

	//				//添加
	//				m_piles.push_back(pPile);
	//				//修改最小最大值
	//				modifyMinMaxPt(pPile->getMinPt(), pPile->getMaxPt());
	//			}
	//		}
	//	}
	//}
}

void rdBridgeDown::copy(rdBridgeDown* pBridgeDown)
{
	m_minPt = pBridgeDown->getMinPt();
	m_maxPt = pBridgeDown->getMaxPt();

	//桥桩
	QVector<rdBridgePier*>* piers = pBridgeDown->getPiers();

	for (int i = 0; i < piers->size(); i++)
	{
		/*if (qobject_cast<rdBridgeCylinderPile*>((*piles)[i]))
		{
			rdBridgeCylinderPile* pCylinderPile = new rdBridgeCylinderPile;
			pCylinderPile->copy((*piles)[i]);
			m_piles.push_back(pCylinderPile);
		}*/
	}
	//承台
}


void rdBridgeDown::createVBO()
{
	for (int i = 0; i < m_piers.size(); i++)
		m_piers[i]->initGeometry();

	for (int i = 0; i < m_abutments.size(); i++)
		m_abutments[i]->initGeometry();

	for (int i = 0; i < m_bentcaps.size(); i++)
		m_bentcaps[i]->initGeometry();

	for (int i = 0; i < m_abutmentcaps.size(); i++)
		m_abutmentcaps[i]->initGeometry();
}