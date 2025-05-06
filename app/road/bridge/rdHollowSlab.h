#ifndef RDHOLLOWSLAB_H
#define RDHOLLOWSLAB_H

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
#include "BridgeGirder.h"

class rdHollowSlab : public rdBridgeGirder// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdHollowSlab();
	virtual ~rdHollowSlab(void);
	 
	void SetPara(_hollowSlabPara hsp);
	void calCoord();

	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeGirder* pSlab);

	void initGeometry();

	static bool readFile(QTextStream& fin, _hollowSlabPara& hsp);

	void getEdgePoints(int type, QVector3D* pts) = 0;
private:

	_hollowSlabPara m_hsp;

	QVector3D m_ldir;
	QVector3D m_rdir;

	QVector3D m_moutcorrs[10];
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
#endif // RDHOLLOWSLAB_H
