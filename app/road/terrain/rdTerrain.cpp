#include "rdTerrain.h"
#include "rdTerrainTriangle.h"

#include <QFile>
#include <QString>
#include <QCoreApplication>
#include <QTextStream>
#include <QDataStream>
#include <math.h>

rdTerrain::rdTerrain()
{
	initializeOpenGLFunctions();

	m_pTextBlock = 0;
	m_textcount = 0;
}

rdTerrain::~rdTerrain()
{
	if (m_pTextBlock) delete[]m_pTextBlock;
}

bool rdTerrain::readSegPile()
{
	QString filename = QCoreApplication::applicationDirPath() + "/../../assets/data/terrain/TerrainSegPile.txt";
	QFile file(filename);
	if (!file.exists()) return false;

	bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ret)
	{
		QTextStream fin(&file);

		int count;
		QString str;

		fin >> count;
		float start, end;
		int index = 0;
		while (!fin.atEnd())
		{
			fin >> start >> end;
			m_segs[index].m_start = start;
			m_segs[index].m_end = end;
			m_segs[index].m_index = index;

			index++;
		}

		m_count = index;
		file.close();

		return true;
	}
	
	return false;
}

bool rdTerrain::readOrthophotoInfor()
{
	QString filename = QCoreApplication::applicationDirPath() + "/../../assets/data/terrain/orthophoto/seg.txt";
	QFile file(filename);
	if (!file.exists()) return false;

	bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ret)
	{
		QTextStream fin(&file);
			
		int index = 0;
		while (!fin.atEnd())
		{
			int no, count, m;
			fin >> no >> count;

			m_segs[index].m_iTexImageCount = count;

			for (int i = 0; i < count; i++)
			{
				fin >> m;
				m_segs[index].m_iTexImageNo[i] = m;
			}

			index++;
		}

		file.close();

		return true;
	}

	return false;
}

bool rdTerrain::readOrthophotoData()
{
	m_pTextBlock = new _TextBlock[54];

	QString filepath = QCoreApplication::applicationDirPath() + "/../../assets/data/terrain/orthophoto/";

	QString indexstr;
	for (int i = 1; i <= 54; i++)
	{
		indexstr = QString::number(i);

		QString fname;
		QString ftxtname;
		if (i < 10)
		{
			fname = "000" + indexstr + ".jpg";
			ftxtname = "000" + indexstr + ".txt";
		}
		else
		{
			fname = "00" + indexstr + ".jpg";
			ftxtname = "00" + indexstr + ".txt";
		}

		m_pTextBlock[i - 1].fname = fname;

		QString filename = filepath + ftxtname;

		QFile file(filename);
		if (!file.exists())
		{
			delete[] m_pTextBlock;
			m_pTextBlock = 0;
			m_textcount = 0;
			return false;
		}

		bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
		if (ret)
		{
			QTextStream fin(&file);
			float x1, y1, x2, y2;
			fin >> x1 >> y1 >> x2 >> y2;
			m_pTextBlock[i - 1].p0 = QVector2D(x1, y1);
			m_pTextBlock[i - 1].p1 = QVector2D(x2, y2);

			file.close();
		}
		else
		{
			delete[] m_pTextBlock;
			m_pTextBlock = 0;
			m_textcount = 0;
			return false;
		}
	}

	m_textcount = 54;

	return true;
}

bool rdTerrain::readTriData()
{
	//return true;
	QString filename = QCoreApplication::applicationDirPath() + "/../../assets/data/terrain/newterrain.dat";

	QString strHead = QString("-terr");
	QFile file(filename);
	if (!file.exists()) return false;
	bool ret = file.open(QIODevice::ReadOnly);
	if (ret)
	{
		QDataStream fin(&file);
		//文件头
		char head[8];
		fin.readRawData(head, 8 * sizeof(char));
		
		QString strhead = QString::fromLocal8Bit(head, 5);
		if (strHead != strhead)
		{
			file.close();
			return false;
		}
		//总的块数
		qint32 allblocks = 0;
		fin.readRawData((char*)&allblocks, sizeof(qint32));

		if (allblocks != m_count)
		{
			file.close();
			return false;
		}

		for (int i = 0; i < allblocks; i++)
		{
			rdTerrainSeg& segterrain = m_segs[i];

			float allfminx, allfmaxx, allfminy, allfmaxy, allfminz, allfmaxz;
			allfminx = allfminy = allfminz = 999999999.0f;
			allfmaxx = allfmaxy = allfmaxz = -allfminx;

			//读子块数
			qint32 subblocknum = 0;
			fin.readRawData((char*)&subblocknum, sizeof(qint32));
			segterrain.m_iTexImageCount = subblocknum;

			int aclocksize = 0;
			int clocksize = 0;
			//读子块数据
			for (int j = 0; j < subblocknum; j++)
			{
				//读纹理号
				qint32 texti = 0;
				fin.readRawData((char*)&texti, sizeof(qint32));
				segterrain.m_iTexImageNo[j] = texti;

				//读三角形数
				qint32 size = 0;// segterrain.m_seg[j].m_segs.size();
				fin.readRawData((char*)&size, sizeof(qint32));
				if (size == 0) continue;
				//读三角形数据
				float* pd = new float[size * 15];

				fin.readRawData((char*)pd, size * 15 * sizeof(float));

				int index = 0;
				for (int l = 0; l < size; l++)//tris.size()
				{
					rdTerrainTriangle* ptri = new rdTerrainTriangle;// tris[l];

					float fminx, fminy, fminz;
					fminx = fminy = fminz = 99999999.0f;
					float fmaxx, fmaxy, fmaxz;
					fmaxx = fmaxy = fmaxz = -99999999.0f;
					for (int sl = 0; sl < 3; sl++)
					{
						ptri->m_point[sl].setX(pd[index++]);
						ptri->m_point[sl].setY(pd[index++]);
						ptri->m_point[sl].setZ(pd[index++]);
						ptri->m_texCoor[sl][0] = pd[index++];
						ptri->m_texCoor[sl][1] = pd[index++];

						if (fmaxx < ptri->m_point[sl].x()) fmaxx = ptri->m_point[sl].x();
						if (fmaxy < ptri->m_point[sl].y()) fmaxy = ptri->m_point[sl].y();
						if (fmaxz < ptri->m_point[sl].z()) fmaxz = ptri->m_point[sl].z();

						if (fminx > ptri->m_point[sl].x()) fminx = ptri->m_point[sl].x();
						if (fminy > ptri->m_point[sl].y()) fminy = ptri->m_point[sl].y();
						if (fminz > ptri->m_point[sl].z()) fminz = ptri->m_point[sl].z();
					}
										
					ptri->m_minx = fminx; ptri->m_maxx = fmaxx;
					ptri->m_miny = fminy; ptri->m_maxy = fmaxy;
					ptri->m_minz = fminz; ptri->m_maxz = fmaxz;

					bool re = ptri->isAnticlockwise();
					if (re) aclocksize++;
					else
					{
						clocksize++;
						//调整顶点顺序--逆时针
					}
                    ptri->calOutsign();
					segterrain.m_subSeg[j].m_segs.push_back(ptri);

					if (fminx < allfminx) allfminx = fminx;
					if (fminy < allfminy) allfminy = fminy;
					if (fminz < allfminz) allfminz = fminz;
					if (fmaxx > allfmaxx) allfmaxx = fmaxx;
					if (fmaxy > allfmaxy) allfmaxy = fmaxy;
					if (fmaxz > allfmaxz) allfmaxz = fmaxz;
				}

				delete[] pd;
			}
			segterrain.m_minx = allfminx; segterrain.m_miny = allfminy;
			segterrain.m_maxx = allfmaxx; segterrain.m_maxy = allfmaxy;
		}
		file.close();

		//设置纹理
		for (int i = 0; i < allblocks; i++)
		{
			rdTerrainSeg& segterrain = m_segs[i];

			for (int j = 0; j < segterrain.m_iTexImageCount; j++)
			{
				//char strname[10];
				//sprintf(strname, "%d", segterrain.m_iTexImageNo[j]);
				//string name(strname);
				QString tname;
				int indext = segterrain.m_iTexImageNo[j];
				tname = QString::number(indext);

				QString fname;
				if (indext < 10)
					fname = "000" + tname + ".jpg";
				else
					fname = "00" + tname + ".jpg";
				segterrain.m_subSeg[j].m_textname = fname;
				//dvTexture* texture = m_textures.GetTexture(strname);// "1");

				//if (texture)
				//{
				//	segterrain.m_CurSeg[j].m_id = texture->GetID();
				//	segterrain.m_OldSeg[j].m_id = texture->GetID();
				//}
				//else
				//{
				//	segterrain.m_CurSeg[j].m_id = -1;
				//	segterrain.m_OldSeg[j].m_id = -1;
				//}
			}
		} 

		return true;
	}
	
	return false;
}

void rdTerrain::loadTerrainTexture()
{
	m_terrainTextureManager.addTexture("0001.jpg");
	m_terrainTextureManager.addTexture("0002.jpg");
	/*m_terrainTextureManager.AddTexture("0003.jpg");
	m_terrainTextureManager.AddTexture("0004.jpg");*/
}

void rdTerrain::createVBO()
{
	for (int i = 0; i < 20; i++)
	{
		rdTerrainSeg& segterrain = m_segs[i];
		segterrain.createVBO();
	}
}

void rdTerrain::render(QOpenGLShaderProgram* program)
{
	//program->setUniformValue("Tex0", 0);
	int vertexLocation = program->attributeLocation("VertexPosition");
	//program->enableAttributeArray(vertexLocation);
	int texcoordLocation = program->attributeLocation("VertexTexCoord");
	//program->enableAttributeArray(texcoordLocation);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int i = 0; i < 20; i++)
	{
		rdTerrainSeg& segterrain = m_segs[i];

		for (int j = 0; j < segterrain.m_iTexImageCount; j++)
		{
			//纹理名
			QString fname = segterrain.m_subSeg[j].m_textname;
			_terraintexture* pText = m_terrainTextureManager.searchTexture(fname);
			if (pText)
			{
				QOpenGLTexture* texture = pText->pTexture;
				texture->bind();
				program->setUniformValue("Tex0", 0);
			}

			int size = segterrain.m_subSeg[j].m_segs.size() * 3;
			
			segterrain.m_subvbo[j].bind();			
			program->enableAttributeArray(vertexLocation);
			program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0,3);
			
			segterrain.m_subtexcorvbo[j].bind();			
			program->enableAttributeArray(texcoordLocation);
			program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0,2 );

			glDrawArrays(GL_TRIANGLES, 0, size);
		}
	}
}