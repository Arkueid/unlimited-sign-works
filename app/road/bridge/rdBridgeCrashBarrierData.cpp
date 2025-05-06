#include "rdBridgeCrashBarrierData.h"
#include "../common/common.h"

rdBridgeCrashBarrierData::rdBridgeCrashBarrierData()
{
	m_name = "1-1";
	//m_r = 0.50f;
	//m_height = 5.0f;
	m_center = QVector3D(0,0,0);
}


rdBridgeCrashBarrierData::~rdBridgeCrashBarrierData(void)
{
	
} 

void rdBridgeCrashBarrierData::setPos(QVector3D pos)
{
	m_center = pos;
}

void rdBridgeCrashBarrierData::setAngle(float angle)
{
	m_angle = angle;
}

void rdBridgeCrashBarrierData::setFlag(int flag)
{
	m_flag = flag;
}

int rdBridgeCrashBarrierData::getFlag()
{
	return m_flag;
}

QVector3D rdBridgeCrashBarrierData::getPos()
{
	return m_center;
}

float rdBridgeCrashBarrierData::getAngle()
{
	return m_angle;
}
//void rdBridgePile::calMinMaxPt()
//{
//	QVector3D pos = m_pos;
//	float minx = pos.x() - m_r - Offset;
//	float miny = pos.y() - m_r - Offset;
//	float maxx = pos.x() + m_r + Offset;
//	float maxy = pos.y() + m_r + Offset;
//	float maxz = pos.z() + Offset;
//	float minz = pos.z() - m_height - Offset;
//
//	m_minPt = QVector3D(minx, miny, minz);
//	m_maxPt = QVector3D(maxx, maxy, maxz);
//}

//bool rdBridgePile::ptIn(QVector3D pt)
//{
//	if (!ptInBox(pt)) return false;//包围盒测试
//
//	QVector3D center = m_pos - QVector3D(0, 0, m_height / 2);
//	float zmin = m_pos.z() - m_height;
//	float zmax = m_pos.z();
//
//	float offset = 0.1;
//
//	if (pt.z() > (zmax+offset) || pt.z() < (zmin-offset)) return false;
//	float r = (m_pos.x() - pt.x()) * (m_pos.x() - pt.x()) + (m_pos.y() - pt.y()) * (m_pos.y() - pt.y());
//
//	if (r > (m_r+offset) * (m_r+offset)) return false;
//
//	return true;
//}
//
//void rdBridgePile::render(QOpenGLShaderProgram* program, geoCylinder* pCylinder)
//{
//	if (!m_bShow) return;
//	//绘制桥
//	if (pCylinder)
//	{
//		QMatrix4x4 modelMatrix;
//		modelMatrix.translate(m_pos.x(), m_pos.y(), m_pos.z());
//		QMatrix4x4 scaleMatrix;
//		scaleMatrix.scale(m_r, m_r, -m_height);
//		modelMatrix = modelMatrix * scaleMatrix;
//		program->setUniformValue("ModelMatrix", modelMatrix);
//
//		pCylinder->render(program);
//	}
//}
//
//void rdBridgePile::renderBox(QOpenGLShaderProgram* program, geoCube* pCube)
//{
//	if (!m_bShow) return;
//
//	QVector3D size = (m_maxPt - m_minPt);
//	
//	QMatrix4x4 scaleMatrix;
//	scaleMatrix.scale(size);
//	QMatrix4x4 modelMatrix;
//	modelMatrix.translate(m_pos.x(),m_pos.y(),m_pos.z() - size.z()/2);
//
//	modelMatrix = modelMatrix * scaleMatrix;
//	//QMatrix4x4 mpv = pv * modelMatrix;
//	program->setUniformValue("Model", modelMatrix);
//	pCube->drawBox(program);
//}

bool rdBridgeCrashBarrierData::save(QDataStream& fout)
{
	//fout << m_name <<m_pos << m_r << m_height<< m_minPt<<m_maxPt;

	return true;
}

bool rdBridgeCrashBarrierData::read(QDataStream& fin)
{
	//fin >> m_name >> m_pos >> m_r >> m_height >> m_minPt >> m_maxPt;

	return true;
}

void rdBridgeCrashBarrierData::setLength(double length)
{
	m_length = length;
}

double rdBridgeCrashBarrierData::getLength()
{
	return m_length;
}

void rdBridgeCrashBarrierData::setAngleX(double angle)
{
	m_anglex = angle;
}

double rdBridgeCrashBarrierData::getAngleX()
{
	return m_anglex;
}