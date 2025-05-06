#include "rdRoadDitch.h"
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <math.h>

rdRoadDitch::rdRoadDitch()
{
	
}

rdRoadDitch::~rdRoadDitch()
{
	vertsBuf.destroy();
}

//读主线数据
bool rdRoadDitch::readData()
{
	QString filename = QCoreApplication::applicationDirPath() + "/../../assets/data/ditch.txt";

	QFile file(filename);
	if (!file.exists()) return false;

	bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ret)
	{
		QTextStream fin(&file);
		_RoadDitchData rdd;
		m_RoadDitch.clear();

    	QString str;

	 	while (!fin.atEnd())
	 	{
			fin >> rdd.no;
			fin >> rdd.start_KNo;
			if (rdd.start_KNo == "" || fin.atEnd()) break;

			fin >> rdd.end_KNo;

			fin >> rdd.lditchSign;//左侧排水沟标志
			fin >> rdd.rditchSign;

			fin >> rdd.width;
			fin >> rdd.height;

			//计算Start pileNo
			str = rdd.start_KNo;
			int sindex = (int)str.indexOf('-');//0或-1
			int index = (int)str.indexOf('+');
			int kindex = (int)str.indexOf('K');
	        QString substr = str.mid(kindex + 1, index - kindex - 1);
			int kk = substr.toInt();
			int len = (int)str.length();
			QString substr1 = str.mid(index + 1, len - index);
			double  s = substr1.toDouble();
			double f = kk * 1000 + s;
			rdd.startno = f;
			if (sindex == 0) rdd.startno = -f;

			//计算End pileNo
			str = rdd.end_KNo;
			sindex = (int)str.indexOf('-');//0或-1
			index = (int)str.indexOf('+');
			kindex = (int)str.indexOf('K');

			substr = str.mid(kindex + 1, index - kindex - 1);

			kk = substr.toInt();

			len = (int)str.length();
			substr1 = str.mid(index + 1, len - index);
			s = substr1.toDouble();
			f = kk * 1000 + s;
			rdd.endno = f;

			if (sindex == 0) rdd.endno = -f;

			m_RoadDitch.push_back(rdd);
		}
		
		file.close();
		return true;
	}

	file.close();
	return false;
}

void rdRoadDitch::dataProcessing(rdRoadRamp& roadRamp)
{
	//主线
	int j = 0;
	int no = 0;
	int index = 0;
	while (index < m_RoadDitch.size())
	{
		if (m_RoadDitch[index].no > 1) break;//暂时只处理1合同段
		QVector<_rampdata>& rampData = roadRamp.m_rampData[no];
		int num = rampData.size();

		while (m_RoadDitch[index].startno <= rampData[j].KNo && rampData[j].KNo <= m_RoadDitch[index].endno)
		{
			_ditchdata rdd;

			rdd._KNo = rampData[j]._KNo;
			rdd.pile = rampData[j].KNo;
			rdd.leftSign = rampData[j].leftSign;
			rdd.rightSign = rampData[j].rightSign;
			rdd.leftnum = rampData[j].leftnum;
			rdd.rightnum = rampData[j].rightnum;

			rdd.lditchSign = m_RoadDitch[index].lditchSign;
			rdd.rditchSign = m_RoadDitch[index].rditchSign;
			rdd.width = m_RoadDitch[index].width;
			rdd.height = m_RoadDitch[index].height;

			m_ditchData[no].push_back(rdd);

			j++;

			if (j >= num)
			{
				j = 0;

				if (no < 14)
				{
					no++;
				}
				break;
			}
		}
	    
		index++;
	}
}

void rdRoadDitch::computing(rdRoadRamp& roadRamp)
{
	dataProcessing(roadRamp);
}


