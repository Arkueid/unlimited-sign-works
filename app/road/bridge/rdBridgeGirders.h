#ifndef RDBRIDGEGIRDERS_H
#define RDBRIDGEGIRDERS_H

#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QTextStream>
#include <QObject>
#include "../geoSolid/geoCylinder.h"
#include "rdBridgeGirder.h"
#include "BridgeGirder.h"

class rdBridgeGirders// protected QOpenGLFunctions
{
public:
	rdBridgeGirders();
	virtual ~rdBridgeGirders(void);
	void addGirder(rdBridgeGirder* pGirder);

	bool read(QString filename);
	bool readGirder(QTextStream &stream,int type);	

	void initGeometry();

	rdBridgeGirder* findGirder(QString stype, double length, double angle);
private:
	QVector <rdBridgeGirder*> m_girders;

	bool readHollowSlab(QTextStream& stream);
	bool readBoxGirder(QTextStream& stream);
};

#endif // RDBRIDGEGIRDERS_H
