#ifndef RDBRIDGEANCILLARYSTRUCTURES_H
#define RDBRIDGEANCILLARYSTRUCTURES_H

//桥梁附属构造物
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QDataStream>

//防撞护栏
#include "rdBridgeCrashBarrierData.h"

class rdBridgeAncillaryStructures : protected QOpenGLFunctions
{
public:
	rdBridgeAncillaryStructures();
	virtual ~rdBridgeAncillaryStructures(void);

	void clear();

	QVector<rdBridgeCrashBarrierData*>* getBridgeCrashBarriers();
	void addBridgeCrashBarrier(rdBridgeCrashBarrierData* pCrashBarrier);
	rdBridgeCrashBarrierData* getBridgeCrashBarrier(QString name);
	
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

	void copy(rdBridgeAncillaryStructures* pBridgeAncillaryStructures);

	void createVBO();
private:
	QVector <rdBridgeCrashBarrierData*> m_crashBarrierData;
	QVector3D m_minPt;
	QVector3D m_maxPt;
};
#endif // RDBRIDGEANCILLARYSTRUCTURES_H
