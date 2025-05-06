#ifndef RDBRIDGEDOWN_H
#define RDBRIDGEDOWN_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QDataStream>

#include "rdBridgePier.h"
#include "rdCylinderPier.h"
#include "rdBridgeAbutment.h"
#include "rdRibbedPlateAbutment.h"
#include "rdBridgeBentCap.h"
#include "rdBridgeAbutmentCap.h"

class rdBridgeDown : protected QOpenGLFunctions
{
public:
	rdBridgeDown();
	virtual ~rdBridgeDown(void);

	void clear();

	QVector<rdBridgePier*>* getPiers();
	void addPier(rdBridgePier* pier);
	rdBridgePier* getPier(QString name);

	QVector<rdBridgeAbutment*>* getAbutments();
	void addAbutment(rdBridgeAbutment* pAbutment);
	rdBridgeAbutment* getAbutment(QString name);

	//盖梁
	QVector <rdBridgeBentCap*>* getBentCaps();
	void addBentCap(rdBridgeBentCap* pBentCap);
	rdBridgeBentCap* getBentCap(QString name);

	//台帽
	QVector <rdBridgeAbutmentCap*>* getAbutmentCaps();
	void addAbutmentCap(rdBridgeAbutmentCap* pBentCap);
	rdBridgeAbutmentCap* getAbutmentCap(QString name);

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

	void copy(rdBridgeDown* pBridgeDown);

	void createVBO();
private:
	//桥墩
	QVector <rdBridgePier*> m_piers;
	//桥台
	QVector < rdBridgeAbutment*> m_abutments;
	//盖梁
	QVector <rdBridgeBentCap*> m_bentcaps;
	//台帽
	QVector <rdBridgeAbutmentCap*> m_abutmentcaps;

	QVector3D m_minPt;
	QVector3D m_maxPt;
};
#endif // RDBRIDGEDOWN_H
