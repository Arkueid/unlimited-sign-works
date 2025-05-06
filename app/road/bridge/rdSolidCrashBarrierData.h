#ifndef RDSOLIDCRASHBARRIERDATA_H
#define RDSOLIDCRASHBARRIERDATA_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QTextStream>
#include <QObject>
#include "geoSolid/geoCylinder.h"
#include "rdBridgeObject.h"
#include "rdBridgeGirder.h"
#include "../common/common.h"
#include "rdBridgeCrashBarrierData.h"
//箱梁
class rdSolidCrashBarrierData : public rdBridgeCrashBarrierData// protected QOpenGLFunctions
{
	//Q_OBJECT
public:
	rdSolidCrashBarrierData();
	virtual ~rdSolidCrashBarrierData(void);
	 
	void calCoord();

	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeCrashBarrierData* pData);

	void initGeometry();
private:
	QVector3D m_ldir;
	QVector3D m_rdir;

	QVector3D m_moutcorrs[12];
	QVector3D m_mincorrs[8];
	int m_mpointsize;

	QVector3D m_minRightPt;
	QVector3D m_maxRightPt;

	QOpenGLBuffer coorMidBuf;
	QOpenGLBuffer normMidBuf;
	QOpenGLBuffer boxMidBuf;

	QOpenGLBuffer coorSideBuf;
	QOpenGLBuffer normSideBuf;
	QOpenGLBuffer boxSideBuf;

	bool ptInRightBox(QVector3D pt);
	
};
#endif // RDSOLIDCRASHBARRIERDATA_H
