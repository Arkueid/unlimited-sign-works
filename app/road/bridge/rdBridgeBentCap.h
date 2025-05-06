#ifndef RDBRIDGEBENTCAP_H
#define RDBRIDGEBENTCAP_H

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
#include "rdPadStone.h"

struct _bentcapPara
{
	QString name;
	QString piername;

	float length;
	float width;
    float height1;
	float height2;
	float height3;
	float height4;
	float downlength;
	float uplength1;	
	float uplength2;
	float uplength3;

	int padstonecubecount;
	float padstonecubewidth;
	float padstonecubeheight;
	int padstonecount;
	float padstonegap;
	float padstonedis;
};

class rdBridgeBentCap : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeBentCap();
	virtual ~rdBridgeBentCap(void);
	 
	void render(QOpenGLShaderProgram* program);

	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);
	void setLeftPos(QVector3D pos);
	void setRightPos(QVector3D pos);

	void setLength(float length);
	void setWidth(float width);
	void setHeight(float h1, float h2, float h3, float h4);
	void setSubLength(float downlength, float uplength1, float uplength2, float uplength3);

	void setDirPos(QVector3D leftPos, QVector3D rightPos);
	void setPadStone(int padstonecubecount, float padstonecubewidth, float padstonecubeheight, int padstonecount, float padstonegap,float dis);

	QVector3D getPos();
	void calMinMaxPt();
	void calCoord();
	void initGeometry();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeBentCap* pBentCap);

	void setPadStone(rdPadStone* pPadStone);
	
	QVector3D* getPadStonePos(int& count);
	float getPadStoneHeight();
protected:
	QVector3D m_center;
	QVector3D m_lpos;
	QVector3D m_rpos;

	//判别方向
	QVector3D m_dirLeftPos;//桥头（小桩号）左侧点
	QVector3D m_dirRightPos;//桥头（小桩号）左侧点

	float m_length;
	float m_width;
	float m_height1;
	float m_height2;
	float m_height3;
	float m_height4;
	float m_downlength;
	float m_uplength1;
	float m_uplength2;
	float m_uplength3;

	//垫石
	int m_padstonecubecount;
	float m_padstonecubewidth;
	float m_padstonecubelength;
	float m_padstonecubeheight;
	int m_padstonecount;
	float m_padstonegap;
	float m_padstonedis;
	float m_padstoneradius;
	QVector3D m_padstonecubePos[8];//最多8块
	QVector3D m_padstonePos[16];
	float m_angle;//旋转角度

	_rendpadstonepara m_rpsp[4];
	//

	QVector3D m_frontpts[16];
	QVector3D m_backpts[16];

	QOpenGLBuffer coorBuf;
	QOpenGLBuffer normBuf;

	rdPadStone* m_pPadStone;

private:
	void calPts(float width, bool bLeft, QVector3D&center, QVector2D& pt0, QVector2D& pt1);
};
#endif // RDBRIDGEBENTCAP_H
