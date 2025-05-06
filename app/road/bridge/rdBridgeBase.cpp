#include "rdBridgeBase.h"
#include "rdBridgeCylinderPile.h"
#include "common/orQStrLine.h"

rdBridgeBase::rdBridgeBase()
{
}

rdBridgeBase::~rdBridgeBase(void)
{
	
} 

//桥基础
QVector<rdBridgePile*> *rdBridgeBase::getPiles()
{
	return &m_piles;
}

rdBridgePile* rdBridgeBase::getPile(QString name)
{
	rdBridgePile* pSelectedPile = 0;
	for (int i = 0; i < m_piles.size(); i++)
	{
		if (m_piles[i]->getName() == name)
		{
			pSelectedPile = m_piles[i];
			break;
		}
    }

	return pSelectedPile;
}

void rdBridgeBase::AddPile(rdBridgePile* pile)
{
	m_piles.push_back(pile);
}

//承台
QVector<rdBridgePileUp*>* rdBridgeBase::getPileUps()
{
	return &m_pileups;
}

void rdBridgeBase::AddPileUp(rdBridgePileUp* pileup)
{
	m_pileups.push_back(pileup);
}

rdBridgePileUp* rdBridgeBase::getPileUp(QString name)
{
	rdBridgePileUp* pSelectedPileUp = 0;
	for (int i = 0; i < m_pileups.size(); i++)
	{
		if (m_pileups[i]->getName() == name)
		{
			pSelectedPileUp = m_pileups[i];
			break;
		}
	}

	return pSelectedPileUp;
}

void rdBridgeBase::clear()
{
	while (m_piles.size())
	{
		rdBridgePile* p = m_piles[0];
		delete p;
		m_piles.erase(m_piles.begin());
	}
}

rdBridgeObject* rdBridgeBase::PtIn(QVector3D pt)
{
	for (int i = 0; i < m_piles.size(); i++)
		if (m_piles[i]->ptIn(pt)) return m_piles[i];

	return 0;
}

void rdBridgeBase::SetAllPiles(bool bShow)
{
	for (int i = 0; i < m_piles.size(); i++) m_piles[i]->setShow(bShow);
}

//最大最小值
QVector3D rdBridgeBase::getMinPt()
{
	return m_minPt;
}

QVector3D rdBridgeBase::getMaxPt()
{
	return m_maxPt;
}

void rdBridgeBase::modifyMinMaxPt(QVector3D minpt, QVector3D maxpt)
{
	if (minpt.x() < m_minPt.x()) m_minPt.setX(minpt.x());
	if (minpt.y() < m_minPt.y()) m_minPt.setY(minpt.y());
	if (minpt.z() < m_minPt.z()) m_minPt.setZ(minpt.z());

	if (maxpt.x() > m_maxPt.x()) m_maxPt.setX(maxpt.x());
	if (maxpt.y() > m_maxPt.y()) m_maxPt.setY(maxpt.y());
	if (maxpt.z() > m_maxPt.z()) m_maxPt.setZ(maxpt.z());
}

void rdBridgeBase::initMinMaxPt()
{
	m_minPt = QVector3D(99999999, 99999999, 99999999);
	m_maxPt = QVector3D(-99999999, -99999999, -99999999);
}

void rdBridgeBase::calMinMaxPt()
{
	initMinMaxPt();
	//桩
	for (int i = 0; i < m_piles.size(); i++)
	{
		rdBridgePile* pPile = m_piles[i];
		QVector3D minPt = pPile->getMinPt();
		QVector3D maxPt = pPile->getMaxPt();

		modifyMinMaxPt(minPt, maxPt);
	}
}

void rdBridgeBase::save(QDataStream& outStream)
{
	if (m_piles.size() > 0) //||承台
	{
		QString str = "BASE";
		outStream.writeRawData(str.toLocal8Bit(), 8);
	}

	if (m_piles.size() > 0) //||承台
	{
		//统计类型 暂时不考虑
		QString str = "PILE";
		outStream.writeRawData(str.toLocal8Bit(), 8);
		int type = 0;
		int size = m_piles.size();
		outStream.writeRawData((char*)&type, sizeof(int));
		outStream.writeRawData((char*)&size, sizeof(int));

		for (int i = 0; i < m_piles.size(); i++)
		{
			rdBridgeCylinderPile* pPile = (rdBridgeCylinderPile*)m_piles[i];
			outStream.writeRawData(pPile->getName().toLocal8Bit(), 8);

			int flag = pPile->getFlag();
			outStream.writeRawData((char*)&flag, sizeof(int));

			QVector3D pos = pPile->getPos();			
			float x = pos.x();
			outStream.writeRawData((char*)&x, sizeof(float));
			float y = pos.y();
			outStream.writeRawData((char*)&y, sizeof(float));
			float z = pos.z();
			outStream.writeRawData((char*)&z, sizeof(float));
			float height = pPile->getHeight();
			outStream.writeRawData((char*)&height, sizeof(float));
			float radius = pPile->getRadius();
			outStream.writeRawData((char*)&radius, sizeof(float));
		}
	}

	//其它类型
	
	//写Base结束符
	QString str = "/";
	outStream.writeRawData(str.toLocal8Bit(), 8);
}

void rdBridgeBase::read(QDataStream& inStream)
{
	//初始化最小最大值
	initMinMaxPt();
	//读8个字符,判断是关键字还是结束符
	char bname[64];
	while (!inStream.atEnd())
	{
		inStream.readRawData(bname, 8);
		QString key = QString::fromLocal8Bit(bname, 8);
		key = orQStrLine::clean(key);

		if (key == "/") break;
		if (key == "PILE")
		{
			//读类型及个数
			int type = 0;
			inStream.readRawData((char*)&type, sizeof(int));
			int count = 0;
			inStream.readRawData((char*)&count, sizeof(int));

			if (type == 0)
			{
				for (int i = 0; i < count; i++)
				{
					//读名称
					inStream.readRawData(bname, 8);
					QString name = QString::fromLocal8Bit(bname, 8);
					name = orQStrLine::clean(name);
					int flag = 0;
					inStream.readRawData((char*)&flag, sizeof(int));

					float x,y, z,height,radius;
					inStream.readRawData((char*)&x, sizeof(float));
					inStream.readRawData((char*)&y, sizeof(float));
					inStream.readRawData((char*)&z, sizeof(float));
					inStream.readRawData((char*)&height, sizeof(float));
					inStream.readRawData((char*)&radius, sizeof(float));

					rdBridgeCylinderPile* pPile = new rdBridgeCylinderPile;
					pPile->setName(name);
					pPile->setHeight(height);
					pPile->setRadius(radius);
					pPile->setPos(QVector3D(x, y, z));
					pPile->calMinMaxPt();

					//添加
					m_piles.push_back(pPile);
					//修改最小最大值
					modifyMinMaxPt(pPile->getMinPt(), pPile->getMaxPt());
				}
			}
		}
	}
}

void rdBridgeBase::copy(rdBridgeBase* pBridgeBase)
{
	m_minPt = pBridgeBase->getMinPt();
	m_maxPt = pBridgeBase->getMaxPt();

	//桥桩
	QVector<rdBridgePile*>* piles = pBridgeBase->getPiles();

	for (int i = 0; i < piles->size(); i++)
	{
		if (qobject_cast<rdBridgeCylinderPile*>((*piles)[i]))
		{
			rdBridgeCylinderPile* pCylinderPile = new rdBridgeCylinderPile;
			pCylinderPile->copy((*piles)[i]);
			m_piles.push_back(pCylinderPile);
		}
	}
	//承台
}


void rdBridgeBase::createVBO()
{
	for (int i = 0; i < m_pileups.size(); i++)
		m_pileups[i]->initGeometry();
}

QVector3D rdBridgeBase::getFirstLeftPilePos()
{
	for (int i = 0; i < m_piles.size(); i++)
	{
		if (m_piles[i]->getFlag() == 1) return m_piles[i]->getPos();
	}

	return  QVector3D(0, 0, 0);
}

QVector3D rdBridgeBase::getFirstRightPilePos()
{
	for (int i = 0; i < m_piles.size(); i++)
	{
		if (m_piles[i]->getFlag() == 2) return m_piles[i]->getPos();
	}

	return  QVector3D(0, 0, 0);
}

QVector3D rdBridgeBase::getLastLeftPilePos()
{
	int size = m_piles.size();
	for (int i = 0; i <size; i++)//后往前找
	{
		int index = size - 1 - i;
		if (m_piles[index]->getFlag() == 3) return m_piles[index]->getPos();
	}

	return  QVector3D(0, 0, 0);
}

QVector3D rdBridgeBase::getLastRightPilePos()
{
	int size = m_piles.size();
	for (int i = 0; i < size; i++)//后往前找
	{
		int index = size - 1 - i;
		if (m_piles[index]->getFlag() == 4) return m_piles[index]->getPos();
	}

	return  QVector3D(0, 0, 0);
}