#ifndef RDBRIDGEUP_H
#define RDBRIDGEUP_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QDataStream>

#include "rdBridgeGirderData.h"

//上部结构：主梁、桥面铺装、人行道等
class rdBridgeUp : protected QOpenGLFunctions
{
public:
	rdBridgeUp();
	virtual ~rdBridgeUp(void);

	void clear();

	QVector<rdBridgeGirderData*>* getBridgeGirders();
	void addBridgeGirder(rdBridgeGirderData* pier);
	rdBridgeGirderData* getBridgeGirder(QString name);
	
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

	void copy(rdBridgeUp* pBridgeDown);

	void createVBO();
private:
	QVector <rdBridgeGirderData*> m_girderData;
	QVector3D m_minPt;
	QVector3D m_maxPt;
};
#endif // RDBRIDGEUP_H
