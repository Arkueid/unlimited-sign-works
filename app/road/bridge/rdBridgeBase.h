#ifndef RDBRIDGEBASE_H
#define RDBRIDGEBASE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QDataStream>
#include "rdBridgePile.h"
#include "rdBridgePileUp.h"
#include "rdSolidPilePlatform.h"
#include "rdFramePilePlatform.h"

class rdBridgeBase : protected QOpenGLFunctions
{
public:
	rdBridgeBase();
	virtual ~rdBridgeBase(void);	

	void clear();

	QVector<rdBridgePile*>* getPiles();
	void AddPile(rdBridgePile* pile);
	rdBridgePile* getPile(QString name);

	QVector<rdBridgePileUp*>* getPileUps();
	void AddPileUp(rdBridgePileUp* pileup);
	rdBridgePileUp* getPileUp(QString name);

	rdBridgeObject* PtIn(QVector3D pt);
	void SetAllPiles(bool bShow);

	//最大最小值
	QVector3D getMinPt();
	QVector3D getMaxPt();
	void modifyMinMaxPt(QVector3D minpt, QVector3D maxpt);
	void initMinMaxPt();
	void calMinMaxPt();

	void save(QDataStream& outStream);
	void read(QDataStream& inStream);

	void copy(rdBridgeBase* pBridge);

	void createVBO();

	QVector3D getFirstLeftPilePos();
	QVector3D getFirstRightPilePos();
	QVector3D getLastLeftPilePos();
	QVector3D getLastRightPilePos();
private:
	//桩
	QVector<rdBridgePile*> m_piles;
	//承台、下系梁（桩基础上部位置）
	QVector <rdBridgePileUp*> m_pileups;

	QVector3D m_minPt;
	QVector3D m_maxPt;
};
#endif // RDBRIDGEBASE_H
