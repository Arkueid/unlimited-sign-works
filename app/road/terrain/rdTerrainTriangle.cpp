#include "rdTerrainTriangle.h"
//#include "dvLineSegment.h"
//#include "dvQuad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

rdTerrainTriangle::rdTerrainTriangle()
{
}


rdTerrainTriangle::~rdTerrainTriangle()
{
}

void rdTerrainTriangle::setData(QVector3D*points)
{
	int i;
	m_maxx = m_maxy = -99999999;
	m_minx = m_miny = 999999999;
	m_minz = 999999999;
	m_maxz = -999999999;

	for (i = 0; i<3; i++)
	{
		m_point[i] = points[i];
		if (points[i].x()>m_maxx) m_maxx = points[i].x();
		if (points[i].y()>m_maxy) m_maxy = points[i].y();
		if (points[i].z()>m_maxz) m_maxz = points[i].z();

		if (points[i].x()<m_minx) m_minx = points[i].x();
		if (points[i].y()<m_miny) m_miny = points[i].y();
		if (points[i].z()<m_minz) m_minz = points[i].z();	
	}
}

bool rdTerrainTriangle::isInBox(QVector2D pp) const
{
	if (pp.x() > m_maxx || pp.x() < m_minx) return false;
	if (pp.y() > m_maxy || pp.y() < m_miny) return false;

	return true;
}

float rdTerrainTriangle::area()
{
	//海伦公式
	double p0p1 = sqrt((m_point[0].x() - m_point[1].x())*(m_point[0].x() - m_point[1].x())
		             + (m_point[0].y() - m_point[1].y())*(m_point[0].y() - m_point[1].y())
		             /*+ (m_point[0].z() - m_point[1].z())*(m_point[0].z() - m_point[1].z())*/);

	double p1p2 = sqrt((m_point[1].x() - m_point[2].x())*(m_point[1].x() - m_point[2].x())
		             + (m_point[1].y() - m_point[2].y())*(m_point[1].y() - m_point[2].y())
		             /*+ (m_point[1].z() - m_point[2].z())*(m_point[1].z() - m_point[2].z())*/);

	double p2p0 = sqrt((m_point[2].x() - m_point[0].x())*(m_point[2].x() - m_point[0].x())
		             + (m_point[2].y() - m_point[0].y())*(m_point[2].y() - m_point[0].y())
		             /*+ (m_point[2].z() - m_point[0].z())*(m_point[2].z() - m_point[0].z())*/);

	double p = (p0p1 + p1p2 + p2p0) / 2;

	return  sqrt(p * (p - p0p1) * (p - p1p2) * (p - p2p0));
}

//BOOL dvTerrainTriangle::CalIntersectionWithLine(dvPoint3D p1, dvPoint3D p2, vector<_InterPos>& PosWithTerrain, vector<_InterPos>& PosWithLine)
//{
//	BOOL flag = FALSE;
//	int j = 0;
//	for (int i = 0; i < 3; i++) {
//		if (i == 2) j = 0;
//		else j = i + 1;
//		float D = p1.x()*(m_point[j].y() - m_point[i].y()) + p2.x()*(m_point[i].y() - m_point[j].y())
//			+ m_point[i].x()*(p1.y() - p2.y()) + m_point[j].x()*(p2.y() - p1.y());
//		if (D != 0) {
//			float t0 = (p1.x()*(m_point[j].y() - m_point[i].y()) + m_point[i].x()*(p1.y() - m_point[j].y()) + m_point[j].x()*(m_point[i].y() - p1.y())) / D;
//			float t1 = -(p1.x()*(m_point[i].y() - p2.y()) + p2.x()*(p1.y() - m_point[i].y()) + m_point[i].x()*(p2.y() - p1.y())) / D;
//			if (t0 >= 0 && t0 <= 1 && t1 >= 0 && t1 <= 1)
//			{
//				flag = TRUE;
//				float x = p1.x() + t0*(p2.x() - p1.x());
//				float y = p1.y() + t0*(p2.y() - p1.y());
//				//求地形三角形上对应交点
//				_InterPos pp;
//				float h = m_point[i].z() + t1*(m_point[j].z() - m_point[i].z());
//				pp.p = dvPoint3D(x, y, h);
//				pp.t = t0;
//				PosWithTerrain.push_back(pp);
//				//求直线上对应交点
//				_InterPos pp1;
//				float hh = p1.z() + t0*(p2.z() - p1.z());
//				pp1.p = dvPoint3D(x, y, hh);
//				pp1.t = t0;
//				PosWithLine.push_back(pp1);
//			}
//		}
//	}
//	return flag;
//}

//pp点在三角形内（XY平面），计算高度
float rdTerrainTriangle::heightFast(QVector2D pp)
{
	float h = 0;
	h = (m_point[0].z() + m_point[1].z() + m_point[2].z()) / 3;
	return h;
}

QVector2D rdTerrainTriangle::texture(QVector2D pp)//纹理
{
	QVector2D st;
	float x = pp.x();
	float y = pp.y();
	float a, b, c;

	float  s = area();

	rdTerrainTriangle dvPBC;
	rdTerrainTriangle dvPAC;
	rdTerrainTriangle dvPAB;
	QVector3D pts[3];
	pts[0] = QVector3D(pp.x(), pp.y(), 0);
	pts[1] = m_point[1];
	pts[2] = m_point[2];
	dvPBC.setData(pts);

	pts[1] = m_point[0];
	dvPAC.setData(pts);

	pts[2] = m_point[1];
	dvPAB.setData(pts);

	float as = dvPBC.area();
	float bs = dvPAC.area();
	float cs = dvPAB.area();

	a = as / s;
	b = bs / s;
	c = cs / s;

	st = a* QVector2D(m_texCoor[0][0], m_texCoor[0][1] ) + b* QVector2D(m_texCoor[1][0],m_texCoor[1][1])
		+ c* QVector2D(m_texCoor[2][0], m_texCoor[2][1]);

	return st;
}

//pp点在三角形内（XY平面），计算高度
float rdTerrainTriangle::height(QVector2D pp)
{
	float h = 0;
	float x = pp.x();
	float y = pp.y();
	float a, b, c;

	float  s = area();

	rdTerrainTriangle dvPBC;
	rdTerrainTriangle dvPAC;
	rdTerrainTriangle dvPAB;
	QVector3D pts[3];
	pts[0] = QVector3D(pp.x(), pp.y(), 0);
	pts[1] = m_point[1];
	pts[2] = m_point[2];
	dvPBC.setData(pts);

	pts[1] = m_point[0];
	dvPAC.setData(pts);

	pts[2] = m_point[1];
	dvPAB.setData(pts);

	float as = dvPBC.area();
	float bs = dvPAC.area();
	float cs = dvPAB.area();

	a = as / s;
	b = bs / s;
	c = cs / s;

	h = a*m_point[0].z() + b*m_point[1].z() + c*m_point[2].z();

	//h = (m_point[0].z() + m_point[1].z() + m_point[2].z()) / 3;

	return h;
}

QVector3D rdTerrainTriangle::heightTexture(QVector2D pp)//高度和纹理
{
	float h = 0;
	float x = pp.x();
	float y = pp.y();
	float a, b, c;

	float  s = area();

	rdTerrainTriangle dvPBC;
	rdTerrainTriangle dvPAC;
	rdTerrainTriangle dvPAB;
	QVector3D pts[3];
	pts[0] = QVector3D(pp.x(), pp.y(), 0);
	pts[1] = m_point[1];
	pts[2] = m_point[2];
	dvPBC.setData(pts);

	pts[1] = m_point[0];
	dvPAC.setData(pts);

	pts[2] = m_point[1];
	dvPAB.setData(pts);

	float as = dvPBC.area();
	float bs = dvPAC.area();
	float cs = dvPAB.area();

	a = as / s;
	b = bs / s;
	c = cs / s;

	h = a*m_point[0].z() + b*m_point[1].z() + c*m_point[2].z();
	float ts = a*m_texCoor[0][0] + b*m_texCoor[1][0] + c*m_texCoor[2][0];
	float tt = a*m_texCoor[0][1] + b*m_texCoor[1][1] + c*m_texCoor[2][1];

	return QVector3D(h, ts, tt);
}

bool rdTerrainTriangle::isAnticlockwise()
{
	QVector3D pts[3];
	pts[0] = m_point[0];
	pts[1] = m_point[1];
	pts[2] = m_point[2];
	pts[0].setZ(0);
	pts[1].setZ(0);
	pts[2].setZ(0);

	QVector3D p1p2 = pts[1] - pts[0];
	QVector3D p2p3 = pts[2] - pts[1];
	QVector3D p3p1 = pts[0] - pts[2];
	
	QVector3D v[3];
	v[0] = QVector3D::crossProduct(p1p2, p2p3);
	v[1] = QVector3D::crossProduct(p2p3, p3p1);
	v[2] = QVector3D::crossProduct(p3p1, p1p2);

	m_bAnticlockwise = false;

	if (v[0].z() > 0 && v[0].z() > 0 && v[0].z() > 0)
	{
		m_bAnticlockwise = true;
	}

	return m_bAnticlockwise;
}
//
//bool rdTerrainTriangle::IsInTriangle(QVector2D pp) const
//{
//	if (!IsInBox(pp)) return false;
//
//	dvPoint2D PA = m_point[0] - pp;
//	dvPoint2D PB = m_point[1] - pp;
//	dvPoint2D PC = m_point[2] - pp;
//
//	double t1 = PA.Cross(PB);
//
//	double t2 = PB.Cross(PC);
//
//	double t3 = PC.Cross(PA);
//
//	return (t1*t2 >= 0 && t1*t3 >= 0);
//}
////struct _Quad
////{
////	vec3 pt[4];
////	vec2 minXY;
////	vec2 maxXY;
////};
//
//BOOL dvTerrainTriangle::IntersectionWithLine(dvPoint3D p1, dvPoint3D p2, vector<_InterPosTex> & PosWithTerrain, vector<_InterPos> & PosWithLine)
//{
//	BOOL flag = FALSE;
//
//	float fmaxx, fmaxy, fminx, fminy;
//	fminx = fmaxx = p1.x();
//	fminy = fmaxy = p1.y();
//	if (p2.x() < fminx) fminx = p2.x();
//	if (p2.x() > fmaxx) fmaxx = p2.x();
//	if (p2.y() < fminy) fminy = p2.y();
//	if (p2.y() > fmaxy) fmaxy = p2.y();
//
//	if (fminx > m_maxx || fminy > m_maxy) return FALSE;
//	if (fmaxx < m_minx || fmaxy < m_miny) return FALSE;
//
//	BOOL bIn1 = IsInTriangle(dvPoint2D(p1.x(), p1.y()));
//	BOOL bIn2 = IsInTriangle(dvPoint2D(p2.x(), p2.y()));
//	if (bIn1&&bIn2)
//	{
//		_InterPos ip;
//		ip.p = p1;
//		ip.t = 0;
//		PosWithLine.push_back(ip);
//		ip.p = p2;
//		ip.t = 1;
//		PosWithLine.push_back(ip);
//
//		_InterPosTex ipt;
//		vec3 ht = HeightTexture(dvPoint2D(p1.x(), p1.y()));
//		ipt.p = dvPoint3D(p1.x(), p1.y(), ht.x);
//		ipt.s = ht.y;
//		ipt.t = ht.z;
//		PosWithTerrain.push_back(ipt);
//
//		ht = HeightTexture(dvPoint2D(p2.x(), p2.y()));
//		ipt.p = dvPoint3D(p2.x(), p2.y(), ht.x);
//		ipt.s = ht.y;
//		ipt.t = ht.z;
//		PosWithTerrain.push_back(ipt);
//
//		return true;
//	}
//	else if (bIn1)
//	{
//		_InterPos ip;
//		ip.p = p1;
//		ip.t = 0;
//		PosWithLine.push_back(ip);
//
//		_InterPosTex ipt;
//		vec3 ht = HeightTexture(dvPoint2D(p1.x(), p1.y()));
//		ipt.p = dvPoint3D(p1.x(), p1.y(), ht.x);
//		ipt.s = ht.y;
//		ipt.t = ht.z;
//		PosWithTerrain.push_back(ipt);
//	}
//	else if (bIn2)
//	{
//		_InterPos ip;
//		ip.p = p2;
//		ip.t = 1;
//		PosWithLine.push_back(ip);
//
//		_InterPosTex ipt;
//		vec3 ht = HeightTexture(dvPoint2D(p2.x(), p2.y()));
//		ipt.p = dvPoint3D(p2.x(), p2.y(), ht.x);
//		ipt.s = ht.y;
//		ipt.t = ht.z;
//		PosWithTerrain.push_back(ipt);
//	}
//
//	float a, b, c;
//	a = p1.y() - p2.y();
//	b = p2.x() - p1.x();
//	c = p1.x()*p2.y() - p2.x()*p1.y();
//
//	for (int i = 0; i < 3; i++)
//	{
//		dvPoint3D pp1 = m_point[i];
//		dvPoint3D pp2 = m_point[(i + 1) % 3];
//		vec2 tex1 = vec2(m_texCoor[i][0], m_texCoor[i][1]);
//		vec2 tex2 = vec2(m_texCoor[(i + 1) % 3][0], m_texCoor[(i + 1) % 3][1]);
//
//		float  fsign1 = pp1.x()*a + pp1.y()*b + c;
//		float  fsign2 = pp2.x()*a + pp2.y()*b + c;
//
//		if (fsign1*fsign2 > 0) continue;//无交点
//
//		if (fsign1 == 0 && fsign2 == 0)//在线上
//		{
//			_InterPos ip;
//			ip.p = p1;
//			ip.t = 0;
//			PosWithLine.push_back(ip);
//			ip.p = p2;
//			ip.t = 1;
//			PosWithLine.push_back(ip);
//
//			_InterPosTex ipt;
//			float t = (pp2.x() - p1.x()) / (pp2.x() - pp1.x());
//			float z = pp1.z() + (pp2.z() - pp1.z())*t;
//			float ts = tex1.s + (tex2.s -tex1.s)*t;
//			float tt = tex1.t + (tex2.t - tex1.t)*t;
//
//			ipt.p = p1;
//			ipt.p.z(z);
//			ipt.t = tt;
//			ipt.s = ts;
//			PosWithTerrain.push_back(ipt);
//
//			t = (pp2.x() - p2.x()) / (pp2.x() - pp1.x());
//			z = pp1.z() + (pp2.z() - pp1.z())*t;
//			ts = tex1.s + (tex2.s - tex1.s)*t;
//			tt = tex1.t + (tex2.t - tex1.t)*t;
//
//			ipt.p = p2;
//			ipt.p.z(z);
//			ipt.t = tt;
//			ipt.s = ts;
//			PosWithTerrain.push_back(ipt);
//
//			return TRUE;
//		}
//		else if(fsign1 == 0)
//		{
//			_InterPos ip;
//			ip.p = p1;
//			ip.t = 0;
//			PosWithLine.push_back(ip);
//
//			_InterPosTex ipt;
//			float t = (pp2.x() - p1.x()) / (pp2.x() - pp1.x());
//			float z = pp1.z() + (pp2.z() - pp1.z())*t;
//			float ts = tex1.s + (tex2.s - tex1.s)*t;
//			float tt = tex1.t + (tex2.t - tex1.t)*t;
//
//			ipt.p = p1;
//			ipt.p.z(z);
//			ipt.t = tt;
//			ipt.s = ts;
//			PosWithTerrain.push_back(ipt);
//		}
//		else  if (fsign2 == 0)
//		{
//			_InterPos ip;
//			ip.p = p2;
//			ip.t = 1;
//			PosWithLine.push_back(ip);
//		
//			_InterPosTex ipt;
//			float t = (pp2.x() - p2.x()) / (pp2.x() - pp1.x());
//			float z = pp1.z() + (pp2.z() - pp1.z())*t;
//			float ts = tex1.s + (tex2.s - tex1.s)*t;
//			float tt = tex1.t + (tex2.t - tex1.t)*t;
//
//			ipt.p = p2;
//			ipt.p.z(z);
//			ipt.t = tt;
//			ipt.s = ts;
//			PosWithTerrain.push_back(ipt);
//		}
//		else//计算交点
//		{
//			float D0 = (p2.x() - p1.x())*(pp1.y() - pp2.y()) + (p2.y() - p1.y())*(pp2.x() - pp1.x());
//
//			if (D0 == 0)
//				D0 = 0;
//			
//			float  t1 = ((pp1.x() - p1.x())*(pp1.y() - pp2.y()) + (pp1.y() - p1.y())*(pp2.x() - pp1.x())) / D0;
//			float  t2 = ((p2.x() - p1.x())*(pp1.y() - p1.y()) - (p2.y() - p1.y())*(pp1.x() - p1.x())) / D0;
//
//			if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)//真正交点
//			{
//				float x = p1.x() + t1*(p2.x() - p1.x());
//				float y = p1.y() + t1*(p2.y() - p1.y());
//				float z1 = p1.z() + t1*(p2.z() - p1.z());
//				float z2 = pp1.z() + t2*(pp2.z() - pp1.z());
//
//				float ts = tex1.s + (tex2.s - tex1.s)*t2;
//				float tt = tex1.t + (tex2.t - tex1.t)*t2;
//
//				_InterPos ip;
//				ip.t = t1;
//				ip.p = dvPoint3D(x, y, z1);
//				PosWithLine.push_back(ip);
//
//				_InterPosTex ipt;
//				ipt.p = ip.p;
//				ipt.p.z(z2);
//				ipt.t = tt;
//				ipt.s = ts;
//				PosWithTerrain.push_back(ipt);
//
//				flag = TRUE;
//			}
//		}
//	}
//	
//	return flag;
//}
//
//BOOL dvTerrainTriangle::ClipByQuad(_QuadLine& quad, vector<dvTerrainTriangle>& result)
//{
//	if (quad.minXY.x > m_maxx || quad.minXY.y > m_maxy) return FALSE;//不裁剪
//	if (quad.maxXY.x < m_minx || quad.maxXY.y < m_miny) return FALSE;//不裁剪
//
//	if (m_minx >= quad.minXY.x && m_maxx <= quad.maxXY.x &&m_miny >= quad.minXY.y && m_maxy <= quad.maxXY.y) return TRUE;//完全落在窗口内删除
//		
//	vector<_3DPtWithTexture> outpoint, inpoint,tempinpoint;
//	vector<float> tlist1, tlist3;
//
//	for (int i = 0; i < 3; i++)
//	{
//		_3DPtWithTexture pt;
//		
//		pt.pt = m_point[i];
//		pt.tex.s = m_texCoor[i][0];
//		pt.tex.t = m_texCoor[i][1];
//
//		tempinpoint.push_back(pt);
//	}
//
//	for (int i = 0; i < 4; i++)
//	{
//		vec3 p0 = quad.pt[i];
//		vec3 p1 = quad.pt[(i+1)%4];
//
//		if (p0 == p1) continue;
//
//		vec3 p2 = quad.pt[(i + 2) % 4];
//		float fsign = quad.a[i] * p2.x + quad.b[i] * p2.y + quad.c[i];
//
//		int size = tempinpoint.size(); 
//		for (int j = 0; j < size; j++)
//		{			
//			dvPoint3D pp0 = tempinpoint[j].pt;
//			dvPoint3D pp1 = tempinpoint[(j+1)%size].pt;
//
//			//m_texCoor[3][2];
//			float texs0 = tempinpoint[j].tex.s;
//			float texs1 = tempinpoint[(j + 1) % size].tex.s;
//			float text0 = tempinpoint[j].tex.t;
//			float text1 = tempinpoint[(j + 1) % size].tex.t;;
//
//			float ss0 = quad.a[i] * pp0.x() + quad.b[i] * pp0.y() + quad.c[i];
//			float ss1 = quad.a[i] * pp1.x() + quad.b[i] * pp1.y() + quad.c[i];
//			float ss2 = quad.a[i] * tempinpoint[(j + 2) % size].pt.x() + quad.b[i] * tempinpoint[(j + 2) % size].pt.y() + quad.c[i];
//			if (/*fabs(ss0) < 0.01 &&*/ fabs(ss1) < 0.01 )//在线上
//			{
//				inpoint.push_back(tempinpoint[(j + 1) % size]);
//				outpoint.push_back(tempinpoint[(j + 1) % size]);
//			}
//						
//			//同在内侧
//			else if (fsign*ss0 >= 0 && fsign*ss1 >= 0)//同在内侧
//			{
//				//inpoint.push_back(pp0);
//				inpoint.push_back(tempinpoint[(j + 1) % size]);
//			}
//			else if (fsign*ss0 <= 0 && fsign*ss1 <= 0)//同在外侧
//			{
//				//outpoint.push_back(pp0);
//				outpoint.push_back(tempinpoint[(j + 1) % size]);
//			}
//			else
//			{
//				//计算交点
//				float D = p0.x*(pp1.y() - pp0.y()) + p1.x*(pp0.y() - pp1.y()) + pp0.x()*(p0.y - p1.y) + pp1.x()*(p1.y - p0.y);
//				float D0 = (p1.x - p0.x)*(pp0.y() - pp1.y()) + (p1.y - p0.y)*(pp1.x() - pp0.x());
//				
//				if (D == 0)
//					D = 0;
//				float t0 = (p0.x*(pp1.y() - pp0.y()) + pp0.x()*(p0.y - pp1.y()) +pp0.x()*(pp0.y() - p0.y)) / D;
//
//				float  t1 = ((pp0.x() - p0.x)*(pp0.y() - pp1.y()) + (pp0.y() - p0.y)*(pp1.x() - pp0.x())) / D0;
//				float  t2 = ((p1.x - p0.x)*(pp0.y() - p0.y) - (p1.y - p0.y)*(pp0.x() - p0.x)) / D0;
//				
//				float x = p0.x + t1*(p1.x - p0.x);
//				float y = p0.y + t1*(p1.y - p0.y);
//				float z = pp0.z() + t2*(pp1.z() - pp0.z());
//				/*float z = p0.z + t2*(p1.z - p0.z); */
//				//if (t0>1||t0<0)//交点不在四边形上
//				//	z = pp0.z() + t2*(pp1.z() - pp0.z());
//				if (i == 1)
//				{
//					tlist1.push_back(t1);
//				}
//				else if (i == 3)
//				{
//					tlist3.push_back(t1);
//				}
//
//				float texs = texs0 + (texs1 - texs0)*t2;
//				float text = text0 + (text1 - text0)*t2;
//
//				dvPoint3D interP(x, y, z);
//				_3DPtWithTexture interPt;
//				interPt.pt = interP;
//				interPt.tex.s = texs;
//				interPt.tex.t = text;
//
//				if (fsign*ss0 <= 0 && fsign*ss1 >= 0)//外内
//				{
//					outpoint.push_back(interPt);
//					inpoint.push_back(interPt);
//					inpoint.push_back(tempinpoint[(j + 1) % size]);
//				}
//				else if (fsign*ss0 >= 0 && fsign*ss1 <= 0)//内外
//				{
//					outpoint.push_back(interPt);
//					outpoint.push_back(tempinpoint[(j + 1) % size]);
//					inpoint.push_back(interPt);
//				}
//				else
//					D = D;
//			}
//			
//			int oo = outpoint.size();
//			if (oo > 0 && (oo < 3 || oo>4))
//				oo = oo;
//			////去掉相同点
//			//bool bFind = true;
//			//while (bFind&&oo > 0)
//			//{
//			//	oo = outpoint.size();
//			//	bFind = false;
//			//	for (int si = 0; si < oo; si++)
//			//	{
//			//		dvPoint3D tp = outpoint[si].pt;
//			//		for (int sj = si + 1; sj < oo; sj++)
//			//		{
//			//			if (tp == outpoint[sj].pt)
//			//			{
//			//				bFind = true;
//			//				outpoint.erase(outpoint.begin() + sj);
//			//				break;
//			//			}
//			//		}
//
//			//		if (bFind == true) break;
//			//	}
//			//}
//
//			//oo = inpoint.size();
//			//bFind = true;
//			//while (bFind&&oo > 0)
//			//{
//			//	oo = inpoint.size();
//			//	bFind = false;
//			//	for (int si = 0; si < oo; si++)
//			//	{
//			//		dvPoint3D tp = inpoint[si].pt;
//			//		for (int sj = si + 1; sj < oo; sj++)
//			//		{
//			//			if (tp == inpoint[sj].pt)
//			//			{
//			//				bFind = true;
//			//				inpoint.erase(inpoint.begin() + sj);
//			//				break;
//			//			}
//			//		}
//
//			//		if (bFind == true) break;
//			//	}
//			//}
//		}
//		
//		tempinpoint.clear();
//		for (int j = 0; j < inpoint.size(); j++)
//		{
//			tempinpoint.push_back(inpoint[j]);
//		}
//		inpoint.clear();
//		
//		if (outpoint.size() == 3)
//		{
//			dvTerrainTriangle tri;
//			for (int dd = 0; dd < 3; dd++)
//			{
//				tri.m_point[dd] = outpoint[dd].pt;
//				tri.m_texCoor[dd][0] = outpoint[dd].tex[0];
//				tri.m_texCoor[dd][1] = outpoint[dd].tex[1];
//			}
//
//			result.push_back(tri);
//		}
//		else if(outpoint.size() == 4)
//		{
//			dvTerrainTriangle tri;
//			for (int dd = 0; dd < 3; dd++)
//			{
//				tri.m_point[dd] = outpoint[dd].pt;
//				tri.m_texCoor[dd][0] = outpoint[dd].tex[0];
//				tri.m_texCoor[dd][1] = outpoint[dd].tex[1];
//			}
//				
//			result.push_back(tri);
//
//			tri.m_point[0] = outpoint[2].pt;
//			tri.m_texCoor[0][0] = outpoint[2].tex[0];
//			tri.m_texCoor[0][1] = outpoint[2].tex[1];
//
//			tri.m_point[1] = outpoint[3].pt;
//			tri.m_texCoor[1][0] = outpoint[3].tex[0];
//			tri.m_texCoor[1][1] = outpoint[3].tex[1];
//
//			tri.m_point[2] = outpoint[0].pt;
//			tri.m_texCoor[2][0] = outpoint[0].tex[0];
//			tri.m_texCoor[2][1] = outpoint[0].tex[1];
//
//			result.push_back(tri);
//		}
//		else if (outpoint.size() == 5)
//		{
//			dvTerrainTriangle tri;
//			for (int dd = 0; dd<3; dd++)
//			{
//				tri.m_point[dd] = outpoint[dd].pt;
//				tri.m_texCoor[dd][0] = outpoint[dd].tex[0];
//				tri.m_texCoor[dd][1] = outpoint[dd].tex[1];
//			}
//			result.push_back(tri);
//
//			tri.m_point[0] = outpoint[2].pt;
//			tri.m_texCoor[0][0] = outpoint[2].tex[0];
//			tri.m_texCoor[0][1] = outpoint[2].tex[1];
//			tri.m_point[1] = outpoint[3].pt;
//			tri.m_texCoor[1][0] = outpoint[3].tex[0];
//			tri.m_texCoor[1][1] = outpoint[3].tex[1];
//			tri.m_point[2] = outpoint[0].pt;
//			tri.m_texCoor[2][0] = outpoint[0].tex[0];
//			tri.m_texCoor[2][1] = outpoint[0].tex[1];
//			result.push_back(tri);
//
//			tri.m_point[0] = outpoint[4].pt;
//			tri.m_texCoor[0][0] = outpoint[4].tex[0];
//			tri.m_texCoor[0][1] = outpoint[4].tex[1];
//			tri.m_point[1] = outpoint[3].pt;
//			tri.m_texCoor[1][0] = outpoint[3].tex[0];
//			tri.m_texCoor[1][1] = outpoint[3].tex[1];
//			tri.m_point[2] = outpoint[0].pt;
//			tri.m_texCoor[2][0] = outpoint[0].tex[0];
//			tri.m_texCoor[2][1] = outpoint[0].tex[1];
//
//			result.push_back(tri);
//		}
//		else if (outpoint.size() == 6)
//		{
//			dvTerrainTriangle tri;
//			for (int dd = 0; dd<3; dd++)
//			{
//				tri.m_point[dd] = outpoint[dd].pt;
//				tri.m_texCoor[dd][0] = outpoint[dd].tex[0];
//				tri.m_texCoor[dd][1] = outpoint[dd].tex[1];
//			}
//			result.push_back(tri);
//
//			for (int dd = 2; dd<5; dd++)
//			{
//				tri.m_point[dd-2] = outpoint[dd].pt;
//				tri.m_texCoor[dd-2][0] = outpoint[dd].tex[0];
//				tri.m_texCoor[dd-2][1] = outpoint[dd].tex[1];
//			}
//			result.push_back(tri);
//			
//			for (int dd = 4; dd<=6; dd++)
//			{
//				tri.m_point[dd - 4] = outpoint[dd%6].pt;
//				tri.m_texCoor[dd - 4][0] = outpoint[dd%6].tex[0];
//				tri.m_texCoor[dd - 4][1] = outpoint[dd%6].tex[1];
//			}
//			result.push_back(tri);
//
//			tri.m_point[0] = outpoint[0].pt;
//			tri.m_texCoor[0][0] = outpoint[0].tex[0];
//			tri.m_texCoor[0][1] = outpoint[0].tex[1];
//			tri.m_point[1] = outpoint[2].pt;
//			tri.m_texCoor[1][0] = outpoint[2].tex[0];
//			tri.m_texCoor[1][1] = outpoint[2].tex[1];
//			tri.m_point[2] = outpoint[4].pt;
//			tri.m_texCoor[2][0] = outpoint[4].tex[0];
//			tri.m_texCoor[2][1] = outpoint[4].tex[1];
//
//			result.push_back(tri);
//		}
//		else if (outpoint.size() >= 7)
//		{
//			int s = 0;
//			s = 1;
//		}
//		outpoint.clear();
//	}
//	
//	return TRUE;
//}
//
//bool dvTerrainTriangle::ClipQuadWithIn(_QuadLine& quad, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris)
//{
//	if (quad.minXY.x > m_maxx || quad.minXY.y > m_maxy) return false;//不裁剪
//	if (quad.maxXY.x < m_minx || quad.maxXY.y < m_miny) return false;//不裁剪
//
//	//首先判读是否顶点是否全部在外侧
//	int sss[4] = { 0,0,0,0 };
//	int outcount[4] = { 0,0,0,0 };
//	vec2 outindex[4];
//
//	vec3 s1cor[4];//交点1坐标
//	vec3 s2cor[4];//交点2坐标
//	vec2 st1[4];//交点1纹理
//	vec2 st2[4];//交点2纹理
//
//	bool flag[4] = { false,false,false,false };
//	float  eps0 = -0.02f;
//	float eps1 = 1.02f;
//	float  eps = 0.001;
//	for (int i = 0; i < 4; i++)
//	{
//		sss[i] = 0;
//		dvPoint3D p1 = dvPoint3D(quad.pt[i].x, quad.pt[i].y, quad.pt[i].z);// dvPoint3Dpt[i];
//		dvPoint3D p2 = dvPoint3D(quad.pt[(i + 1) % 4].x, quad.pt[(i + 1) % 4].y, quad.pt[(i + 1) % 4].z);//quad.pt[(i + 1) % 4];
//
//		float fsigns[3];
//		float fsignside[3];
//		for (int j = 0; j < 3; j++)
//		{
//			fsigns[j] = quad.a[i] * m_point[j].x() + quad.b[i] * m_point[j].y() + quad.c[i];
//			fsignside[j] = fsigns[j] * quad.fsign[i];
//		}
//
//		/*	if (fsignside[0] < 0 && fsignside[1] < 0 && fsignside[2] < 0) return false;
//		if (fabs(fsigns[0]) < 0.01f &&  abs(fsigns[1]) < 0.01f && fsignside[2] < 0) return false;
//		if (fabs(fsigns[0]) < 0.01f &&  abs(fsigns[2]) < 0.01f && fsignside[1] < 0) return false;
//		if (fabs(fsigns[1]) < 0.01f &&  abs(fsigns[2]) < 0.01f && fsignside[0] < 0) return false;*/
//
//		if (fsignside[0] >= 0 && fsignside[1] >= 0 && fsignside[2] >= 0)
//			flag[i] = true;
//
//		//如果有2个交点情况
//		int test = 0;
//		for (int j = 0; j < 3; j++)
//		{
//			if (fsignside[j] >= 0 && fsignside[(j + 1) % 3] <= 0 && fsignside[(j + 2) % 3] <= 0)
//			{
//				dvPoint3D pt0, pt1;
//				pt0 = p1; pt1 = p2;
//				dvPoint3D pp0 = m_point[j];
//				dvPoint3D pp1 = m_point[(j + 1) % 3];
//
//				float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//				float t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//				/*	if (t1 < eps0 )
//				t1 = 0;
//				if (t1 > eps1)
//				t1 = 1;*/
//
//				float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//				float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//				/*float ix1 = pp0.x() + t1*(pt1.x() - pp0.x());
//				float iy1 = pp0.y() + t1*(pt1.y() - pp0.y());*/
//				float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				//float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//
//				float iz1 = iz11;
//
//				float ts1 = m_texCoor[j][0] + t1*(m_texCoor[(j + 1) % 3][0] - m_texCoor[j][0]);
//				float tt1 = m_texCoor[j][1] + t1*(m_texCoor[(j + 1) % 3][1] - m_texCoor[j][1]);
//
//				pp1 = m_point[(j + 2) % 3];
//				float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				double DD2 = (pt1.x() - pt0.x())*((double)pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*((double)pp1.x() - pp0.x());
//				float s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//				float t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;//t三角形的参数
//				double dt2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / DD2;//t三角形的参数
//				/*	if (t2 < eps0)
//				t2 = 0;
//				if (t2 > eps1)
//				t2 = 1;*/
//
//				float ts2 = m_texCoor[j][0] + t2*(m_texCoor[(j + 2) % 3][0] - m_texCoor[j][0]);
//				float tt2 = m_texCoor[j][1] + t2*(m_texCoor[(j + 2) % 3][1] - m_texCoor[j][1]);
//
//				float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//				float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//				/*	float ix2 = pp0.x() + t2*(pp1.x() - pp0.x());
//				float iy2 = pp0.y() + t2*(pp1.y() - pp0.y());*/
//				//	float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//				float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz2 = iz21;
//
//				s1cor[i] = vec3(ix1, iy1, iz1);
//				st1[i] = vec2(ts1, tt1);
//				s2cor[i] = vec3(ix2, iy2, iz2);
//				st2[i] = vec2(ts2, tt2);
//
//				if (fabs(s1) <= eps) s1 = 0;
//				if (fabs(s2) <= eps) s2 = 0;
//				if (fabs(s1 - 1) <= eps) s1 = 1;
//				if (fabs(s2 - 1) <= eps) s2 = 1;
//
//				if ((s1 <= 0 && s2 <= 0) || (s1 >= 1 && s2 >= 1))
//					sss[i] = 2;
//				else
//					sss[i] = 1;
//
//
//				outcount[i] = 2;
//				outindex[i] = vec2((j + 1) % 3, (j + 2) % 3);
//				break;
//			}
//			if (fsignside[j] <= 0 && fsignside[(j + 1) % 3] >= 0 && fsignside[(j + 2) % 3] >= 0)
//			{
//				dvPoint3D pt0, pt1;
//				pt0 = p1; pt1 = p2;
//				dvPoint3D pp0 = m_point[j];
//				dvPoint3D pp1 = m_point[(j + 1) % 3];
//
//				float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//				float t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//				/*	if (t1 < eps0)
//				t1 = 0;
//				if (t1 > eps1)
//				t1 = 1;*/
//
//				float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//				float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//				//float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//				//float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//
//				/*float ix1 = pp0.x() + t1*(pp1.x() - pp0.x());
//				float iy1 = pp0.y() + t1*(pp1.y() - pp0.y());*/
//				float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz1 = iz11;
//				float ts1 = m_texCoor[j][0] + t1*(m_texCoor[(j + 1) % 3][0] - m_texCoor[j][0]);
//				float tt1 = m_texCoor[j][1] + t1*(m_texCoor[(j + 1) % 3][1] - m_texCoor[j][1]);
//
//				pp1 = m_point[(j + 2) % 3];
//				float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//				float t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;//t三角形的参数
//				/*	if (t2 < eps0)
//				t2 = 0;
//				if (t2 > eps1)
//				t2 = 1;*/
//
//				float ts2 = m_texCoor[j][0] + t2*(m_texCoor[(j + 2) % 3][0] - m_texCoor[j][0]);
//				float tt2 = m_texCoor[j][1] + t2*(m_texCoor[(j + 2) % 3][1] - m_texCoor[j][1]);
//
//				float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//				float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//				/*float ix2 = pp0.x() + t2*(pp1.x() - pp0.x());
//				float iy2 = pp0.y() + t2*(pp1.y() - pp0.y());*/
//				float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//				float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz2 = iz21;
//
//				s1cor[i] = vec3(ix1, iy1, iz1);
//				st1[i] = vec2(ts1, tt1);
//				s2cor[i] = vec3(ix2, iy2, iz2);
//				st2[i] = vec2(ts2, tt2);
//	
//				if (fabs(s1) <= eps) s1 = 0;
//				if (fabs(s2) <= eps) s2 = 0;
//				if (fabs(s1-1) <= eps) s1 = 1;
//				if (fabs(s2-1) <= eps0) s2 = 1;
//
//				if ((s1 <= 0 && s2 <= 0) || (s1 >= 1 && s2 >= 1))
//					sss[i] = 2;
//				else 
//					sss[i] = 1;
//
//				outcount[i] = 1;
//				outindex[i] = vec2(j, 0);
//
//				break;
//			}
//		}
//	}
//
//	if (flag[0] && flag[1] && flag[2] && flag[3])
//		return true;
//
//	bool bFind = false;
//	for (int i = 0; i < 4; i++)
//	{
//		if (sss[i] == 1)
//		{
//			bFind = true;
//			break;
//		}
//	}
//	if (!bFind) return false;
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (outcount[i] == 1)
//		{
//			int outindexx = (int)(outindex[i].x);
//
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[outindexx];
//			pts[1] = dvPoint3D(s1cor[i].x, s1cor[i].y, s1cor[i].z);
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//
//			tri.m_texCoor[0][0] = m_texCoor[outindexx][0];	tri.m_texCoor[0][1] = m_texCoor[outindexx][1];
//			tri.m_texCoor[1][0] = st1[i].s;					tri.m_texCoor[1][1] = st1[i].t;
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//
//			continue;
//		}
//		if (outcount[i] == 2)
//		{
//			int outindex1 = (int)(outindex[i].x);
//			int outindex2 = (int)(outindex[i].y);
//
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[outindex1];
//			pts[1] = dvPoint3D(s1cor[i].x, s1cor[i].y, s1cor[i].z);
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//
//			tri.m_texCoor[0][0] = m_texCoor[outindex1][0];	tri.m_texCoor[0][1] = m_texCoor[outindex1][1];
//			tri.m_texCoor[1][0] = st1[i].s;					tri.m_texCoor[1][1] = st1[i].t;
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			//外侧
//			pts[0] = m_point[outindex1];
//			pts[1] = m_point[outindex2];
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//
//			tri.m_texCoor[0][0] = m_texCoor[outindex1][0];	tri.m_texCoor[0][1] = m_texCoor[outindex1][1];
//			tri.m_texCoor[1][0] = m_texCoor[outindex2][0];	tri.m_texCoor[1][1] = m_texCoor[outindex2][1];
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			continue;
//		}
//	}
//
//	return true;
//}
//
//bool dvTerrainTriangle::ClipQuad(_QuadLine& quad, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris)
//{
//	if (quad.minXY.x > m_maxx || quad.minXY.y > m_maxy) return false;//不裁剪
//	if (quad.maxXY.x < m_minx || quad.maxXY.y < m_miny) return false;//不裁剪
//	
//	return true;
//	//首先判读是否顶点是否全部在外侧
//	int sss[4] = { 0,0,0,0 };
//	int outcount[4] = { 0,0,0,0 };
//	vec2 outindex[4];
//
//	vec3 s1cor[4];//交点1坐标
//	vec3 s2cor[4];//交点2坐标
//	vec2 st1[4];//交点1纹理
//	vec2 st2[4];//交点2纹理
//
//	vec2 interPtS[4];//交点参数
//
//	bool flag[4] = { false,false,false,false };
//	float eps0 = -0.02f;
//	float eps1 = 1.02f;
//	float eps = 0.01f;
//	float fsigns[4][3];
//	float fsignside[4][3];
//	float dis[4][3];
//
//	for (int i = 0; i < 4; i++)
//	{
//		sss[i] = 0;
//		dvPoint3D p1 = dvPoint3D(quad.pt[i].x, quad.pt[i].y, quad.pt[i].z);// dvPoint3Dpt[i];
//		dvPoint3D p2 = dvPoint3D(quad.pt[(i + 1) % 4].x, quad.pt[(i + 1) % 4].y, quad.pt[(i + 1) % 4].z);//quad.pt[(i + 1) % 4];
//			
//		for (int j = 0; j < 3; j++)
//		{
//			fsigns[i][j] = quad.a[i] *m_point[j].x() + quad.b[i] *m_point[j].y() + quad.c[i];
//			fsignside[i][j] = fsigns[i][j] * quad.fsign[i];
//			dis[i][j] = fabs(fsigns[i][j]) / (sqrt(quad.a[i] * quad.a[i] + quad.b[i] * quad.b[i]));
//		}
//
//	/*	if (fsignside[0] < 0 && fsignside[1] < 0 && fsignside[2] < 0) return false;
//		if (fabs(fsigns[0]) < 0.01f &&  abs(fsigns[1]) < 0.01f && fsignside[2] < 0) return false;
//		if (fabs(fsigns[0]) < 0.01f &&  abs(fsigns[2]) < 0.01f && fsignside[1] < 0) return false;
//		if (fabs(fsigns[1]) < 0.01f &&  abs(fsigns[2]) < 0.01f && fsignside[0] < 0) return false;*/
//
//		if (fsignside[i][0] >= 0 && fsignside[i][1] >= 0 && fsignside[i][2] >= 0)
//			flag[i] = true;
//
//		//如果有2个交点情况
//		int test = 0;
//		for (int j = 0; j < 3; j++)
//		{
//			if (fsignside[i][j] >= 0 && fsignside[i][(j + 1) % 3] <= 0 && fsignside[i][(j + 2) % 3] <= 0)
//			{
//				dvPoint3D pt0, pt1;
//				pt0 = p1; pt1 = p2;
//				dvPoint3D pp0 = m_point[j];
//				dvPoint3D pp1 = m_point[(j + 1) % 3];
//
//				float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//				float t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//			/*	if (t1 < eps0 ) 
//					t1 = 0;
//				if (t1 > eps1)
//					t1 = 1;*/
//
//				float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//				float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//				/*float ix1 = pp0.x() + t1*(pt1.x() - pp0.x());
//				float iy1 = pp0.y() + t1*(pt1.y() - pp0.y());*/
//				float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				//float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//				
//				float iz1 = iz11;
//				
//				float ts1 = m_texCoor[j][0] + t1*(m_texCoor[(j + 1) % 3][0] - m_texCoor[j][0]);
//				float tt1 = m_texCoor[j][1] + t1*(m_texCoor[(j + 1) % 3][1] - m_texCoor[j][1]);
//
//				pp1 = m_point[(j + 2) % 3];
//				float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				double DD2 = (pt1.x() - pt0.x())*((double)pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*((double)pp1.x() - pp0.x());
//				float s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//				float t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;//t三角形的参数
//				double dt2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / DD2;//t三角形的参数
//			/*	if (t2 < eps0)
//					t2 = 0;
//				if (t2 > eps1)
//					t2 = 1;*/
//
//				float ts2 = m_texCoor[j][0] + t2*(m_texCoor[(j + 2) % 3][0] - m_texCoor[j][0]);
//				float tt2 = m_texCoor[j][1] + t2*(m_texCoor[(j + 2) % 3][1] - m_texCoor[j][1]);
//				
//				float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//				float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//			/*	float ix2 = pp0.x() + t2*(pp1.x() - pp0.x());
//				float iy2 = pp0.y() + t2*(pp1.y() - pp0.y());*/
//			//	float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//				float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz2 = iz21;
//
//				s1cor[i] = vec3(ix1, iy1, iz1);
//				st1[i] = vec2(ts1, tt1);
//				s2cor[i] = vec3(ix2, iy2, iz2);
//				st2[i] = vec2(ts2, tt2);
//
//				interPtS[i].x = s1;
//				interPtS[i].y = s2;
//
//				if (fabs(s1) <= eps) s1 = 0;
//				if (fabs(s2) <= eps) s2 = 0;
//				if (fabs(s1 - 1) <= eps) s1 = 1;
//				if (fabs(s2 - 1) <= eps) s2 = 1;
//
//				if ((s1 <= 0 && s2 <= 0) || (s1 >= 1 && s2 >= 1))
//				{
//					if ((s1 == 0 && s2 == 0) || (s1 == 1 && s2 == 1))
//					{
//						sss[i] = 0;
//						outcount[i] = 0;
//					}
//					else
//					{
//						sss[i] = 2;
//						outcount[i] = 2;
//						outindex[i] = vec2((j + 1) % 3, (j + 2) % 3);
//					}
//				}
//				else
//				{
//					sss[i] = 1;
//					outcount[i] = 2;
//					outindex[i] = vec2((j + 1) % 3, (j + 2) % 3);
//				}
//				break;
//			}
//			if (fsignside[i][j] <= 0 && fsignside[i][(j + 1) % 3] >= 0 && fsignside[i][(j + 2) % 3] >= 0)
//			{
//				dvPoint3D pt0, pt1;
//				pt0 = p1; pt1 = p2;
//				dvPoint3D pp0 = m_point[j];
//				dvPoint3D pp1 = m_point[(j + 1) % 3];
//
//				float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//				float t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//			/*	if (t1 < eps0)
//					t1 = 0;
//				if (t1 > eps1)
//					t1 = 1;*/
//
//				float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//				float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//				//float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//				//float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				
//				/*float ix1 = pp0.x() + t1*(pp1.x() - pp0.x());
//				float iy1 = pp0.y() + t1*(pp1.y() - pp0.y());*/
//				float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz1 = iz11;
//				float ts1 = m_texCoor[j][0] + t1*(m_texCoor[(j + 1) % 3][0] - m_texCoor[j][0]);
//				float tt1 = m_texCoor[j][1] + t1*(m_texCoor[(j + 1) % 3][1] - m_texCoor[j][1]);
//
//				pp1 = m_point[(j + 2) % 3];
//				float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//				float s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//				float t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;//t三角形的参数
//			/*	if (t2 < eps0)
//					t2 = 0;
//				if (t2 > eps1)
//					t2 = 1;*/
//
//				float ts2 = m_texCoor[j][0] + t2*(m_texCoor[(j + 2) % 3][0] - m_texCoor[j][0]);
//				float tt2 = m_texCoor[j][1] + t2*(m_texCoor[(j + 2) % 3][1] - m_texCoor[j][1]);
//
//				float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//				float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//				/*float ix2 = pp0.x() + t2*(pp1.x() - pp0.x());
//				float iy2 = pp0.y() + t2*(pp1.y() - pp0.y());*/
//				float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//				float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//				float iz2 = iz21;
//
//				s1cor[i] = vec3(ix1, iy1, iz1);
//				st1[i] = vec2(ts1, tt1);
//				s2cor[i] = vec3(ix2, iy2, iz2);
//				st2[i] = vec2(ts2, tt2);
//
//				interPtS[i].x = s1;
//				interPtS[i].y = s2;
//
//				if (fabs(s1) <= eps) s1 = 0;
//				if (fabs(s2) <= eps) s2 = 0;
//				if (fabs(s1 - 1) <= eps) s1 = 1;
//				if (fabs(s2 - 1) <= eps) s2 = 1;
//
//				if ((s1 <= 0 && s2 <= 0) || (s1 >= 1 && s2 >= 1))
//				{
//					if ((s1 == 0 && s2 == 0) || (s1 == 1 && s2 == 1))
//					{
//						sss[i] = 0; 
//						outcount[i] = 0;
//					}
//					else
//					{
//						sss[i] = 2;
//					//	sss[i] = 1;
//
//						outcount[i] = 1;
//						outindex[i] = vec2(j, 0);
//					}
//				}
//				else
//				{
//					sss[i] = 1;
//
//					outcount[i] = 1;
//					outindex[i] = vec2(j, 0);
//				}
//
//				break;
//			}
//		}
//	}
//	int nnout = 0;
//	if (quad.pile == 5830)
//	{
//		for (int i = 0; i < 4; i++)
//			if (outcount[i] == 0)nnout++;
//
//		if (nnout == 4) 
//			return true;//全在内
//	}
//
//	if (flag[0] && flag[1] && flag[2] && flag[3])
//	{
//		if (quad.pile == 5830)
//			eps0 = eps0;
//		return true;
//	}
//
//	bool bFind = false;
//	
//	for (int i = 0; i < 4; i++)
//	{
//		if (sss[i] == 1)
//		{
//			bFind = true;
//			break;
//		}
//		
//	}
//
//	//if (nnout == 4) 
//	//	return true;//全在内
//
//	static int count = 0;
//
//	if (!bFind)
//	{
//		/*if (quad.pile == 5830)
//		{
//			bFind = bFind;
//			count++;
//
//			if (count == 4) 
//				return true;
//		}*/		
//		
//		return false;
//	}
//
//	
//	if (quad.pile == 5830)
//	{
//		bFind = bFind;
//		//return false;
//
//		//if (dis[0][0] > 42 && dis[0][1] < 0.124 && dis[0][2] < 0.123&& dis[3][1] < 0.1)
//		//	return false;
//	}
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (outcount[i] == 1)
//		{
//			int outindexx = (int)(outindex[i].x);
//					
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[outindexx];
//			pts[1] = dvPoint3D(s1cor[i].x, s1cor[i].y, s1cor[i].z);
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//			
//			tri.m_texCoor[0][0] = m_texCoor[outindexx][0];	tri.m_texCoor[0][1] = m_texCoor[outindexx][1];
//			tri.m_texCoor[1][0] = st1[i].s;					tri.m_texCoor[1][1] = st1[i].t;
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//			tri.SetData(pts);
//			outtris.push_back(tri);
//			continue;
//		}
//		if (outcount[i] == 2)
//		{
//			int outindex1 = (int)(outindex[i].x);
//			int outindex2 = (int)(outindex[i].y);
//			
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[outindex1];
//			pts[1] = dvPoint3D(s1cor[i].x, s1cor[i].y, s1cor[i].z);
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//			
//			tri.m_texCoor[0][0] = m_texCoor[outindex1][0];	tri.m_texCoor[0][1] = m_texCoor[outindex1][1];
//			tri.m_texCoor[1][0] = st1[i].s;					tri.m_texCoor[1][1] = st1[i].t;
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//			
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			//外侧
//			pts[0] = m_point[outindex1];
//			pts[1] = m_point[outindex2];
//			pts[2] = dvPoint3D(s2cor[i].x, s2cor[i].y, s2cor[i].z);
//
//			tri.m_texCoor[0][0] = m_texCoor[outindex1][0];	tri.m_texCoor[0][1] = m_texCoor[outindex1][1];
//			tri.m_texCoor[1][0] = m_texCoor[outindex2][0];	tri.m_texCoor[1][1] = m_texCoor[outindex2][1];
//			tri.m_texCoor[2][0] = st2[i].s;                 tri.m_texCoor[2][1] = st2[i].t;
//
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			continue;
//		}
//	}
//
//	return true;
//	////判断裁剪多边形是不是落在三角形内
//	//BOOL intri[4];
//	//for (int i = 0; i < 4; i++)
//	//{
//	//	vec3 pp0 = quad.pt[i];
//	//	intri[i] = IsInTriangle(dvPoint2D(pp0.x, pp0.y));
//	//}
//
//	//if (intri[0] && intri[1] && intri[2] && intri[3])//都在内
//	//{
//	//	static int countt = 0;
//	//	countt++;
//	//	//return true;
//	//}
//	//int sd = 0;
//	//if (m_minx >= quad.minXY.x && m_maxx <= quad.maxXY.x &&m_miny >= quad.minXY.y && m_maxy <= quad.maxXY.y)
//	//{
//	//	sd = 1;
//	//	
//	//	return true;//完全落在窗口内删除
//	//}
//	vector<dvTerrainTriangle> outtristemp;
//	vector<dvTerrainTriangle> intristemp;
//
//	vector<dvTerrainTriangle> source;
//	source.push_back(*this);
//
//	for (int i = 0; i < 4; i++)
//	{
//		dvPoint3D p1 = dvPoint3D(quad.pt[i].x, quad.pt[i].y, quad.pt[i].z);// dvPoint3Dpt[i];
//		dvPoint3D p2 = dvPoint3D(quad.pt[(i + 1) % 4].x, quad.pt[(i + 1) % 4].y, quad.pt[(i + 1) % 4].z);//quad.pt[(i + 1) % 4];
//		float coef[6];
//		coef[0] = quad.a[i];
//		coef[1] = quad.b[i];
//		coef[2] = quad.c[i];
//
//		coef[3] = quad.fsign[i];
//
//		coef[4] = quad.height[i];
//		coef[5] = quad.height[(i+1)%4];
//
//		intristemp.clear();
//		for (int j = 0; j<source.size(); j++)
//			source[j].ClipByLine(p1, p2, coef, outtristemp, intristemp);
//
//		for (int k = 0; k < outtristemp.size(); k++)
//			outtris.push_back(outtristemp[k]);
//		outtristemp.clear();
//
//		source.clear();
//		for (int k = 0; k < intristemp.size(); k++)
//			source.push_back(intristemp[k]);
//	}
//	for (int i = 0; i < source.size(); i++)
//		intris.push_back(source[i]);
//
//	////判断是不是有外面的
//	//for (int i = 0; i < intris.size(); i++)
//	//{
//	//	dvTerrainTriangle tri = intris[i];
//
//	//	for (int k = 0; k < 4; k++)
//	//	{
//	//		float fsigns[3];
//	//		float fsignside[3];
//	//		for (int j = 0; j < 3; j++)
//	//		{
//	//			fsigns[j] = quad.a[k] * tri.m_point[j].x() + quad.b[k] * tri.m_point[j].y() + quad.c[k];
//	//			fsignside[j] = fsigns[j] * quad.fsign[k];
//	//		}
//	//		int cc0 = 0;
//	//		int cc1 = 0;
//	//		int cc2 = 0;
//	//		for (int j = 0; j < 3; j++)
//	//		{
//	//			if (fabs(fsigns[j]) < 0.001) cc0++;
//	//			if (fsignside[j] < 0 && fabs(fsigns[j]) > 0.001) cc1++;
//	//			if (fsignside[j] < 0 || fabs(fsigns[j]) < 0.001) cc2++;//外或者裁剪边上
//	//		}
//	//		if (cc0 == 3)
//	//		{
//	//			break;; //全边上
//	//		}
//	//		if (cc1 == 3)
//	//		{
//	//			outtris.push_back(tri);//全在外侧
//	//			break;
//	//		}
//	//		if (cc2 == 3)
//	//		{
//	//			outtris.push_back(tri);//全在外侧
//	//			break;
//	//		}
//	//		//if (fsignside[0] < 0 && fsignside[1] < 0 && fsignside[2] < 0) return false;
//	//		//	if (fabs(fsignside[0]) < 0.01f &&  abs(fsignside[1]) < 0.01f && fsignside[2] < 0) return false;
//	//		//	if (fabs(fsignside[0]) < 0.01f &&  abs(fsignside[2]) < 0.01f && fsignside[1] < 0) return false;
//	//		//	if (fabs(fsignside[1]) < 0.01f &&  abs(fsignside[2]) < 0.01f && fsignside[0] < 0) return false;
//	//	}
//	//}
//
//	return true;
//}
//
//BOOL dvTerrainTriangle::ClipByLine(dvPoint3D p1, dvPoint3D p2, float *coef, vector<dvTerrainTriangle>&outtris, vector<dvTerrainTriangle>&intris)
//{
//	float a = coef[0];
//	float b = coef[1];
//	float c = coef[2];
//	float fsign = coef[3];
//
//	float fsigns[3];
//	float fsignside[3];
//	float eps1 = -0.001;
//	float eps2 = 1.001;
//
//	for (int i = 0; i < 3; i++)
//	{
//		fsigns[i] = a*m_point[i].x() + b*m_point[i].y() + c;
//		fsignside[i] = fsigns[i] * fsign;
//	}
//
//	if (fsignside[0] <= 0 && fsignside[1] <= 0 && fsignside[2] <= 0)
//	{
//		outtris.push_back(*this);
//		return true;
//	}
//
//	if (fsignside[0] >= 0 && fsignside[1] >= 0 && fsignside[2] >= 0)
//	{
//		intris.push_back(*this);
//		return true;
//	}
//
//	float eps = 0.007f;
//	//是否有顶点在裁剪边上
//	int count = 0;
//	for (int i = 0; i < 3; i++)
//		if (fabs(fsigns[i]) <= eps)
//			count++;
//
//	if (count == 3)
//	{
//		intris.push_back(*this);
//		return true;
//	}
//	else if (count == 2)
//	{
//		int index;
//		for (int i = 0; i < 3; i++)
//		{
//			if (fabs(fsigns[i]) > eps)
//			{
//				index = i;
//				break;
//			}
//		}
//
//		if (fsignside[index] > 0)
//			intris.push_back(*this);
//		else
//			outtris.push_back(*this);
//
//		return true;
//	}
//	else if (count == 1)
//	{
//		int index;
//		for (int i = 0; i < 3; i++)
//		{
//			if (fabs(fsigns[i]) <= eps)
//			{
//				index = i;
//				break;
//			}
//		}
//
//		if (fsignside[(index + 1) % 3] < 0 && fsignside[(index + 2) % 3] < 0)//都在外侧
//		{
//			outtris.push_back(*this);
//			return true;
//		}
//		if (fsignside[(index + 1) % 3] > 0 && fsignside[(index + 2) % 3] > 0)//都在内侧
//		{
//			intris.push_back(*this);
//			return true;
//		}
//
//		dvPoint3D pt0, pt1;
//		pt0 = p1; pt1 = p2;
//		dvPoint3D pp0 = m_point[(index + 1) % 3];
//		dvPoint3D pp1 = m_point[(index + 2) % 3];
//		
//		float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//
//		if (D1 == 0)
//			D1 = D1;
//		float  s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//		float  t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//
//		float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//		float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//		float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//		float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//		float ixx1 = pp0.x() + t1*(pp1.x() - pp0.x());//被裁剪三角形上的X
//		float iyy1 = pp0.y() + t1*(pp1.y() - pp0.y());//被裁剪三角形上的Y
//		float iz1 = iz11;
//		//if (s1 >= eps1 || s1 <= eps2) iz1 = iz10;
//
//		float ts = m_texCoor[(index + 1) % 3][0] + t1*(m_texCoor[(index + 2) % 3][0] - m_texCoor[(index + 1) % 3][0]);
//		float tt = m_texCoor[(index + 1) % 3][1] + t1*(m_texCoor[(index + 2) % 3][1] - m_texCoor[(index + 1) % 3][1]);
//
//		if (fsignside[(index + 1) % 3] > 0 && fsignside[(index + 2) % 3] < 0)//内外侧
//		{
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[index];
//			pts[1] = m_point[(index + 2) % 3];;
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.SetData(pts);
//			tri.m_texCoor[0][0] = m_texCoor[index][0];			 tri.m_texCoor[0][1] = m_texCoor[index][1];
//			tri.m_texCoor[1][0] = m_texCoor[(index + 2) % 3][0]; tri.m_texCoor[1][1] = m_texCoor[(index + 2) % 3][1];
//			tri.m_texCoor[2][0] = ts;                            tri.m_texCoor[2][1] = tt;
//			outtris.push_back(tri);
//
//			//内侧
//			pts[0] = m_point[index];
//			pts[1] = m_point[(index + 1) % 3];;
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.m_texCoor[0][0] = m_texCoor[index][0];			 tri.m_texCoor[0][1] = m_texCoor[index][1];
//			tri.m_texCoor[1][0] = m_texCoor[(index + 1) % 3][0]; tri.m_texCoor[1][1] = m_texCoor[(index + 1) % 3][1];
//			tri.m_texCoor[2][0] = ts;                            tri.m_texCoor[2][1] = tt;
//			tri.SetData(pts);
//			intris.push_back(tri);
//
//			return true;
//		}
//
//		if (fsignside[(index + 1) % 3] < 0 && fsignside[(index + 2) % 3] > 0)//外内侧
//		{
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[index];
//			pts[1] = m_point[(index + 1) % 3];;
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.m_texCoor[0][0] = m_texCoor[index][0];			 tri.m_texCoor[0][1] = m_texCoor[index][1];
//			tri.m_texCoor[1][0] = m_texCoor[(index + 1) % 3][0]; tri.m_texCoor[1][1] = m_texCoor[(index + 1) % 3][1];
//			tri.m_texCoor[2][0] = ts;                            tri.m_texCoor[2][1] = tt;
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			//内侧
//			pts[0] = m_point[index];
//			pts[1] = m_point[(index + 2) % 3];;
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.m_texCoor[0][0] = m_texCoor[index][0];			 tri.m_texCoor[0][1] = m_texCoor[index][1];
//			tri.m_texCoor[1][0] = m_texCoor[(index + 2) % 3][0]; tri.m_texCoor[1][1] = m_texCoor[(index + 2) % 3][1];
//			tri.m_texCoor[2][0] = ts;                            tri.m_texCoor[2][1] = tt;
//			tri.SetData(pts);
//			intris.push_back(tri);
//
//			return true;
//		}
//	}
//
//	//p1和p2对应地形点高度--h s t
//	vec3 p1ht = HeightTexture(dvPoint2D(p1.x(), p1.y()));
//	p1ht.x = coef[4];
//	vec3 p2ht = HeightTexture(dvPoint2D(p2.x(), p2.y()));
//	p2ht.x = coef[5];
//	for (int i = 0; i < 3; i++)
//	{
//		//1个点在外
//		if (fsignside[i] < 0 && fsignside[(i + 1) % 3]>0 && fsignside[(i + 2) % 3] > 0)
//		{
//			//计算交点
//			dvPoint3D pt0, pt1;
//			pt0 = p1; pt1 = p2;
//			dvPoint3D pp0 = m_point[i];
//			dvPoint3D pp1 = m_point[(i + 1) % 3];
//
//			float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//
//			float  s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//			float  t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//
//			float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//			float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//			float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//			float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//			float iz1 = iz11;
//			//if (s1 >= eps1 || s1 <= eps2) iz1 = iz10;
//			
//			float ts1 = m_texCoor[i][0] + t1*(m_texCoor[(i + 1) % 3][0] - m_texCoor[i][0]);
//			float tt1 = m_texCoor[i][1] + t1*(m_texCoor[(i + 1) % 3][1] - m_texCoor[i][1]);
//
//			pp1 = m_point[(i + 2) % 3];
//			float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//
//			float  s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//			float  t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;
//
//			float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//			float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//			float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//			float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//			float iz2 = iz21;
//			//if (s2 >= eps1 || s2 <= eps2) iz2 = iz20;
//			
//			float ts2 = m_texCoor[i][0] + t2*(m_texCoor[(i + 2) % 3][0] - m_texCoor[i][0]);
//			float tt2 = m_texCoor[i][1] + t2*(m_texCoor[(i + 2) % 3][1] - m_texCoor[i][1]);
//
//			//计算p1和p2的纹理坐标
//			float dxi12 = ix2 - ix1;//计算交点的x差
//			float dyi12 = iy2 - iy1;//计算交点的y差
//			float ttp1, ttp2;
//			float p1s, p1t, p2s, p2t;
//
//			if (fabs(dxi12) > 0)
//			{
//				ttp1 = (p1.x() - ix1) / dxi12;
//				ttp2 = (p2.x() - ix1) / dxi12;
//			}
//			else
//			{
//				ttp1 = (p1.y() - iy1) / dyi12;
//				ttp2 = (p2.y() - iy1) / dyi12;
//			}
//			p1s = p1ht.y;// ts1 + (ts2 - ts1)*ttp1;
//			p1t = p1ht.z;// tt1 + (tt2 - tt1)*ttp1;
//			p2s = p2ht.y;// ts1 + (ts2 - ts1)*ttp2;
//			p2t = p2ht.z;// tt1 + (tt2 - tt1)*ttp2;
//			p1.z(p1ht.x);
//			p2.z(p2ht.x);
//
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[i];
//			pts[1] = dvPoint3D(ix1, iy1, iz1);
//			pts[2] = dvPoint3D(ix2, iy2, iz2);
//			tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			//内侧
//			pts[0] = m_point[(i + 1) % 3];
//			pts[1] = dvPoint3D(ix2, iy2, iz2);
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			tri.SetData(pts);
//			intris.push_back(tri);
//
//			pts[0] = m_point[(i + 1) % 3];
//			pts[1] = dvPoint3D(ix2, iy2, iz2);;
//			pts[2] = m_point[(i + 2) % 3];
//			tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			tri.SetData(pts);
//			intris.push_back(tri);
//
//			//if (s1 < 0 && s2 >= 0 && s2 <= 1)
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//			//	//外侧
//			//	pts[0] = m_point[i];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p1s;				tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	tri.SetData(pts);
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p1s;				tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts1;				tri.m_texCoor[2][1] = tt1;
//			//	tri.SetData(pts);
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	tri.SetData(pts);
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = m_point[(i + 2) % 3];
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.SetData(pts);
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//
//			//	tri.SetData(pts);
//			//	intris.push_back(tri);
//
//			//	break;
//			//}
//			//if (s1 > 1 && s2 >= 0 && s2 <= 1)
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//			//	pts[0] = m_point[i];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;				tri.m_texCoor[2][1] = tt1;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = m_point[(i + 2) % 3];
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	intris.push_back(tri);
//			//	break;
//			//}
//
//			//if (s2 < 0 && s1 >= 0 && s1 <= 1)
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//			//	//外侧
//			//	pts[0] = m_point[i];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p1s;				tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p1s;				tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts1;				tri.m_texCoor[2][1] = tt1;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = m_point[(i + 2) % 3];
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	intris.push_back(tri);
//
//			//	break;
//			//}
//			//if (s2 > 1 && s1 >= 0 && s1 <= 1)
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//			//	pts[0] = m_point[i];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;				tri.m_texCoor[2][1] = tt1;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = m_point[(i + 2) % 3];
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	intris.push_back(tri);
//			//	break;
//			//}
//
//			//if (s1 <= 1 && s1 >= 0 && s2 >= 0 && s2 <= 1)//交点都在裁剪边上
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = m_point[(i + 2) % 3];
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 2) % 3][1];;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	intris.push_back(tri);
//			//	break;
//			//}
//			//else if ((s1 < 0 || s1>1) && (s2 < 0 || s2>1))//交点都在裁剪边外侧
//			//{
//			//	if ((s1 < 0 && s2 < 0) || (s1 > 1 && s2 > 1))//在同侧
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//外侧三角形
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//		outtris.push_back(tri);
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = m_point[(i + 2) % 3];
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 2) % 3][1];
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//		intris.push_back(tri);
//
//			//		break;
//			//	}
//			//	//异侧
//			//	if ((s1 < 0 && s2 > 1))
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//外侧三角形
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts2;				tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p2s;				tri.m_texCoor[2][1] = p2t;
//			//		outtris.push_back(tri);
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = m_point[(i + 1) % 3];
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		break;
//			//	}
//			//	if ((s1 > 1 && s2 < 0))
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//外侧三角形
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p2s;				tri.m_texCoor[2][1] = p2t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts2;				tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = m_point[(i + 1) % 3];
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		break;
//			//	}
//			//}
//			break;
//		}
//
//		//2个点在外
//		else if (fsignside[i] < 0 && fsignside[(i + 1) % 3]<0 && fsignside[(i + 2) % 3] > 0)
//		{
//			//计算交点
//			dvPoint3D pt0, pt1;
//			pt0 = p1; pt1 = p2;
//			dvPoint3D pp0 = m_point[i];
//			dvPoint3D pp1 = m_point[(i + 2) % 3];
//
//			float D1 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//
//			float  s1 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D1;//s裁剪边的参数
//			float  t1 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D1;//t三角形的参数
//
//			float ix1 = pt0.x() + s1*(pt1.x() - pt0.x());
//			float iy1 = pt0.y() + s1*(pt1.y() - pt0.y());
//			float iz10 = pt0.z() + s1*(pt1.z() - pt0.z());//裁剪边上的高度
//			float iz11 = pp0.z() + t1*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//			float iz1 = iz11;
//			//if (s1 >= eps1 || s1 <= eps2) iz1 = iz10;
//			
//			float ts1 = m_texCoor[i][0] + t1*(m_texCoor[(i + 2) % 3][0] - m_texCoor[i][0]);
//			float tt1 = m_texCoor[i][1] + t1*(m_texCoor[(i + 2) % 3][1] - m_texCoor[i][1]);
//
//			pp0 = m_point[(i + 1) % 3];
//			float D2 = (pt1.x() - pt0.x())*(pp0.y() - pp1.y()) + (pt1.y() - pt0.y())*(pp1.x() - pp0.x());
//
//			float  s2 = ((pp0.x() - pt0.x())*(pp0.y() - pp1.y()) + (pp0.y() - pt0.y())*(pp1.x() - pp0.x())) / D2;
//			float  t2 = ((pt1.x() - pt0.x())*(pp0.y() - pt0.y()) - (pt1.y() - pt0.y())*(pp0.x() - pt0.x())) / D2;
//
//			float ix2 = pt0.x() + s2*(pt1.x() - pt0.x());
//			float iy2 = pt0.y() + s2*(pt1.y() - pt0.y());
//			//float iz2 = pp0.z() + t2*(pp1.z() - pp0.z());
//			float iz20 = pt0.z() + s2*(pt1.z() - pt0.z());//裁剪边上的高度
//			float iz21 = pp0.z() + t2*(pp1.z() - pp0.z());//被裁剪三角形上的高度
//			float iz2 = iz21;
//			//if (s2 >= eps1 || s2 <= eps2) iz2 = iz20;
//			
//
//			float ts2 = m_texCoor[(i + 1) % 3][0] + t2*(m_texCoor[(i + 2) % 3][0] - m_texCoor[(i + 1) % 3][0]);
//			float tt2 = m_texCoor[(i + 1) % 3][1] + t2*(m_texCoor[(i + 2) % 3][1] - m_texCoor[(i + 1) % 3][1]);
//
//			//计算p1和p2的纹理坐标
//			float dxi12 = ix2 - ix1;//计算交点的x差
//			float dyi12 = iy2 - iy1;//计算交点的y差
//			float ttp1, ttp2;
//			float p1s, p1t, p2s, p2t;
//
//			if (fabs(dxi12) > 0)
//			{
//				ttp1 = (p1.x() - ix1) / dxi12;
//				ttp2 = (p2.x() - ix1) / dxi12;
//			}
//			else
//			{
//				ttp1 = (p1.y() - iy1) / dyi12;
//				ttp2 = (p2.y() - iy1) / dyi12;
//			}
//			p1s = ts1 + (ts2 - ts1)*ttp1;
//			p1t = tt1 + (tt2 - tt1)*ttp1;
//			p2s = ts1 + (ts2 - ts1)*ttp2;
//			p2t = tt1 + (tt2 - tt1)*ttp2;
//
//			p1s = p1ht.y;// ts1 + (ts2 - ts1)*ttp1;
//			p1t = p1ht.z;// tt1 + (tt2 - tt1)*ttp1;
//			p2s = p2ht.y;// ts1 + (ts2 - ts1)*ttp2;
//			p2t = p2ht.z;// tt1 + (tt2 - tt1)*ttp2;
//			p1.z(p1ht.x);
//			p2.z(p2ht.x);
//
//			dvTerrainTriangle tri;
//			dvPoint3D pts[3];
//			//外侧
//			pts[0] = m_point[(i + 1) % 3];
//			pts[1] = dvPoint3D(ix2, iy2, iz2);
//			pts[2] = dvPoint3D(ix1, iy1, iz1);
//			tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			tri.SetData(pts);
//			outtris.push_back(tri);
//
//			pts[0] = m_point[(i + 1) % 3];
//			pts[1] = dvPoint3D(ix1, iy1, iz1);;
//			pts[2] = m_point[i];
//			tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			tri.m_texCoor[2][0] = m_texCoor[i][0];	tri.m_texCoor[2][1] = m_texCoor[i][1];
//			tri.SetData(pts);
//			outtris.push_back(tri);
//			
//			//内侧
//			pts[0] = m_point[(i + 2) % 3];
//			pts[1] = dvPoint3D(ix1, iy1, iz1);
//			pts[2] = dvPoint3D(ix2, iy2, iz2);
//			tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			tri.SetData(pts);
//			intris.push_back(tri);
//
//
//			//if (s1 <= 1 && s1 >= 0 && s2 > 1)//
//			//{
//			//	//外侧
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = p2s;				tri.m_texCoor[2][1] = p2t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = m_point[(i + 1) % 3];
//			//	pts[2] = p2;
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];				tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	tri.SetData(pts);
//
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	intris.push_back(tri);
//			//	break;
//			//}
//
//			//if (s1 <= 1 && s1 >= 0 && s2 <0)//
//			//{
//			//	//外侧
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = m_point[(i + 1) % 3];
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];				tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//	intris.push_back(tri);
//			//	break;
//			//}
//
//			//if (s1 < 0 && s2 >= 0 && s2 <= 1)//
//			//{
//			//	//外侧
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts2;				tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = m_point[i];
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = m_texCoor[i][0];				tri.m_texCoor[2][1] = m_texCoor[i][1];
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p1;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p1;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//	intris.push_back(tri);
//
//			//	break;
//			//}
//			//if (s1 > 1 && s2 >= 0 && s2 < 1)//
//			//{
//			//	//外侧
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 1) % 3];
//			//	pts[1] = m_point[i];
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[1][0] = m_texCoor[i][0];				tri.m_texCoor[1][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);;
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = p2s;				tri.m_texCoor[2][1] = p2t;
//			//	outtris.push_back(tri);
//
//			//	//内侧
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = p2;
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = p2;
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//	intris.push_back(tri);
//
//			//	break;
//			//}
//
//			//if (s1 <= 1 && s1 >= 0 && s2 >= 0 && s2 <= 1)//交点都在裁剪边上
//			//{
//			//	dvTerrainTriangle tri;
//			//	dvPoint3D pts[3];
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//	tri.m_texCoor[2][0] = ts2;				tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[i];
//			//	pts[1] = m_point[(i + 1) % 3];
//			//	pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[i][0];				tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//	tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//	tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//	outtris.push_back(tri);
//
//			//	pts[0] = m_point[(i + 2) % 3];
//			//	pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//	pts[2] = dvPoint3D(ix1, iy1, iz1);
//			//	tri.SetData(pts);
//			//	tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//	tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//	tri.m_texCoor[2][0] = ts1;							tri.m_texCoor[2][1] = tt1;
//			//	intris.push_back(tri);
//
//			//	break;
//			//}
//
//			//if ((s1 < 0 || s1>1) && (s2 < 0 || s2>1))//交点都在裁剪边外侧
//			//{
//			//	if ((s1 < 0 && s2 < 0) || (s1 > 1 && s2 > 1))//在同侧
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//外侧三角形
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = m_point[i];
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = m_texCoor[i][0];				tri.m_texCoor[2][1] = m_texCoor[i][1];
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//		outtris.push_back(tri);
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = dvPoint3D(ix2, iy2, iz2);
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = ts2;							tri.m_texCoor[2][1] = tt2;
//			//		intris.push_back(tri);
//
//			//		break;
//			//	}
//
//			//	if ((s1 < 0 && s2 > 1))//在异侧
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//		intris.push_back(tri);
//
//			//		//外侧三角形
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = m_point[(i + 1) % 3];
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];				tri.m_texCoor[0][1] = m_texCoor[i][1];
//			//		tri.m_texCoor[1][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[1][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		break;
//			//	}
//			//	if ((s1 > 1 && s2 < 0))
//			//	{
//			//		dvTerrainTriangle tri;
//			//		dvPoint3D pts[3];
//
//			//		//内侧三角形
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;							tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		intris.push_back(tri);
//
//			//		pts[0] = m_point[(i + 2) % 3];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 2) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 2) % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;							tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p2s;							tri.m_texCoor[2][1] = p2t;
//			//		intris.push_back(tri);
//
//			//		//外侧三角形
//			//		pts[0] = m_point[(i + 1) % 3];
//			//		pts[1] = dvPoint3D(ix2, iy2, iz2);
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[0][1] = m_texCoor[(i + 1) % 3][1];
//			//		tri.m_texCoor[1][0] = ts2;							tri.m_texCoor[1][1] = tt2;
//			//		tri.m_texCoor[2][0] = p1s;							tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = dvPoint3D(ix1, iy1, iz1);
//			//		pts[2] = p2;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i % 3][1];
//			//		tri.m_texCoor[1][0] = ts1;				tri.m_texCoor[1][1] = tt1;
//			//		tri.m_texCoor[2][0] = p2s;				tri.m_texCoor[2][1] = p2t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = p2;
//			//		pts[2] = p1;
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];	tri.m_texCoor[0][1] = m_texCoor[i % 3][1];
//			//		tri.m_texCoor[1][0] = p2s;				tri.m_texCoor[1][1] = p2t;
//			//		tri.m_texCoor[2][0] = p1s;				tri.m_texCoor[2][1] = p1t;
//			//		outtris.push_back(tri);
//
//			//		pts[0] = m_point[i];
//			//		pts[1] = p1;
//			//		pts[2] = m_point[(i + 1) % 3];
//			//		tri.SetData(pts);
//			//		tri.m_texCoor[0][0] = m_texCoor[i][0];				tri.m_texCoor[0][1] = m_texCoor[i % 3][1];
//			//		tri.m_texCoor[1][0] = p1s;							tri.m_texCoor[1][1] = p1t;
//			//		tri.m_texCoor[2][0] = m_texCoor[(i + 1) % 3][0];	tri.m_texCoor[2][1] = m_texCoor[(i + 1) % 3][1];
//			//		outtris.push_back(tri);
//
//			//		break;
//			//	}
//			//}
//		}
//	}
//
//	//消除相同点的三角形
//	//vector<dvTerrainTriangle>::iterator    iter;
//	//iter = intris.begin();
//	//while (iter!= intris.end())
//	//{
//	//	dvTerrainTriangle tri = *iter;
//
//	//	if (!tri.isValidation())
//	//		iter = intris.erase(iter);
//	//	else
//	//		iter++;
//	//}
//
//	return true;
//}
//
//bool dvTerrainTriangle::isValidation()
//{
//	if (m_point[0] == m_point[1]) return false;
//	if (m_point[0] == m_point[2]) return false;
//	if (m_point[1] == m_point[2]) return false;
//
//	return true;
//}
//
//
//BOOL dvTerrainTriangle::IntersectionWithLineByTerrain(dvPoint3D p1, dvPoint3D p2, vector<_InterPosTex> & PosWithTerrain)
//{
//	float a, b, c;
//	a = p1.y() - p2.y();
//	b = p2.x() - p1.x();
//	c = p1.x()*p2.y() - p2.x()*p1.y();
//
//	for (int i = 0; i < 3; i++)
//	{
//		dvPoint3D pp1 = m_point[i];
//		dvPoint3D pp2 = m_point[(i + 1) % 3];
//		vec2 tex1 = vec2(m_texCoor[i][0], m_texCoor[i][1]);
//		vec2 tex2 = vec2(m_texCoor[(i + 1) % 3][0], m_texCoor[(i + 1) % 3][1]);
//
//		float  fsign1 = pp1.x()*a + pp1.y()*b + c;
//		float  fsign2 = pp2.x()*a + pp2.y()*b + c;
//
//		if (fsign1*fsign2 > 0) continue;//无交点
//
//		float D0 = (p2.x() - p1.x())*(pp1.y() - pp2.y()) + (p2.y() - p1.y())*(pp2.x() - pp1.x());
//
//		if (fabs(D0) < 0.001f) continue;//平行无交点
//
//		float  t1 = ((pp1.x() - p1.x())*(pp1.y() - pp2.y()) + (pp1.y() - p1.y())*(pp2.x() - pp1.x())) / D0;
//		float  t2 = ((p2.x() - p1.x())*(pp1.y() - p1.y()) - (p2.y() - p1.y())*(pp1.x() - p1.x())) / D0;
//
//		if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)//真正交点
//		{
//			float x = p1.x() + t1*(p2.x() - p1.x());
//			float y = p1.y() + t1*(p2.y() - p1.y());
//			float z1 = p1.z() + t1*(p2.z() - p1.z());
//			float z2 = pp1.z() + t2*(pp2.z() - pp1.z());
//
//			float ts = tex1.s + (tex2.s - tex1.s)*t2;
//			float tt = tex1.t + (tex2.t - tex1.t)*t2;
//
//			_InterPosTex ip;
//			ip.t = t1;
//			ip.s = ts;
//			ip.t = tt;
//			ip.p = dvPoint3D(x, y, z2);
//			ip.tt = t1;//路基线上的t
//
//			PosWithTerrain.push_back(ip);
//		}
//	}
//
//	return TRUE;
//}
//
//BOOL dvTerrainTriangle::IntersectionWithLineByRoad(dvPoint3D p1, dvPoint3D p2, vector<_InterPosTex> & PosWithRoad)
//{
//	float a, b, c;
//	a = p1.y() - p2.y();
//	b = p2.x() - p1.x();
//	c = p1.x()*p2.y() - p2.x()*p1.y();
//
//	for (int i = 0; i < 3; i++)
//	{
//		dvPoint3D pp1 = m_point[i];
//		dvPoint3D pp2 = m_point[(i + 1) % 3];
//		vec2 tex1 = vec2(m_texCoor[i][0], m_texCoor[i][1]);
//		vec2 tex2 = vec2(m_texCoor[(i + 1) % 3][0], m_texCoor[(i + 1) % 3][1]);
//
//		float  fsign1 = pp1.x()*a + pp1.y()*b + c;
//		float  fsign2 = pp2.x()*a + pp2.y()*b + c;
//
//		if (fsign1*fsign2 > 0) continue;//无交点
//
//		float D0 = (p2.x() - p1.x())*(pp1.y() - pp2.y()) + (p2.y() - p1.y())*(pp2.x() - pp1.x());
//
//		if (fabs(D0) < 0.001f) continue;//平行无交点
//
//		float  t1 = ((pp1.x() - p1.x())*(pp1.y() - pp2.y()) + (pp1.y() - p1.y())*(pp2.x() - pp1.x())) / D0;
//		float  t2 = ((p2.x() - p1.x())*(pp1.y() - p1.y()) - (p2.y() - p1.y())*(pp1.x() - p1.x())) / D0;
//
//		if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)//真正交点
//		{
//			float x = p1.x() + t1*(p2.x() - p1.x());
//			float y = p1.y() + t1*(p2.y() - p1.y());
//			float z1 = p1.z() + t1*(p2.z() - p1.z());
//			float z2 = pp1.z() + t2*(pp2.z() - pp1.z());
//
//			float ts = tex1.s + (tex2.s - tex1.s)*t2;
//			float tt = tex1.t + (tex2.t - tex1.t)*t2;
//
//			_InterPosTex ip;
//			ip.t = t1;
//			ip.s = ts;
//			ip.t = tt;
//			ip.p = dvPoint3D(x, y, z1);
//
//			PosWithRoad.push_back(ip);
//		}
//	}
//
//	return TRUE;
//}
//
////refPt参考点，side=true，取三角形位于refPt同（内）侧的部分三角形，false为异（外）侧
//bool dvTerrainTriangle::ClipByLine(double a, double b, double c, dvPoint3D refPt, dvPoint3D outp[10], int *outcount, bool  side)//三角形被直线裁剪
//{
//	double bRefPt = a*refPt.x() + b*refPt.y() + c;
//	double bSide[3];
//
//	for (int i = 0; i < 3; i++)
//	{
//		bSide[i] = a*m_point[i].x() + b*m_point[i].y() + c;
//		bSide[i] *= bRefPt;
//	}
//
//	//输出内侧
//	if (side)
//	{
//		if (bSide[0] >= 0 && bSide[1] >= 0 && bSide[2] >= 0)
//		{
//			return true;
//		}
//	}
//	//输出外侧
//	if (!side)
//	{
//		if (bSide[0] <= 0 && bSide[1] <= 0 && bSide[2] <= 0)
//		{
//			return true;
//		}
//	}
//
//	//切割：先计算2个交点；再决定输出
//	for (int i = 0; i < 3; i++)
//	{
//		if (bSide[i] >= 0)//找第1个同侧的
//		{
//			int j1 = (i + 1) % 3;
//			int j2 = (i + 2) % 3;
//
//			if (bSide[j1] >= 0)//
//			{
//				// 计算ij2及j1j2与直线交点
//				dvLineSegment line1(m_point[i], m_point[j2]);
//				dvLineSegment line2(m_point[j1], m_point[j2]);
//				dvPoint3D inter1, inter2;
//				bool re1 = line1.IsCrossLine(a, b, c, &inter1);
//				bool re2 = line2.IsCrossLine(a, b, c, &inter2);
//				//
//				if (side)//输出内侧
//				{
//					outp[0] = m_point[i]; outp[1] = inter1; outp[2] = m_point[j1];
//					outp[3] = m_point[j1]; outp[4] = inter2; outp[5] = inter1;
//
//					*outcount = 6;
//				}
//				else//输出外侧
//				{
//					outp[0] = inter1; outp[1] = inter2; outp[2] = m_point[j2];
//
//					*outcount = 3;
//				}
//			}
//			else if (bSide[j2] >= 0)
//			{
//				// 计算ij1及j2j1与直线交点
//				dvLineSegment line1(m_point[i], m_point[j1]);
//				dvLineSegment line2(m_point[j2], m_point[j1]);
//				dvPoint3D inter1, inter2;
//				bool re1 = line1.IsCrossLine(a, b, c, &inter1);
//				bool re2 = line2.IsCrossLine(a, b, c, &inter2);
//
//				if (side)//输出内侧
//				{
//					outp[0] = m_point[i]; outp[1] = inter1; outp[2] = m_point[j2];
//					outp[3] = m_point[j2]; outp[4] = inter2; outp[5] = inter1;
//
//					*outcount = 6;
//				}
//				else//输出外侧
//				{
//					outp[0] = inter1; outp[1] = inter2; outp[2] = m_point[j1];
//
//					*outcount = 3;
//				}
//			}
//			else //剩余2个在异侧
//			{
//				//计算ij1及ij2与直线交点
//				dvLineSegment line1(m_point[i], m_point[j1]);
//				dvLineSegment line2(m_point[i], m_point[j2]);
//				dvPoint3D inter1, inter2;
//				bool re1 = line1.IsCrossLine(a, b, c, &inter1);
//				bool re2 = line2.IsCrossLine(a, b, c, &inter2);
//
//				if (side)//输出内侧
//				{
//					outp[0] = m_point[i]; outp[1] = inter1; outp[2] = inter2;
//
//					*outcount = 3;
//				}
//				else//输出外侧
//				{
//					outp[0] = inter1; outp[1] = m_point[j1]; outp[2] = inter2;
//					outp[3] = m_point[j1]; outp[4] = m_point[j2]; outp[5] = inter2;
//
//					*outcount = 6;
//				}
//			}
//
//			break;
//		}
//	}
//
//	return false;
//}
//
void rdTerrainTriangle::calOutsign()
{
	QVector2D p[3];
	for (int i=0;i<3; i++)
		p[i] = QVector2D(m_point[i].x(), m_point[i].y());
	
	for (int i = 0; i < 3; i++)
	{
		double a = p[i].y() - p[(i + 1) % 3].y();
		double b = p[(i + 1) % 3].x() - p[i].x();
		double c = p[i].x() * p[(i + 1) % 3].y() - p[(i + 1) % 3].x() *p[i].y();

		m_linecoef[i] = QVector3D(a, b, c);

		double sign = a*p[(i + 2) % 3].x() + b*p[(i + 2) % 3].y() + c;

		m_outsign[i] = -sign;
	}
}
//
//bool dvTerrainTriangle::IsOutWithQuad(vec2*pts)
//{
//	for (int i = 0; i < 3; i++)//是否同在某条边的外侧，如是，则不相交
//	{
//		float s[4];
//		int count = 0;
//		for (int j = 0; j < 4; j++)
//		{
//			s[i] = m_linecoef[i].x*pts[j].x + m_linecoef[i].y*pts[j].y + m_linecoef[i].z;
//
//			if (s[i]*m_outsign[i] < 0) break;
//			count++;
//		}
//
//		if (count == 4) return true;
//	}
//
//	return false;
//}
//
//void dvTerrainTriangle::CalMinMax()
//{
//	//包围盒
//	m_minx = m_point[0].x();
//	m_maxx = m_point[0].x();
//	m_miny = m_point[0].y();
//	m_maxy = m_point[0].y();
//	m_minz = m_point[0].z();
//	m_maxz = m_point[0].z();
//
//	for (int i = 1; i < 3; i++)
//	{
//		if (m_point[i].x() > m_maxx) m_maxx = m_point[i].x();
//		if (m_point[i].y() > m_maxy) m_maxy = m_point[i].y();
//		if (m_point[i].z() > m_maxz) m_maxz = m_point[i].z();
//
//		if (m_point[i].x() < m_minx) m_minx = m_point[i].x();
//		if (m_point[i].y() < m_miny) m_miny = m_point[i].y();
//		if (m_point[i].z() < m_minz) m_minz = m_point[i].z();
//	}
//}