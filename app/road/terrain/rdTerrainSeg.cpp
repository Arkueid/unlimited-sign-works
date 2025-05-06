#include "rdTerrainSeg.h"

rdTerrainSeg::rdTerrainSeg()
{
	initializeOpenGLFunctions();
}

rdTerrainSeg::~rdTerrainSeg()
{
	for (int i = 0; i < m_iTexImageCount; i++)
	{
		_RenderSeg& rs = m_subSeg[i];

		while (rs.m_segs.size())
		{
			rdTerrainTriangle* first = rs.m_segs.first();
			rs.m_segs.pop_front();
			delete first;
		}
	}

	for (int i = 0; i < m_iTexImageCount; i++)
	{
		m_subvbo[i].destroy();
		m_subtexcorvbo[i].destroy();
	}
}

void rdTerrainSeg::createVBO()
{
	for (int i = 0; i < m_iTexImageCount; i++)
	{
		_RenderSeg &rs = m_subSeg[i];

		int size = rs.m_segs.size();

		QVector3D* pVerts = new QVector3D[3 * size];
		int index = 0;
		QVector2D* pTextCors = new QVector2D[3 * size];
		int indexT = 0;

		for (auto it = rs.m_segs.begin(); it != rs.m_segs.end(); ++it)		
		{
			rdTerrainTriangle* pTri = *it;

			for (int j = 0; j < 3; j++)
			{
				pVerts[index++] = pTri->m_point[j];
				pTextCors[indexT++] = QVector2D(pTri->m_texCoor[j][0],
					pTri->m_texCoor[j][1]);
			}
		}
		bool re = m_subvbo[i].create();
		m_subvbo[i].bind();
		m_subvbo[i].allocate(pVerts, 3 * size * sizeof(QVector3D));

		re = m_subtexcorvbo[i].create();
		m_subtexcorvbo[i].bind();
		m_subtexcorvbo[i].allocate(pTextCors, 3 * size * sizeof(QVector2D));

		delete[] pVerts;
		delete[] pTextCors;
	}
}