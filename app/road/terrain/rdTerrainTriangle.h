#ifndef TerrainTriangle_H
#define TerrainTriangle_H

#include <QVector3D>
#include <QVector2D>

class rdTerrainTriangle
{
public:
	rdTerrainTriangle();
	~rdTerrainTriangle();

	void setData(QVector3D*points);
	bool isInTriangle(QVector2D pp) const;
	bool isInBox(QVector2D pp) const;
	void calMinMax();

	float height(QVector2D pp);
	float heightFast(QVector2D pp);
	float area();

	QVector2D texture(QVector2D pp);//纹理
	QVector3D heightTexture(QVector2D pp);//高度和纹理
	//BOOL CalIntersectionWithLine(QVector3D p1, QVector3D p2, vector<_InterPos> & PosWithTerrain, vector<_InterPos> & PosWithLine);

	//BOOL ClipByQuad(_QuadLine& quad, vector<dvTerrainTriangle>& result);
	//BOOL IntersectionWithLine(QVector3D p1, QVector3D p2, vector<_InterPosTex> & PosWithTerrain, vector<_InterPos> & PosWithLine);
	//
	//bool ClipQuad(_QuadLine& quad, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris);
	//bool ClipQuadWithIn(_QuadLine& quad, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris);
	//BOOL ClipByLine(dvPoint3D p1, dvPoint3D p2, float *coef, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris);

	//bool isValidation();

	//BOOL IntersectionWithLineByTerrain(dvPoint3D p1, dvPoint3D p2, vector<_InterPosTex> & PosWithTerrain);
	//BOOL IntersectionWithLineByRoad(dvPoint3D p1, dvPoint3D p2, vector<_InterPosTex> & PosWithTerrain);

	//bool ClipByLine(double a, double b, double c, dvPoint3D refPt, dvPoint3D outp[10], int *outcount, bool  side);//三角形被直线裁剪
	void calOutsign();
	//bool IsOutWithQuad(vec2*pts);
	bool isAnticlockwise();
public:
	QVector3D m_point[3];//多边形顶点
	float   m_texCoor[3][2];
	float   m_mutitexWeight[3][4];
	bool m_bAnticlockwise;

	//包围盒
	double m_minx;
	double m_maxx;
	double m_miny;
	double m_maxy;
	double m_minz;
	double m_maxz;

	QVector3D   m_linecoef[3];//三条边所在直线的系数，ax+by+c=0；
	QVector3D   m_outsign;//三条边的外点正负符号
};

#endif // TerrainTriangle_H