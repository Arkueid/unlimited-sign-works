#include "rdBridge.h"
#include <QFile.h>
#include <QDataStream.h>
#include <QObject>
#include <QByteArray>
#include <QPoint>
#include <QVector>

#include "rdBridgeCylinderPile.h"
#include "common/orQStrLine.h"
#include "rdBridgeAbutment.h"
#include "rdBridgeGirder.h"


rdBridge::rdBridge()
{
	m_pCylinder = 0;
	m_pCube = 0;
	m_pPadStone = 0;
	m_pGirders = 0;
	m_pCrashBarriers = 0;

	initMinMaxPt();
}

rdBridge::~rdBridge(void)
{
	clear();
	//if (m_pCylinder) delete m_pCylinder;
	//if (m_pCube) delete m_pCube;
} 

void rdBridge::setShow(bool bShow)
{
	m_bShow = bShow;
}

bool rdBridge::getShow()
{
	return m_bShow;
}

void rdBridge::setName(QString name)
{
	m_name = name;
}

QString rdBridge::getName()
{
	return m_name;
}

QVector3D rdBridge::getMinPt()
{
	return m_minPt;
}

QVector3D rdBridge::getMaxPt()
{
	return m_maxPt;
}

QVector<rdBridgePile*>* rdBridge::getPiles()
{
	return m_base.getPiles();
}

rdBridgePile* rdBridge::getPile(QString name)
{
	return m_base.getPile(name);
}

void rdBridge::clear()
{
	m_base.clear();
}

void rdBridge::setCylinder(geoCylinder* pCylinder)
{
	m_pCylinder = pCylinder;
}

void rdBridge::setCube(geoCube* pCube)
{
	m_pCube = pCube;
}

void rdBridge::setPadStone(rdPadStone* pPadStone)
{
	m_pPadStone = pPadStone;
}

void rdBridge::setBridgeGirders(rdBridgeGirders* pGirders)
{
	m_pGirders = pGirders;
}

void rdBridge::setBridgeCrashBarriers(rdBridgeCrashBarriers* pCrashBarriers)
{
	m_pCrashBarriers = pCrashBarriers;
}

void rdBridge::initMinMaxPt()
{
	m_minPt = QVector3D(99999999, 99999999, 99999999);
	m_maxPt = QVector3D(-99999999, -99999999, -99999999);
}

void rdBridge::modifyMinMaxPt(QVector3D minpt, QVector3D maxpt)
{
	if (minpt.x() < m_minPt.x()) m_minPt.setX(minpt.x());
	if (minpt.y() < m_minPt.y()) m_minPt.setY(minpt.y());
	if (minpt.z() < m_minPt.z()) m_minPt.setZ(minpt.z());

	if (maxpt.x() > m_maxPt.x()) m_maxPt.setX(maxpt.x());
	if (maxpt.y() > m_maxPt.y()) m_maxPt.setY(maxpt.y());
	if (maxpt.z() > m_maxPt.z()) m_maxPt.setZ(maxpt.z());
}

void rdBridge:: calMinMaxPt()
{
	initMinMaxPt();
	m_base.calMinMaxPt();
	QVector3D minPt = m_base.getMinPt();
	QVector3D maxPt = m_base.getMaxPt();

	modifyMinMaxPt(minPt, maxPt);
}

rdBridgeBase* rdBridge::getBase()
{
	return &m_base;
}

void rdBridge::addPile(rdBridgePile*pile)
{
	m_base.AddPile(pile);

	//修改最大最小值
	QVector3D minPt = pile->getMinPt();
	QVector3D maxPt = pile->getMaxPt();
	modifyMinMaxPt(minPt, maxPt);
}

void rdBridge::addPileUp(rdBridgePileUp* pileup)
{
	//计算坐标
	pileup->calCoord();
	m_base.AddPileUp(pileup);

	//修改最大最小值
	QVector3D minPt = pileup->getMinPt();
	QVector3D maxPt = pileup->getMaxPt();
	modifyMinMaxPt(minPt, maxPt);
}

rdBridgeDown* rdBridge::getDown()
{
	return &m_down;
}

void rdBridge::addPier(rdBridgePier* pPier)
{
	pPier->calCoord();
	m_down.addPier(pPier);
}

void rdBridge::addAbutment(rdBridgeAbutment*pBridgeAbutment)
{
	pBridgeAbutment->calCoord();
	m_down.addAbutment(pBridgeAbutment);
}

void rdBridge::addBentCap(rdBridgeBentCap* pBentCap)
{
	pBentCap->calCoord();
	m_down.addBentCap(pBentCap);
}

void rdBridge::addAbutmentCap(rdBridgeAbutmentCap* pAbutmentCap)
{
	pAbutmentCap->calCoord();
	m_down.addAbutmentCap(pAbutmentCap);
}

//桥上部结构
//箱梁
void rdBridge::addBoxGirder(rdBoxGirderData*pBoxGirder)
{
	m_up.addBridgeGirder(pBoxGirder);
}

//防撞护栏
void rdBridge::addCrashBarrier(rdBridgeCrashBarrierData*pCrashBarrier)
{
	m_ancillaryStructures.addBridgeCrashBarrier(pCrashBarrier);
}

void rdBridge::initGeometry()
{
	//initializeOpenGLFunctions();	
	m_base.createVBO();
	m_down.createVBO();
}

//包围盒测试
bool rdBridge::ptInBox(QVector3D pt)
{
	if (pt.x() > m_maxPt.x() || pt.x() < m_minPt.x()) return false;
	if (pt.y() > m_maxPt.y() || pt.y() < m_minPt.y()) return false;
	if (pt.z() > m_maxPt.z() || pt.z() < m_minPt.z()) return false;

	return true;
}

rdBridgeObject* rdBridge::PtIn(QVector3D pt)
{
	rdBridgeObject* pSelectedObject = m_base.PtIn(pt);
	if (pSelectedObject) return pSelectedObject;
	pSelectedObject = m_down.PtIn(pt);

	return pSelectedObject;
}

void rdBridge::render(QOpenGLShaderProgram* program)
{
	if (!m_bShow) return;
	//绘制桥桩
	QVector<rdBridgePile*>* pPiles = m_base.getPiles();
	for (int i = 0; i < pPiles->size(); i++)
	{
		//类型识别
		if (qobject_cast<rdBridgeCylinderPile*>((*pPiles)[i]))
		{
			rdBridgeCylinderPile* pBridgeCylinderPile = (rdBridgeCylinderPile*)((*pPiles)[i]);
			pBridgeCylinderPile->render(program, m_pCylinder);
		}
	}		

	//绘制承台
	QMatrix4x4 modelMatrix;
	modelMatrix.translate(0.0, 0.0, 0.0);
	program->setUniformValue("ModelMatrix", modelMatrix);
	QVector<rdBridgePileUp*>* pPileUps = m_base.getPileUps();
	for (int i = 0; i < pPileUps->size(); i++)
	{
		(*pPileUps)[i]->render(program);
	}

	//绘制桥墩
	QVector<rdBridgePier*>* pPiers = m_down.getPiers();
	for (int i = 0; i < pPiers->size(); i++)
	{
		(*pPiers)[i]->render(program);
	}

	//绘制桥台
	QVector<rdBridgeAbutment*>* pAbutments = m_down.getAbutments();
	for (int i = 0; i < pAbutments->size(); i++)
	{
		(*pAbutments)[i]->render(program);
	}

	//绘制盖梁
	QVector <rdBridgeBentCap*>* pBentCaps = m_down.getBentCaps();
	for (int i = 0; i < pBentCaps->size(); i++)
	{
		(*pBentCaps)[i]->render(program);
	}

	//绘制台帽
	QVector <rdBridgeAbutmentCap*>* pAbutementCaps = m_down.getAbutmentCaps();
	for (int i = 0; i < pAbutementCaps->size(); i++)
	{
		(*pAbutementCaps)[i]->render(program);
	}

	//绘制主梁
	QVector<rdBridgeGirderData*>* pBridgeGirderDatas = m_up.getBridgeGirders();
	QString str = "BoxGirder";
	double len = 25;
	double angle = 0;
	if (m_pGirders)
	{
		rdBridgeGirder* pBridgeGirder = m_pGirders->findGirder(str, len, angle);

		for (int i = 0; i < pBridgeGirderDatas->size(); i++)
		{
			rdBridgeGirderData* pBridgeGirderData = (*pBridgeGirderDatas)[i];
			QVector3D pos = pBridgeGirderData->getPos();
			float angle = pBridgeGirderData->getAngle();
			QMatrix4x4 rotateMatrix;
			rotateMatrix.rotate(angle + 90, 0, 0, 1);

			double anglex = pBridgeGirderData->getAngleX();
			QMatrix4x4 rotatexMatrix;
			rotatexMatrix.rotate(anglex, 1, 0, 0);

			QMatrix4x4 scaleMatrix;
			scaleMatrix.scale(-1, 1, 1);

			QMatrix4x4 modelMatrix;
			modelMatrix.translate(pos);
			int type = pBridgeGirderData->getFlag();
			QMatrix4x4 allMatrix = modelMatrix * rotateMatrix * rotatexMatrix * scaleMatrix;
			if (type == 1 || type == 3) allMatrix = modelMatrix * rotateMatrix * rotatexMatrix;

			program->setUniformValue("ModelMatrix", allMatrix);			

			pBridgeGirder->render(program,type);
		}
	}

	//绘制附属构造
	QVector<rdBridgeCrashBarrierData*>* pBridgeCrashBarrierDatas = m_ancillaryStructures.getBridgeCrashBarriers();
	if (m_pCrashBarriers)
	{
		rdBridgeCrashBarrier* pCrashBarrier =  m_pCrashBarriers->findCrashBarrier("SolidCrashBarrier", 0, 0);

		if (pCrashBarrier)
		{
			for (int i = 0; i < pBridgeCrashBarrierDatas->size(); i++)
			{
				rdBridgeCrashBarrierData* pCrashBarrierData = (*pBridgeCrashBarrierDatas)[i];
				QVector3D pos = pCrashBarrierData->getPos();
				float angle = pCrashBarrierData->getAngle();
				QMatrix4x4 rotateMatrix;
				rotateMatrix.rotate(angle + 90, 0, 0, 1);

				double anglex = pCrashBarrierData->getAngleX();
				QMatrix4x4 rotatexMatrix;
				rotatexMatrix.rotate(anglex, 1, 0, 0);
	
				double length = pCrashBarrierData->getLength();
				QMatrix4x4 scaleMatrix;
				scaleMatrix.scale(1, length/10, 1);

				QMatrix4x4 modelMatrix;
				modelMatrix.translate(pos);
				QMatrix4x4 allMatrix = modelMatrix * rotateMatrix * rotatexMatrix * scaleMatrix;

				program->setUniformValue("ModelMatrix", allMatrix);
				int flag = pCrashBarrierData->getFlag();
				pCrashBarrier->render(program, flag);
			}
		}
	}

	modelMatrix.translate(0.0, 0.0, 0.0);
	program->setUniformValue("ModelMatrix", modelMatrix);
}

void rdBridge::SetAllPiles(bool bShow)
{
	m_base.SetAllPiles(bShow);
}

void rdBridge::setSectionShow(_sectionTypeValue stv, bool bSHow)
{
	if (stv.svalue == "") return;
	switch (stv.type)
	{
	case PILE:
	{
		rdBridgePile* pPile = getPile(stv.svalue);
		pPile->setShow(bSHow);
		break;
	}
	}
}

bool rdBridge::save(QString fname)
{
	QFile aFile(fname);
	if (!(aFile.open(QIODevice::WriteOnly)))
		return false;
	QDataStream aStream(&aFile);
	aStream.setByteOrder(QDataStream::LittleEndian);//windows平台
	int s = 10;
	QString str = "Bridge2.0";
	
	aStream.writeRawData(str.toLocal8Bit(),16);
	aStream.writeRawData(m_name.toLocal8Bit(), 64);
	aStream.writeRawData(m_centerPeg.toLocal8Bit(), 16);
	//合同段（标段）
	aStream.writeRawData((char*)&m_sectionNo,sizeof(int));

	m_base.save(aStream);	

	aFile.close();
	
	//read(fname);

	return true;
}

bool rdBridge::read(QString fname)
{
	//测试读
	QFile inFile(fname);
	if (!(inFile.open(QIODevice::ReadOnly)))
		return false;
	QDataStream inStream(&inFile);

	inStream.setByteOrder(QDataStream::LittleEndian);
	
	char bname[64];
	//文件标记
	inStream.readRawData(bname, 16);	
	QString strname = QString::fromLocal8Bit(bname, 16);
	strname = orQStrLine::clean(strname);
	if (strname != "Bridge2.0") return false;

	//桥名
	inStream.readRawData(bname, 64);
	QString bridgename = QString::fromLocal8Bit(bname, 64);
	bridgename = orQStrLine::clean(bridgename);
	setName(bridgename);

	//中心桩号
	inStream.readRawData(bname, 16);
	QString centerPeg = QString::fromLocal8Bit(bname, 16);
	centerPeg = orQStrLine::clean(centerPeg);
	m_centerPeg = centerPeg;

	int sectionNo = 0;
	inStream.readRawData((char*)&sectionNo, sizeof(int));
	m_sectionNo = sectionNo;

	//读关键字
	while (!inStream.atEnd())
	{
		inStream.readRawData(bname, 8);
		QString key = QString::fromLocal8Bit(bname, 8);
		key = orQStrLine::clean(key);
		if (key == "BASE") m_base.read(inStream);
		else if (key == "DOWN") m_down.read(inStream);
		else if (key == "UP") m_up.read(inStream);		
	}

	inFile.close();

	//计算最大最小值
	calMinMaxPt();
	return true;
}

bool rdBridge::inport(QTextStream& aStream)
{
	//文件头 --第一行   
	QString linestr = aStream.readLine();
	QVector<QString>  words;
	int re = orQStrLine::AnayStringLine(linestr, words);
	if (re > 0)
	{
		QString key = words[0];
		if (key != "BRIDGE1.0") return false;
	}
	//桥名
	QString bridgeName;
	while (!aStream.atEnd())
	{
		linestr = aStream.readLine();
		re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString key = words[0];
			bridgeName = key;
			break;
		}
	}
	setName(bridgeName);

	initMinMaxPt();
	//其它关键字，/结束 
	while (!aStream.atEnd())
	{
		linestr = aStream.readLine();
		re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString key = words[0];
		//	if (key == "/") break;
			if (key == "CENTERPEG") m_centerPeg = readCenterPeg(aStream);
			else if (key == "SECTION") m_sectionNo = readSection(aStream);
			else if (key == "BASE") readBase(aStream);
			else if (key == "DOWN") readDown(aStream);
			else if (key == "UP") readUp(aStream);
			else if (key == "ANCSTRU") readAncillaryStructure(aStream);
		}
	}

	return true;
}

QString rdBridge::readCenterPeg(QTextStream& fin)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			return qstr;
		}
	}

	return "";
}

int rdBridge::readSection(QTextStream& fin)//读合同段
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			return qstr.toInt();
		}
	}

	return 0;
}

void rdBridge::readBase(QTextStream& fin)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			int type = 0;//默认类型为1
			if (words.size() > 1) type = words[1].toInt();

			if (qstr == "PILE")
			{
				if (type == 0)
				{
					QVector<_bridgepilePara> piles;
					readPiles(fin, piles);

					//生成桥桩数据
					createPiles(piles);
					piles.clear();
				}
			}
			else if (qstr == "PILEUP")
			{
				if (type == 0)//实体式承台
				{
					QVector<_solidplatformPara> platforms;
					readSolidPilePlatform(fin, platforms);

					//生成实体承台数据
					createSolidPilePlatform(platforms);
					platforms.clear();
				}
				else if(type == 1)//框架式承台
				{
					QVector<_frameplatformPara> platforms;
					readFramePilePlatform(fin, platforms);

					//生成实体承台数据
					createFramePilePlatform(platforms);
					platforms.clear();
				}

			}
		}
	}
}

void rdBridge::readDown(QTextStream& fin)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			int type = 0;//默认类型为1
			if (words.size() > 1) type = words[1].toInt();

			if (qstr == "PIER")//桥墩
			{
				if (type == 0)
				{
					QVector<_cylinderPierPara> cPiers;
					readCylinderPier(fin, cPiers);

					//生成桥墩数据
					createCylinderPier(cPiers);
					cPiers.clear();
				}
			}		
			else if (qstr == "ABUTMENT") //台身
			{
				if (type == 0)
				{
					QVector<_ribbedplatePara> ribbedplate;
					readRibbedPlateAbutment(fin, ribbedplate);

					//生成桥墩数据
					createRibbedPlateAbutment(ribbedplate);
					ribbedplate.clear();
				}
			}
			else if (qstr == "BENTCAP") //盖梁
			{
				if (type == 0)
				{
					QVector<_bentcapPara> bentcaps;
					readBentCap(fin, bentcaps);

					//生成桥墩数据
					createBentCap(bentcaps);
					bentcaps.clear();
				}
			}
			else if (qstr == "ABUTMENTCAP") //台帽
			{
				if (type == 0)
				{
					QVector<_abutmentcapPara> abutmentcaps;
					readAbutmentCap(fin, abutmentcaps);

					//生成桥墩数据
					createAbutmentCap(abutmentcaps);
					abutmentcaps.clear();
				}
			}
		}
	}
}

void rdBridge::readUp(QTextStream& fin)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			int type = 0;//默认类型为1
			if (words.size() > 1) type = words[1].toInt();

			if (qstr == "GIRDER")//主梁
			{
				if (type == 1)
				{
					_boxGirderDataPara  bgp;
					readBoxGirder(fin, bgp);

					//生成箱梁数据
					createBoxGirder(bgp);
				}
			}
		}
	}
}

void rdBridge::readAncillaryStructure(QTextStream& fin)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			int type = 0;//默认类型为1
			if (words.size() > 1) type = words[1].toInt();

			if (qstr == "CRASHBARRIER")//防撞护栏 
			{
				if (type == 0)//全封闭
				{
					_solidCrashBarrierDataPara  scbdp;
					readSolidCrashBarrier(fin, scbdp);

					//生成全封闭防撞护栏数据
					createSolidCrashBarrier(scbdp);
				}
			}
		}
	}
}

void rdBridge::readPiles(QTextStream& fin, QVector<_bridgepilePara>& piles)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			if (words.size() <7) continue;//数据不足

			_bridgepilePara bpp;
			bpp.name = words[0];
			bpp.flag = words[1].toInt();
			double x = words[3].toDouble();//float精度不够
			double y = words[2].toDouble();
			double z = words[4].toDouble();
			bpp.pos = QVector3D(x - StartX, y - StartY, z - StartZ);
			bpp.height = words[5].toFloat() / 100.0f;
			bpp.r = words[6].toFloat() / 100.0f;
			piles.push_back(bpp);
		}
	}
}

void rdBridge::readSolidPilePlatform(QTextStream& fin, QVector<_solidplatformPara>& platform)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			_solidplatformPara bpfp;
			bpfp.name = words[0];
			int rows = words[1].toInt();
			bpfp.rows = rows;
			int index = 2;
			int allcols = 0;
			for (int i = 0; i < rows; i++)
			{
				int cols = words[index].toInt();
				allcols += cols;
				bpfp.cols.push_back(cols);
				index++;
			}

			int dadasize = 5 + rows + allcols;
			if (words.size() < dadasize) continue;//数据不足
			for (int i = 0; i < allcols; i++)
			{
				bpfp.pilesname.push_back(words[index]);
				index++;
			}
			bpfp.length = words[index++].toFloat();
			bpfp.width = words[index++].toFloat();
			bpfp.height = words[index++].toFloat();
			
			platform.push_back(bpfp);
		}
	}
}

void rdBridge::readFramePilePlatform(QTextStream& fin, QVector<_frameplatformPara>& platforms)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;

			_frameplatformPara fpfp;
			fpfp.name = words[0];
			int rows = words[1].toInt();
			fpfp.rows = rows;
			int index = 2;
			int allcols = 0;
			for (int i = 0; i < rows; i++)
			{
				int cols = words[index].toInt();
				allcols += cols;
				fpfp.cols.push_back(cols);
				index++;
			}

			int dadasize = 5 + rows + allcols + 6;
			if (words.size() < dadasize) continue;//数据不足
			for (int i = 0; i < allcols; i++)
			{
				fpfp.pilesname.push_back(words[index]);
				index++;
			}

			fpfp.length = words[index++].toFloat() / 100.0f;
			for(int i=0; i<3;i++) fpfp.sublength[i] = words[index++].toFloat() / 100.0f;
			fpfp.width = words[index++].toFloat() / 100.0f;
			for (int i = 0; i < 3; i++) fpfp.subwidth[i] = words[index++].toFloat() / 100.0f;
			fpfp.height = words[index++].toFloat() / 100.0f;

			platforms.push_back(fpfp);
		}
	}
}

void rdBridge::readCylinderPier(QTextStream& fin, QVector<_cylinderPierPara>& piers)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 7;
			if (words.size() < dadasize) continue;//数据不足

			_cylinderPierPara cpp;
			cpp.name = words[0];
			cpp.lpilename = words[1];
			cpp.lr = words[2].toFloat();
			cpp.lheight = words[3].toFloat();
			cpp.rpilename = words[4];
			cpp.rr = words[5].toFloat();
			cpp.rheight = words[6].toFloat();			

			piers.push_back(cpp);
		}
	}
}

void rdBridge::readRibbedPlateAbutment(QTextStream& fin, QVector<_ribbedplatePara>& abutments)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 10;
			if (words.size() < dadasize) continue;//数据不足

			_ribbedplatePara rpp;
			rpp.name = words[0];
			rpp.lname0 = words[1];
			rpp.lname1 = words[2];
			rpp.rname0 = words[3];
			rpp.rname1 = words[4];
			rpp.upwidth = words[5].toFloat();
			rpp.downwidth = words[6].toFloat();
			rpp.length = words[7].toFloat();
			rpp.lheight = words[8].toFloat();
			rpp.rheight = words[9].toFloat();

			abutments.push_back(rpp);
		}
	}
}

void rdBridge::readBentCap(QTextStream& fin, QVector<_bentcapPara>& bentcaps)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 18;
			if (words.size() < dadasize) continue;//数据不足

			_bentcapPara bcp;
			bcp.name = words[0];
			bcp.piername = words[1];

			bcp.length = words[2].toFloat();
			bcp.width = words[3].toFloat();
			bcp.height1 = words[4].toFloat();
			bcp.height2 = words[5].toFloat();
			bcp.height3 = words[6].toFloat();
			bcp.height4 = words[7].toFloat();
			bcp.downlength = words[8].toFloat();
			bcp.uplength1 = words[9].toFloat();
			bcp.uplength2 = words[10].toFloat();
			bcp.uplength3 = words[11].toFloat();

			bcp.padstonecubecount = words[12].toInt();
			bcp.padstonecubewidth = words[13].toFloat();
			bcp.padstonecubeheight = words[14].toFloat();
			bcp.padstonecount = words[15].toInt();
			bcp.padstonegap = words[16].toFloat();
			bcp.padstonedis = words[17].toFloat();

			bentcaps.push_back(bcp);
		}
	}
}

//台帽
void rdBridge::readAbutmentCap(QTextStream& fin, QVector<_abutmentcapPara>& abutmentcaps)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 24;
			if (words.size() < dadasize) continue;//数据不足

			_abutmentcapPara acp;
			acp.name = words[0];
			acp.abutmentname = words[1];

			acp.length = words[2].toFloat();
			acp.width = words[3].toFloat();
			acp.offsetwidth = words[4].toFloat();
			acp.height1 = words[5].toFloat();
			acp.height2 = words[6].toFloat();
			acp.height3 = words[7].toFloat();
			acp.uplength1 = words[8].toFloat();
			acp.uplength2 = words[9].toFloat();
			acp.uplength3 = words[10].toFloat();
			acp.uplength4 = words[11].toFloat();

			acp.backwidth1 = words[12].toFloat();
			acp.backwidth2 = words[13].toFloat();

			acp.backheight1 = words[14].toFloat();
			acp.backheight2 = words[15].toFloat();
			acp.backheight3 = words[16].toFloat();
			acp.backheight4 = words[17].toFloat();

			acp.padstonecubecount = words[18].toInt();
			acp.padstonecubewidth = words[19].toFloat();
			acp.padstonecubeheight = words[20].toFloat();
			acp.padstonecount = words[21].toInt();
			acp.padstonegap = words[22].toFloat();
			acp.padstonedis = words[23].toFloat();

			abutmentcaps.push_back(acp);
		}
	}
}

//箱梁
void rdBridge::readBoxGirder(QTextStream& fin, _boxGirderDataPara& girder)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 2;
			if (words.size() < dadasize) continue;//数据不足

			girder.lenght = words[0].toDouble();
			girder.angle = words[1].toDouble();
		}
	}
}

//封闭式防撞护栏
void rdBridge::readSolidCrashBarrier(QTextStream& fin, _solidCrashBarrierDataPara& solidCrashBarrier)
{
	QVector<QString>  words;
	while (!fin.atEnd())
	{
		QString linestr = fin.readLine();
		int re = orQStrLine::AnayStringLine(linestr, words);
		if (re > 0)
		{
			QString qstr = words[0];
			if (qstr == "/") break;
			int dadasize = 2;
			if (words.size() < dadasize) continue;//数据不足

			solidCrashBarrier.length = words[0].toDouble();
			solidCrashBarrier.offset = words[1].toDouble();
		}
	}
}

void rdBridge::createPiles(QVector<_bridgepilePara>& piles)
{
	clear();

	for (int i = 0; i < piles.size(); i++)
	{
		_bridgepilePara bpp = piles[i];
		rdBridgeCylinderPile* pBridgeCylinderPile = new rdBridgeCylinderPile;
		pBridgeCylinderPile->setName(bpp.name);
		pBridgeCylinderPile->setHeight(bpp.height);
		pBridgeCylinderPile->setPos(bpp.pos);
		pBridgeCylinderPile->calMinMaxPt();
		pBridgeCylinderPile->seFlag(bpp.flag);
		
		addPile(pBridgeCylinderPile);
	}
}

void rdBridge::createSolidPilePlatform(QVector<_solidplatformPara>& platforms)
{
	for (int i = 0; i < platforms.size(); i++)
	{
		_solidplatformPara& bpfp = platforms[i];
		int rows = bpfp.rows;
		QVector<QPoint> pts;//每排首尾序号
		int start = 0;
		int end = 0;
		QPoint pt;
		if (rows > 0)
		{
			for (int j = 0; j < rows; j++)
			{
				pt.setX(start);
				int count = bpfp.cols[j];
				end = start + count - 1;
				pt.setY(end);

				pts.push_back(pt);
				start = end + 1;
			}
		}

		rdSolidPilePlatform* pSolid = new rdSolidPilePlatform;

		//计算中心坐标
		QVector3D centerPos = QVector3D(0,0,0);
		QVector3D dir;//方向以第1组为准
		for (int k = 0; k < pts.size(); k++)
		{
			int s = pts[k].x();
			int e = pts[k].y();

			QString sname = bpfp.pilesname[s];
			QString ename = bpfp.pilesname[e];
			//获取坐标
			QVector3D sPos = m_base.getPile(sname)->getPos();
			QVector3D ePos = m_base.getPile(ename)->getPos();

			centerPos = centerPos + (sPos + ePos) / 2 ;
			if (k > 0) centerPos = centerPos / 2;
			dir = sPos - ePos;
		}

		pSolid->setPos(centerPos);
		pSolid->setName(bpfp.name);
		pSolid->setDir(dir);
		pSolid->setLength(bpfp.length/100);
		pSolid->setWidth(bpfp.width/100);
		pSolid->setHeight(bpfp.height/100);

		addPileUp(pSolid);
	}
}

void rdBridge::createFramePilePlatform(QVector<_frameplatformPara>& platforms)
{
	for (int i = 0; i < platforms.size(); i++)
	{
		_frameplatformPara& fpfp = platforms[i];
		int rows = fpfp.rows;
		QVector<QPoint> pts;//每排首尾序号
		int start = 0;
		int end = 0;
		QPoint pt;
		if (rows > 0)
		{
			for (int j = 0; j < rows; j++)
			{
				pt.setX(start);
				int count = fpfp.cols[j];
				end = start + count - 1;
				pt.setY(end);

				pts.push_back(pt);
				start = end + 1;
			}
		}

		rdFramePilePlatform* pFrame = new rdFramePilePlatform;

		//计算中心坐标
		QVector3D centerPos = QVector3D(0, 0, 0);
		QVector3D dir;//方向以第1组为准
		for (int k = 0; k < pts.size(); k++)
		{
			int s = pts[k].x();
			int e = pts[k].y();

			QString sname = fpfp.pilesname[s];
			QString ename = fpfp.pilesname[e];
			//获取坐标
			QVector3D sPos = m_base.getPile(sname)->getPos();
			QVector3D ePos = m_base.getPile(ename)->getPos();

			centerPos = centerPos + (sPos + ePos) / 2;
			if (k > 0) centerPos = centerPos / 2;
			dir = sPos - ePos;
		}

		pFrame->setPos(centerPos);
		pFrame->setName(fpfp.name);
		pFrame->setDir(dir);
		pFrame->setLength(fpfp.length ,fpfp.sublength);
		pFrame->setWidth(fpfp.width,fpfp.subwidth);
		pFrame->setHeight(fpfp.height);

		addPileUp(pFrame);
	}
}

void rdBridge::createCylinderPier(QVector<_cylinderPierPara>& piers)
{
	for (int i = 0; i < piers.size(); i++)
	{
		_cylinderPierPara& cpp = piers[i];
		QString lname = cpp.lpilename;
		QString rname = cpp.rpilename;
		
		rdCylinderPier* pCylinderPier = new rdCylinderPier;
		pCylinderPier->setName(cpp.name);
		pCylinderPier->setLeftRadius(cpp.lr);
		pCylinderPier->setRightRadius(cpp.rr);
		pCylinderPier->setLeftHeight(cpp.lheight);
		pCylinderPier->setRightHeight(cpp.rheight);
		//获取坐标
		QVector3D lpos = m_base.getPile(lname)->getPos();
		QVector3D rpos = m_base.getPile(rname)->getPos();

		pCylinderPier->setLeftPos(lpos);
		pCylinderPier->setRightPos(rpos);

		addPier(pCylinderPier);
	}
}

void rdBridge::createRibbedPlateAbutment(QVector<_ribbedplatePara>& abutments)
{
	for (int i = 0; i < abutments.size(); i++)
	{
		_ribbedplatePara& rpp = abutments[i];

		rdRibbedPlateAbutment* pRibbedPlateAbutment = new rdRibbedPlateAbutment;
		pRibbedPlateAbutment->setName(rpp.name);
		pRibbedPlateAbutment->setWidth(rpp.upwidth, rpp.downwidth);
		pRibbedPlateAbutment->setLength(rpp.length);
		pRibbedPlateAbutment->setLeftHeight(rpp.lheight);
		pRibbedPlateAbutment->setRightHeight(rpp.rheight);

		QString shead = rpp.name[0];
		if (shead.toInt() > 0) pRibbedPlateAbutment->setBridgeHead(false);

		//获取坐标
		QString lname0 = rpp.lname0;
		QVector3D lpos0 = m_base.getPile(lname0)->getPos();
		QString lname1 = rpp.lname1;
		QVector3D lpos1 = m_base.getPile(lname1)->getPos();

		QString rname0 = rpp.rname0;
		QVector3D rpos0 = m_base.getPile(rname0)->getPos();
		QString rname1 = rpp.rname1;
		QVector3D rpos1 = m_base.getPile(rname1)->getPos();

		pRibbedPlateAbutment->setLeftPos(lpos0,lpos1);
		pRibbedPlateAbutment->setRightPos(rpos0,rpos1);

		addAbutment(pRibbedPlateAbutment);
	}
}

void rdBridge::createBentCap(QVector<_bentcapPara>& bentcaps)
{
	for (int i = 0; i < bentcaps.size(); i++)
	{
		_bentcapPara& bcp = bentcaps[i];

		rdBridgeBentCap* pBentCap = new rdBridgeBentCap;
		pBentCap->setName(bcp.name);

		pBentCap->setWidth(bcp.width);
		pBentCap->setLength(bcp.length);

		pBentCap->setHeight(bcp.height1, bcp.height2, bcp.height3, bcp.height4);
		pBentCap->setSubLength(bcp.downlength,bcp.uplength1, bcp.uplength2, bcp.uplength3);
		pBentCap->setPadStone(bcp.padstonecubecount, bcp.padstonecubewidth, bcp.padstonecubeheight, bcp.padstonecount, bcp.padstonegap,bcp.padstonedis);
		////获取桥墩位置
		QString piername = bcp.piername;
		rdBridgePier* pPier = m_down.getPier(piername);
		QVector3D lpos = pPier->getLeftPos();
		float lheight = pPier->getLeftHeight();
		pBentCap->setLeftPos(lpos + QVector3D(0, 0, lheight));
		QVector3D rpos = pPier->getRightPos();
		float rheight = pPier->getRightHeight();
		pBentCap->setRightPos(rpos + QVector3D(0, 0, rheight));

		QVector3D pos1 = m_base.getFirstLeftPilePos();
		QVector3D pos2 = m_base.getFirstRightPilePos();
		pBentCap->setDirPos(pos1, pos2);
		pBentCap->setPadStone(m_pPadStone);

		addBentCap(pBentCap);
	}
}

void rdBridge::createAbutmentCap(QVector<_abutmentcapPara>& abutmentcaps)
{
	for (int i = 0; i < abutmentcaps.size(); i++)
	{
		_abutmentcapPara& acp = abutmentcaps[i];

		rdBridgeAbutmentCap* pAbutmentCap = new rdBridgeAbutmentCap;
		pAbutmentCap->setName(acp.name);

		pAbutmentCap->setWidth(acp.width);
		pAbutmentCap->setOffsetWidth(acp.offsetwidth);
		pAbutmentCap->setLength(acp.length);

		pAbutmentCap->setHeight(acp.height1, acp.height2, acp.height3);
		pAbutmentCap->setSubLength(acp.uplength1, acp.uplength2, acp.uplength3, acp.uplength4);

		pAbutmentCap->setBackWidth(acp.backwidth1, acp.backwidth2);
		pAbutmentCap->setBackHeight(acp.backheight1, acp.backheight2, acp.backheight3, acp.backheight4);
		pAbutmentCap->setPadStone(acp.padstonecubecount,acp.padstonecubewidth,acp.padstonecubeheight,
			                      acp.padstonecount, acp.padstonegap, acp.padstonedis);
		

		QString shead = acp.name[0];
		if (shead.toInt() > 0) pAbutmentCap->setBridgeHead(false);

		//获取桥头肋板位置
		QString abutmentname = acp.abutmentname;
		rdBridgeAbutment* pAbutment = m_down.getAbutment(abutmentname);
		QVector3D lpos = pAbutment->getLeftPos();
		QVector3D rpos = pAbutment->getRightPos();
		float lh = pAbutment->getLeftHeight();
		float rh = pAbutment->getRightHeight();

		pAbutmentCap->setLeftPos(lpos + QVector3D(0, 0, lh));
		pAbutmentCap->setRightPos(rpos + QVector3D(0, 0, rh));
		pAbutmentCap->setPadStone(m_pPadStone);

		addAbutmentCap(pAbutmentCap);
	}
}

void rdBridge::createBoxGirder(_boxGirderDataPara& girder)
{
	//盖梁
	QVector <rdBridgeBentCap*>* pBentCaps = m_down.getBentCaps();
	for (int i = 0; i < pBentCaps->size(); i++)
	{
		QString name = (*pBentCaps)[i]->getName();
		int pos = name.indexOf('-');
		if (pos == -1) continue;
		QString strrow = name.left(pos);
		QString strcol = name.right(name.length() - pos - 1);
		int row = strrow.toInt();
		int col = strcol.toInt();
		int count0 = 0;
		QVector3D* pPadStonePos0 = (*pBentCaps)[i]->getPadStonePos(count0);
		float h0 = (*pBentCaps)[i]->getPadStoneHeight();
		col = col;
		//找row+1 col
		QString nextstrrow = QString::number(row + 1);
		QString nextname = nextstrrow + "-" + strcol;

		rdBridgeBentCap* pNextBentCap = m_down.getBentCap(nextname);
		if (pNextBentCap)
		{
			int count1 = 0;
			QVector3D* pPadStonePos1 = pNextBentCap->getPadStonePos(count1);
			float h1 = pNextBentCap->getPadStoneHeight();
			if (count1 == count0)
			{
				for (int j = 0; j < count0; j++)
				{
					QVector3D p0 = pPadStonePos0[j];					
					QVector3D p1 = pPadStonePos1[j];

					//角度
					QVector3D dir = p1 - p0;
					float angle = 0;
					if (dir.x() != 0)
					{
						float s = dir.y() / dir.x();
						angle = atan(s);
						angle = angle * 180 / 3.14;
					}

					double dis = p0.distanceToPoint(p1);
					dis = dis;

					rdBoxGirderData* pBoxGirderData = new rdBoxGirderData;
					QVector3D center = (p0 + p1) / 2;
					float h = (h0 + h1) / 2;
					center.setZ(center.z() + 2*h/3.0);
					pBoxGirderData->setPos(center);
					pBoxGirderData->setAngle(angle);

					p0.setZ(p0.z() + 2 * h / 3.0);
					p1.setZ(p1.z() + 2 * h / 3.0);
					pBoxGirderData->setFrontBackCenter(p0, p1);

					//高度变化倾斜角（x旋转）
					QVector2D pt20 = QVector2D(p0.x(), p0.y());
					QVector2D pt21 = QVector2D(p1.x(), p1.y());
					dis = pt20.distanceToPoint(pt21);
					double ss = (p1.z() - p0.z()) / dis;
					angle = atan(ss);
					angle = angle * 180 / 3.14;
					pBoxGirderData->setAngleX(angle);
					//取名字
					QString sname = name + "-" + QString::number(j);
					pBoxGirderData->setName(sname);

					if (j == 0)
					{
						if (col == 0)//左幅
						{
							pBoxGirderData->setFlag(1);
						}
						else//右幅
						{
							pBoxGirderData->setFlag(3);
						}
					}
					else if (j == (count0 - 1))
					{
						if (col == 0)//左幅
						{
							pBoxGirderData->setFlag(2);
						}
						else//右幅
						{
							pBoxGirderData->setFlag(4);
						}
					}
					else pBoxGirderData->setFlag(0);

					addBoxGirder(pBoxGirderData);
				}
			}
		}
	}

	//台帽
	QVector <rdBridgeAbutmentCap*>* AbutmentCaps = m_down.getAbutmentCaps();
	for (int i = 0; i < AbutmentCaps->size(); i++)
	{
		QString name = (*AbutmentCaps)[i]->getName();
		int pos = name.indexOf('-');
		if (pos == -1) continue;
		QString strrow = name.left(pos);
		QString strcol = name.right(name.length() - pos - 1);
		int row = strrow.toInt();
		int col = strcol.toInt();
		int count0 = 0;
		QVector3D* pPadStonePos0 = (*AbutmentCaps)[i]->getPadStonePos(count0);
		float h0 = (*pBentCaps)[i]->getPadStoneHeight();
		col = col;
		//找row+1 col
		QString nextstrrow = QString::number(row + 1);
		QString nextname = nextstrrow + "-" + strcol;
		if (row > 0)//桥尾
		{
			nextstrrow = QString::number(row - 1);
			nextname = nextstrrow + "-" + strcol;
		}

		//盖梁里找
		rdBridgeBentCap* pNextBentCap = m_down.getBentCap(nextname);
		if (pNextBentCap)
		{
			int count1 = 0;
			QVector3D* pPadStonePos1 = pNextBentCap->getPadStonePos(count1);
			float h1 = pNextBentCap->getPadStoneHeight();
			if (count1 == count0)
			{
				for (int j = 0; j < count0; j++)
				{
					QVector3D p0 = pPadStonePos0[j];
					QVector3D p1 = pPadStonePos1[j];

					if (row > 0)//桥尾
					{
						QVector3D temp = p0;
						p0 = p1;
						p1 = temp;
					}

					//角度
					QVector3D dir = p1 - p0;
					float angle = 0;
					if (dir.x() != 0)
					{
						float s = dir.y() / dir.x();
						angle = atan(s);
						angle = angle * 180 / 3.14;
					}

					double dis = p0.distanceToPoint(p1);
					dis = dis;

					rdBoxGirderData* pBoxGirderData = new rdBoxGirderData;
					QVector3D center = (p0 + p1) / 2;
					float h = (h0 + h1) / 2;
					center.setZ(center.z() + 2 * h / 3.0);
					pBoxGirderData->setPos(center);
					pBoxGirderData->setAngle(angle);

					p0.setZ(p0.z() + 2 * h / 3.0);
					p1.setZ(p1.z() + 2 * h / 3.0);
					pBoxGirderData->setFrontBackCenter(p0, p1);

					//高度变化倾斜角（x旋转）
					QVector2D pt20 = QVector2D(p0.x(), p0.y());
					QVector2D pt21 = QVector2D(p1.x(), p1.y());
					dis = pt20.distanceToPoint(pt21);
					double ss = (p1.z() - p0.z()) / dis;
					angle = atan(ss);
					angle = angle * 180 / 3.14;
					pBoxGirderData->setAngleX(angle);

					//取名字
					QString sname = name + "-" + QString::number(j);
					pBoxGirderData->setName(sname);

					if (j == 0)
					{
						if (col == 0)//左幅
						{
							pBoxGirderData->setFlag(1);
						}
						else//右幅
						{
							pBoxGirderData->setFlag(3);
						}
					}
					else if (j == (count0 - 1))
					{
						if (col == 0)//左幅
						{
							pBoxGirderData->setFlag(2);
						}
						else//右幅
						{
							pBoxGirderData->setFlag(4);
						}
					}
					else pBoxGirderData->setFlag(0);

					addBoxGirder(pBoxGirderData);
				}
			}
		}
	}
}

void rdBridge::createSolidCrashBarrier(_solidCrashBarrierDataPara& solidCrashBarrier)
{
	if (!m_pGirders) return;
	
	//获取箱梁表面模型的边界（分为左右侧）
	rdBridgeGirder *pGirder = m_pGirders->findGirder("BoxGirder", 0, 0);
	if (!pGirder) return;
	//左侧
	QVector3D leftpt[4];
	pGirder->getEdgePoints(1, leftpt);
	QVector3D rightpt[4];
	pGirder->getEdgePoints(2, rightpt);

	//获取实际位置及方向，进行计算防撞护栏的摆放位置
	QVector <rdBridgeGirderData*> *pGirders = m_up.getBridgeGirders();
	if (pGirders->size() == 0) return;

	double length = solidCrashBarrier.length;
	double offset = solidCrashBarrier.offset;

	for (int i = 0; i < pGirders->size(); i++)
	{
		rdBridgeGirderData* pGirderData = (*pGirders)[i];
		QVector3D pos = pGirderData->getPos();
		double angle = pGirderData->getAngle();
		double anglex = pGirderData->getAngleX();
		//计算4个点的实际坐标
		int flag = pGirderData->getFlag();
		if (flag == 0) continue;
		
		QMatrix4x4 rotateMatrix;
		rotateMatrix.rotate(angle + 90, 0, 0, 1);
		QMatrix4x4 rotatexMatrix;
		rotatexMatrix.rotate(anglex, 1, 0, 0);
		QMatrix4x4 modelMatrix;
		modelMatrix.translate(pos);

		QVector3D pts[4];
		bool bLeft = true;
		if (flag == 1 || flag == 3)//左侧
		{
			for (int j = 0; j < 4; j++)
				pts[j] = modelMatrix * rotateMatrix * rotatexMatrix * leftpt[j];
		}
		else if (flag == 2 || flag == 4)//右侧
		{
			for (int j = 0; j < 4; j++)
				pts[j] = modelMatrix * rotateMatrix * rotatexMatrix * rightpt[j];

			bLeft = false;
		}

		rdSolidCrashBarrierData* pSolidCrashBarrier = new rdSolidCrashBarrierData;
		double dis = pts[0].distanceToPoint(pts[1]);

		if (bLeft)
		{
			QVector3D fpt = pts[0] + (pts[1] - pts[0]) * (offset / dis);
			QVector3D bpt = pts[3] + (pts[2] - pts[3]) * (offset / dis);

			QVector3D center = (fpt + bpt) / 2;
			pSolidCrashBarrier->setPos(center);
		}
		else
		{
			QVector3D fpt = pts[1] + (pts[0] - pts[1]) * (offset / dis);
			QVector3D bpt = pts[2] + (pts[3] - pts[2]) * (offset / dis);

			QVector3D center = (fpt + bpt) / 2;
			pSolidCrashBarrier->setPos(center);
		}

		/*QVector3D pt = pts[3] - pts[0];
		float s = pt.z() / length;
		float anglex = atan(s);
		anglex = anglex * 180 / 3.14;	*/	
		pSolidCrashBarrier->setAngleX(anglex);
		pSolidCrashBarrier->setAngle(angle);
		pSolidCrashBarrier->setLength(length);
		if (bLeft)
			pSolidCrashBarrier->setFlag(1);
		else 
			pSolidCrashBarrier->setFlag(2);

		addCrashBarrier(pSolidCrashBarrier);
	}
}

void rdBridge::setCenterPeg(QString centerPeg)
{
	m_centerPeg = centerPeg;
}

QString rdBridge::getCenterPeg()
{
	return m_centerPeg;
}

void rdBridge::setSectionNo(int no)
{
	m_sectionNo = no;
}

int rdBridge::getSectionNo()
{
	return m_sectionNo;
}

void rdBridge::copy(rdBridge* pBridge)
{
	setName(pBridge->getName());
	setCenterPeg(pBridge->getCenterPeg());
	m_minPt = pBridge->getMinPt();
	m_maxPt = pBridge->getMaxPt();
	m_sectionNo = pBridge->getSectionNo();

	m_base.copy(pBridge->getBase());

}