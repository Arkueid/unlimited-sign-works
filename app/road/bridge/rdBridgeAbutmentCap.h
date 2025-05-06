#ifndef RDBRIDGEABUTMENTCAP_H
#define RDBRIDGEABUTMENTCAP_H

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

//台帽
struct _abutmentcapPara
{
	QString name;
	QString abutmentname;

	float length;
	float width;
	float offsetwidth;
    float height1;
	float height2;
	float height3;

	float uplength1;	
	float uplength2;
	float uplength3;
	float uplength4;

	float backwidth1;
	float backwidth2;
	float backheight1;
	float backheight2;
	float backheight3;
	float backheight4;

	//垫石
	int padstonecubecount;
	float padstonecubewidth;
	float padstonecubeheight;
	int padstonecount;
	float padstonegap;
	float padstonedis;
};

class rdBridgeAbutmentCap : public rdBridgeObject// protected QOpenGLFunctions
{
	Q_OBJECT
public:
	rdBridgeAbutmentCap();
	virtual ~rdBridgeAbutmentCap(void);
	 
	void setBridgeHead(bool bHead);
	void render(QOpenGLShaderProgram* program);
	void renderBox(QOpenGLShaderProgram* program, geoCube* pCube);
	bool ptIn(QVector3D pt);
	void setLeftPos(QVector3D pos);
	void setRightPos(QVector3D pos);

	void setLength(float length);
	void setWidth(float width);
	void setOffsetWidth(float width);
	void setHeight(float h1, float h2, float h3);
	void setSubLength(float uplength1, float uplength2, float uplength3, float uplength4);
	void setBackWidth(float w1, float w2);
	void setBackHeight(float h1, float h2, float h3, float h4);

	void setPadStone(int padstonecubecount,float padstonecubewidth,float padstonecubeheight,int padstonecount,
	                 float padstonegap,float padstonedis);

	QVector3D getPos();
	void calMinMaxPt();
	void calCoord();
	void initGeometry();

	bool save(QDataStream& fout);
	bool read(QDataStream& fin);

	void copy(rdBridgeAbutmentCap* pBentCap);
	
	void setPadStone(rdPadStone* pPadStone);

	QVector3D* getPadStonePos(int& count);
	float getPadStoneHeight();
protected:
	bool m_bHead;

	QVector3D m_center;
	QVector3D m_lpos;
	QVector3D m_rpos;

	float m_length;
	float m_width;
	float m_offsetwidth;
	float m_height1;
	float m_height2;
	float m_height3;

	float m_uplength1;
	float m_uplength2;
	float m_uplength3;
	float m_uplength4;

	float m_backwidth1;
	float m_backwidth2;
	float m_backheight1;
	float m_backheight2;
	float m_backheight3;
	float m_backheight4;

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

	QVector3D m_frontpts[14];
	QVector3D m_backpts[14];

	QVector3D m_backleftpts[8];
	QVector3D m_backrightpts[8];

	QOpenGLBuffer coorBuf;
	QOpenGLBuffer normBuf;

	rdPadStone* m_pPadStone;
private:
	void calPts(float width, bool bLeft, QVector3D& center, QVector2D& pt0, QVector2D& pt1);
};
#endif // RDBRIDGEABUTMENTCAP_H
