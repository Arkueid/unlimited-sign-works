#ifndef RDCYLINDERPIER_H
#define RDCYLINDERPIER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QDataStream>
#include <QObject>
#include "geoSolid/geoCylinder.h"
#include "rdBridgeObject.h"
#include "rdBridgePier.h"
#include "../common/common.h"

class rdCylinderPier : public rdBridgePier// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdCylinderPier();
	virtual ~rdCylinderPier(void);
	 
	void calCoord();

	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftPos(QVector3D pos);
	void setRightPos(QVector3D pos);

	QVector3D getLeftPos();
	QVector3D getRightPos();

	void setLeftRadius(float r);
	float getLeftRadius();
	void setRightRadius(float r);
	float getRightRadius();
	
	void setLeftHeight(float height);
	float getLeftHeight();
	void setRightHeight(float height);
	float getRightHeight();

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgePier* pPier);

	void initGeometry();
private:
	QVector3D m_lpos;
	QVector3D m_rpos;

	float m_lr;
	float m_lheight;	
	float m_curlheight;//施工
	float m_rr;
	float m_rheight;
	float m_currheight;//施工

	QVector3D m_ldir;
	QVector3D m_rdir;

	QVector3D m_lcorrs[CIRCLEPTS];
	QVector3D m_rcorrs[CIRCLEPTS];
	QVector3D m_norms[CIRCLEPTS+2];

	QVector3D m_minRightPt;
	QVector3D m_maxRightPt;

	QOpenGLBuffer coorlBuf;
	QOpenGLBuffer normlBuf;
	QOpenGLBuffer boxlBuf;

	QOpenGLBuffer coorrBuf;
	QOpenGLBuffer normrBuf;
	QOpenGLBuffer boxrBuf;

	bool ptInRightBox(QVector3D pt);
	
};
#endif // RDCYLINDERPIER_H
