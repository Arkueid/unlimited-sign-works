#ifndef RDBRIDGECRASHBARRIERS_H
#define RDBRIDGECRASHBARRIERS_H

#include <QMatrix4x4>
#include <QString>
#include <QVector3D>
#include <QTextStream>
#include <QObject>
#include "geoSolid/geoCylinder.h"
#include "rdBridgeCrashBarrier.h"
#include "BridgeCrashBarrier.h"

class rdBridgeCrashBarriers// protected QOpenGLFunctions
{
public:
	rdBridgeCrashBarriers();
	virtual ~rdBridgeCrashBarriers(void);
	void addCrashBarrier(rdBridgeCrashBarrier* CrashBarrier);

	bool read(QString filename);
	bool readCrashBarrier(QTextStream &stream,int type);

	void initGeometry();

	rdBridgeCrashBarrier* findCrashBarrier(QString stype, double length, double angle);
private:
	QVector <rdBridgeCrashBarrier*> m_crashbarriers;

	bool readSolidCrashBarrier(QTextStream& stream);
};

#endif // RDBRIDGEGIRDERS_H
