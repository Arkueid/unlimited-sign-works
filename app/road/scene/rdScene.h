#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "scene/rdTextures.h"
#include "scene/rdShaders.h"
#include "rdCamera.h"
#include "rdSky.h"
#include "rdContractSection.h"

#include "../geosolid/geoCylinder.h"
#include "../geosolid/geoCube.h"
#include "../common/common.h"
#include "../bridge/rdBridgeConicalSlope.h"
#include "../road/rdRoadLine.h"
#include "../road/rdSubgrade.h"
#include "../terrain/rdTerrain.h"

#include <core.hpp>

class rdScene : protected QOpenGLFunctions
{
public:
	rdScene();
	~rdScene();

	void release();

    rdSky* m_pSky;
	rdRoadLine  m_rl;
	rdTerrain m_terrain;
	rdSubgrade  m_roadbed;

    rdTextures m_textures;
	void loadTextures();

	rdShaders m_shaders;
	void loadShaders();

	void initGeometry();
	void loadData();
	void computing();

	void addBridge(rdBridge* pBridge);
	void setCameraPosByBridge(rdBridge* pBridge);

    rdCamera m_camera;
	
	void render(const QMatrix4x4& projection, const QMatrix4x4& view);

	geoCylinder m_cylinder;
	geoCube     m_cube;
	QVector3D m_lightColor;
	QVector3D m_lightDirection;

	//test
	rdBridgeConicalSlope m_bcs;
	rdBridgeConicalSlope m_tailbcs;

	Point m_originPoint;
	float m_originYaw;
private:
	rdContractSection m_sections[14];//标段
};
#endif // TERRAIN_H
