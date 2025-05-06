#include "rdContractSection.h"
#include <QFile.h>
#include <QDataStream.h>
#include <QObject>
#include <QByteArray>

#include "common/orQStrLine.h"

rdContractSection::rdContractSection()
{
}

rdContractSection::~rdContractSection(void)
{
	clear();
} 

void rdContractSection::initGeometry()
{
	initializeOpenGLFunctions();	
}

void rdContractSection::renderBridge(QOpenGLShaderProgram* program)
{
	for (int i = 0; i < m_bridges.size(); i++)
		m_bridges[i]->render(program);
}

void rdContractSection::clear()
{
	while (m_bridges.size())
	{
		rdBridge* pBridge = m_bridges[0];
		m_bridges.erase(m_bridges.begin());
		delete pBridge;
	}
}

bool rdContractSection::addBridge(rdBridge* pBridge)
{
	//查找是否存在
	for (int i = 0; i < m_bridges.size(); i++)
	{
		if (m_bridges[i]->getName() == pBridge->getName()) return false;
	}	

	m_bridges.push_back(pBridge);
	return true;
}