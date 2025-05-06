#include "rdBridgeCrashBarriers.h"
#include "../common/common.h"
#include <QFile>
#include <QTextStream>
#include "common/orQStrLine.h"
#include "rdSolidCrashBarrier.h"

rdBridgeCrashBarriers::rdBridgeCrashBarriers()
{

}

rdBridgeCrashBarriers::~rdBridgeCrashBarriers(void)
{
	
} 

void rdBridgeCrashBarriers::addCrashBarrier(rdBridgeCrashBarrier* pCrashBarrier)
{
	pCrashBarrier->calCoord();
	m_crashbarriers.push_back(pCrashBarrier);
}

bool rdBridgeCrashBarriers::read(QString filename)
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
			readCrashBarrier(aStream, type);
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

bool rdBridgeCrashBarriers::readCrashBarrier(QTextStream& stream, int type)
{
	switch (type)
	{
	case 0:
		return readSolidCrashBarrier(stream);

	}

	return false;
}

bool rdBridgeCrashBarriers::readSolidCrashBarrier(QTextStream& fin)
{
	_solidcrashbarrierPara scbp;
	bool re = rdSolidCrashBarrier::readFile(fin, scbp);
	
	if (re)
	{
		rdSolidCrashBarrier* pSolidCrashBarrier = new rdSolidCrashBarrier;
		pSolidCrashBarrier->SetPara(scbp);
		pSolidCrashBarrier->setName("SolidCrashBarrier");

		addCrashBarrier(pSolidCrashBarrier);
	}
	return true;
}

void rdBridgeCrashBarriers::initGeometry()
{
	for (int i = 0; i < m_crashbarriers.size(); i++)
	{
		m_crashbarriers[i]->initGeometry();
	}
}

rdBridgeCrashBarrier* rdBridgeCrashBarriers::findCrashBarrier(QString stype, double length, double angle)
{
	rdBridgeCrashBarrier* pSelectedCrashBarrier = 0;
	
	for (int i = 0; i < m_crashbarriers.size(); i++)
	{
		if (m_crashbarriers[i]->getName() != stype) continue;
		pSelectedCrashBarrier = m_crashbarriers[i];
		break;
	}

	return pSelectedCrashBarrier;
}
