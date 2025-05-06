#include "rdBridgeGirders.h"
#include "../common/common.h"
#include <QFile>
#include <QTextStream>
#include "common/orQStrLine.h"
#include "rdHollowSlab.h"
#include "rdBoxGirder.h"

rdBridgeGirders::rdBridgeGirders()
{

}

rdBridgeGirders::~rdBridgeGirders(void)
{
	
} 

void rdBridgeGirders::addGirder(rdBridgeGirder* pGirder)
{
	pGirder->calCoord();
	m_girders.push_back(pGirder);
}

bool rdBridgeGirders::read(QString filename)
{
    QFile aFile(filename);
    if (!aFile.open(QIODevice::ReadOnly)) return false;
    QTextStream aStream(&aFile);
   
	//文件头 --第一行   
	QString linestr = aStream.readLine();
	QVector<QString>  words;
	int re = orQStrLine::AnayStringLine(linestr, words);
	if (re > 0)
	{
		QString key = words[0];
		if (key != "BRIDGE1.0") return false;
	}

	//读每类主梁参数数据，以/结束
	while (!aStream.atEnd())
	{
		linestr = aStream.readLine();
		re = orQStrLine::AnayStringLine(linestr, words);
	
		if (re == 2)
		{
			QString key = words[0];
			int type = words[1].toInt();
			readGirder(aStream, type);
		}
		else if (re == 1)
		{
			if (words[0] == "/") break;
		}
		else continue;
	}

    aFile.close();

	return true;
}

bool rdBridgeGirders::readGirder(QTextStream& stream, int type)
{
	switch (type)
	{
	case 0:
		return readHollowSlab(stream);
	case 1:
		return readBoxGirder(stream);
	}

	return false;
}

bool rdBridgeGirders::readHollowSlab(QTextStream& fin)
{
	_hollowSlabPara hsp;
	bool re = rdHollowSlab::readFile(fin, hsp);
	
	if (re)
	{
		/*rdHollowSlab* pHS = new rdHollowSlab;
		pHS->SetPara(hsp);

		addGirder(pHS);*/
	}
	return true;
}

bool rdBridgeGirders::readBoxGirder(QTextStream& fin)
{
	_boxGirderPara bgp;
	bool re = rdBoxGirder::readFile(fin, bgp);

	if (re)
	{
		rdBoxGirder* pBG = new rdBoxGirder;
		pBG->SetPara(bgp);
		pBG->setName("BoxGirder");

		addGirder(pBG);
	}
	return true;
}

void rdBridgeGirders::initGeometry()
{
	QVector <rdBridgeGirder*>;
	for (int i = 0; i < m_girders.size(); i++)
	{
		m_girders[i]->initGeometry();
	}
}

rdBridgeGirder* rdBridgeGirders::findGirder(QString stype, double length, double angle)
{
	rdBridgeGirder* pSelectedGirder = 0;
	
	for (int i = 0; i < m_girders.size(); i++)
	{
		if (m_girders[i]->getName() != stype) continue;
		pSelectedGirder = m_girders[i];
		break;
	}

	return pSelectedGirder;
}
