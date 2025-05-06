#include "rdBridgeObject.h"
#include "../common/common.h"

rdBridgeObject::rdBridgeObject()
{
	m_name = " ";
	m_bShow = true;

	m_pTextures = 0;
}


rdBridgeObject::~rdBridgeObject(void)
{
	
} 

void rdBridgeObject::setTextures(rdTextures* pTextures)
{
	m_pTextures = pTextures;
}

QString rdBridgeObject::getName()
{
	return m_name;
}

void rdBridgeObject::setName(QString name)
{
	m_name = name;
}

QVector3D rdBridgeObject::getMinPt()
{
	return m_minPt;
}

QVector3D rdBridgeObject::getMaxPt()
{
	return m_maxPt;
}

void rdBridgeObject::setShow(bool bShow)
{
	m_bShow = bShow;
}

bool rdBridgeObject::getShow()
{
	return m_bShow;
}

bool rdBridgeObject::ptInBox(QVector3D pt)
{
	if (pt.x() > m_maxPt.x() || pt.x() < m_minPt.x()) return false;
	if (pt.y() > m_maxPt.y() || pt.y() < m_minPt.y()) return false;
	if (pt.z() > m_maxPt.z() || pt.z() < m_minPt.z()) return false;

	return true;
}