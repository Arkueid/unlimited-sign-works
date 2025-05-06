#ifndef RDCONTRACTSECTION_H
#define RDCONTRACTSECTION_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>
#include <QString>
#include <QTextStream>
#include <QDataStream>

#include "../bridge/rdBridge.h"

class rdContractSection : protected QOpenGLFunctions
{
public:
	rdContractSection();
	virtual ~rdContractSection(void);
	 
	void initGeometry();
	void renderBridge(QOpenGLShaderProgram* program);

	void clear();
	bool addBridge(rdBridge* pBridge);
private:
	QString m_name;
	int m_no;
	float m_startPeg;
	float m_endPeg;

	QVector<rdBridge*> m_bridges;
};
#endif // RDCONTRACTSECTION_H
