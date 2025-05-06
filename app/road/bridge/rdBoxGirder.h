#ifndef RDBOXGIRDER_H
#define RDBOXGIRDER_H

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
//箱梁
class rdBoxGirder : public rdBridgeGirder// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBoxGirder();
	virtual ~rdBoxGirder(void);
	 
	void SetPara(_boxGirderPara bgp);
	void calCoord();
	void calMidCoord();
	void calSideCoord();
	void render(QOpenGLShaderProgram* program,int type);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeGirder* pSlab);

	void initGeometry();
	void initMidGeometry();
	void initSideGeometry();

	static bool readFile(QTextStream& fin, _boxGirderPara& bgp);

	void getEdgePoints(int type, QVector3D* pts);
private:

	_boxGirderPara m_bgp;

	QVector3D m_ldir;
	QVector3D m_rdir;

	//中间
	QVector3D m_moutcorrs[12];
	QVector3D m_mincorrs[8];
	int m_mpointsize;
	//边
	QVector3D m_soutcorrs[11];
	QVector3D m_sincorrs[8];
	int m_spointsize;

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
#endif // RDBOXGIRDER_H
