#ifndef RDSOLIDCRASHBARRIER_H
#define RDSOLIDCRASHBARRIER_H

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
#include "rdBridgeCrashBarrier.h"
#include "../common/common.h"
#include "bridgecrashbarrier.h"
//箱梁
class rdSolidCrashBarrier : public rdBridgeCrashBarrier// protected QOpenGLFunctions
{
	//Q_OBJECT
public:
	rdSolidCrashBarrier();
	virtual ~rdSolidCrashBarrier(void);
	 
	void SetPara(_solidcrashbarrierPara cbp);
	void calCoord();
	void render(QOpenGLShaderProgram* program,int type);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeCrashBarrier* SolidCrashBarrier);

	void initGeometry();

	static bool readFile(QTextStream& fin, _solidcrashbarrierPara& cbp);
	void setRight(bool bRight);
	bool getRight();
private:
	bool m_bRight;
	_solidcrashbarrierPara m_scbp;

	QVector3D m_ldir;
	QVector3D m_rdir;

	QVector3D m_rightcorrs[9];//右
	QVector3D m_leftcorrs[9];//左
	int m_pointsize;

	QVector3D m_minRightPt;
	QVector3D m_maxRightPt;

	QOpenGLBuffer coorRightBuf;
	QOpenGLBuffer normRightBuf;
	QOpenGLBuffer boxRightBuf;

	QOpenGLBuffer coorLeftBuf;
	QOpenGLBuffer normLeftBuf;
	QOpenGLBuffer boxLeftBuf;

	bool ptInRightBox(QVector3D pt);
	
};
#endif // RDSOLIDCRASHBARRIER_H
