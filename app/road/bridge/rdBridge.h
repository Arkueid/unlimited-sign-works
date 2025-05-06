#ifndef RDBRIDGE_H
#define RDBRIDGE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QString>
#include <QTextStream>
#include <QDataStream>

#include "rdBridgePile.h"
#include "rdBridgeCylinderPile.h"
#include "rdBridgePileUp.h"
#include "rdBridgeBentCap.h"
#include "rdBridgeAbutmentCap.h"

#include "../geosolid/geoCylinder.h"
#include "../geosolid/geoCube.h"
#include "../common/common.h"
#include "rdBridgeBase.h"
#include "rdBridgeDown.h"
#include "rdBridgeUp.h"
#include "rdBridgeAncillaryStructures.h"

#include "rdBoxGirderData.h"
#include "rdSolidCrashBarrierData.h"

#include "rdBoxGirder.h"
#include "rdBridgeGirders.h"
#include "rdBridgeCrashBarriers.h"
#include "rdBridgeGirder.h"
#include "rdPadStone.h"

class rdBridge : protected QOpenGLFunctions
{
public:
	rdBridge();
	virtual ~rdBridge(void);
	 
	void initGeometry();
	void render(QOpenGLShaderProgram* program);

	void setCylinder(geoCylinder* pCylinder);
	void setCube(geoCube* pCube);
	void setPadStone(rdPadStone* pPadStone);
	void setBridgeGirders(rdBridgeGirders* pGirders);
	void setBridgeCrashBarriers(rdBridgeCrashBarriers* pCrashBarriers);

	void clear();
	void modifyMinMaxPt(QVector3D minpt, QVector3D maxpt);
	void initMinMaxPt();
	void calMinMaxPt();

	void setName(QString name);
	QString getName();
	QVector3D getMinPt();
	QVector3D getMaxPt();
	QVector<rdBridgePile*>* getPiles();
	rdBridgePile* getPile(QString name);

	//桥基础
	rdBridgeBase* getBase();	
	void addPile(rdBridgePile*);
	void addPileUp(rdBridgePileUp*);
	//桥下部结构
	rdBridgeDown* getDown();
	void addPier(rdBridgePier*);
	void addAbutment(rdBridgeAbutment*);
	void addBentCap(rdBridgeBentCap*);
	void addAbutmentCap(rdBridgeAbutmentCap*);
	//桥上部结构
	//箱梁
	void addBoxGirder(rdBoxGirderData*);
	//桥附属构造
	//防撞护栏
	void addCrashBarrier(rdBridgeCrashBarrierData*);

	bool ptInBox(QVector3D pt);
	rdBridgeObject* PtIn(QVector3D pt);

	void SetAllPiles(bool bShow);
	void setSectionShow(_sectionTypeValue stv, bool bSHow);

	bool save(QString fname);
	bool read(QString fname);
	bool inport(QTextStream& stream);
	void copy(rdBridge* pBridge);

	void setCenterPeg(QString centerPeg);
	QString getCenterPeg();

	void setSectionNo(int no);
	int getSectionNo();
	void setShow(bool bShow);
	bool getShow();
	
private:
	geoCylinder* m_pCylinder;
	geoCube* m_pCube;
	rdBridgeGirders* m_pGirders;
	rdBridgeCrashBarriers* m_pCrashBarriers;
	rdPadStone* m_pPadStone;

	rdBridgeBase m_base;//基础
	rdBridgeDown m_down;//下部构造
	rdBridgeUp m_up;//上部构造
	rdBridgeAncillaryStructures m_ancillaryStructures;//附属构造

	QVector3D m_minPt;
	QVector3D m_maxPt;

	QString m_name;
	QString m_centerPeg;
	int m_sectionNo;

	QString readCenterPeg(QTextStream& in);
	int readSection(QTextStream& stream);//读合同段
	void readBase(QTextStream& in);
	void readDown(QTextStream& in);
	void readUp(QTextStream& in);
	void readAncillaryStructure(QTextStream& in);
	//
	void readPiles(QTextStream& in, QVector<_bridgepilePara>& piles);
	void createPiles(QVector<_bridgepilePara>& piles);
	void readSolidPilePlatform(QTextStream& in, QVector<_solidplatformPara>& platforms);
	void createSolidPilePlatform(QVector<_solidplatformPara>& platforms);

	void readFramePilePlatform(QTextStream& in, QVector<_frameplatformPara>& platforms);
	void createFramePilePlatform(QVector<_frameplatformPara>& platforms);

	//桥墩
	void readCylinderPier(QTextStream& fin, QVector<_cylinderPierPara>& piers);
	void createCylinderPier(QVector<_cylinderPierPara>& piers);
	//肋板式桥台
	void readRibbedPlateAbutment(QTextStream& fin, QVector<_ribbedplatePara>& abutments);
	void createRibbedPlateAbutment(QVector<_ribbedplatePara>& abutments);
	//盖梁
	void readBentCap(QTextStream& fin, QVector<_bentcapPara>& abutments);
	void createBentCap(QVector<_bentcapPara>& abutments);
	//台帽
	void readAbutmentCap(QTextStream& fin, QVector<_abutmentcapPara>& abutments);
	void createAbutmentCap(QVector<_abutmentcapPara>& abutments);
	//箱梁
	void readBoxGirder(QTextStream& fin, _boxGirderDataPara& girder);
	void createBoxGirder(_boxGirderDataPara& girder);
	//封闭式防撞护栏
	void readSolidCrashBarrier(QTextStream& fin, _solidCrashBarrierDataPara& solidCrashBarrier);
	void createSolidCrashBarrier(_solidCrashBarrierDataPara& solidCrashBarrier);

	bool m_bShow; 	
};
#endif // RDBRIDGE_H
