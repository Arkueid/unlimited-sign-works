#ifndef RDBOXGIRDERDATA_H
#define RDBOXGIRDERDATA_H

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
#include "rdBridgeGirderData.h"
//箱梁
class rdBoxGirderData : public rdBridgeGirderData// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBoxGirderData();
	virtual ~rdBoxGirderData(void);
	 
	void calCoord();

	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);

	void setLeftDir(QVector3D dir);
	void setRightDir(QVector3D dir);
	void calMinMaxPt();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeGirderData* pData);

	void initGeometry();
private:
	QVector3D m_ldir;
	QVector3D m_rdir;

	QVector3D m_minRightPt;
	QVector3D m_maxRightPt;

	bool ptInRightBox(QVector3D pt);
	
};
#endif // RDBOXGIRDERDATA_H
