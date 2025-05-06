#include "rdScene.h"
#include <QMatrix4x4>

rdScene::rdScene()
{
    initializeOpenGLFunctions();
    m_pSky = 0;

    m_lightColor = QVector3D(1, 1, 1);
    m_lightDirection = QVector3D(-1, -1, 1);
    m_lightDirection.normalize();

     m_camera.setPos(QVector3D(0, -4, 0));
    m_camera.setYaw(0);
}

rdScene::~rdScene()
{
    release();
}

void rdScene::release()
{
    if (m_pSky)
    {
        delete m_pSky;
        m_pSky = 0;
    }

    //释放纹理
    m_textures.releaseTextures();

    m_rl.releaseGL();
    m_cylinder.releaseGL();
    m_cube.releaseGL();

}

void rdScene::loadData()
{
     //路线数据
    m_rl.readData();
    m_rl.computing();
    
    //读地形数据
    m_terrain.readSegPile();
    //每段地形使用的纹理信息
    m_terrain.readOrthophotoInfor();
    m_terrain.readOrthophotoData();
    m_terrain.readTriData();

    //路基数据
    m_roadbed.readData();

    //思贤桥
    float peg = 1580;
    _PosAng pa = m_rl.findPos(peg);
    
    // m_camera.setPos(QVector3D(pa.x, pa.y, pa.z + m_camera.getOffsetZ()));
    // m_camera.setYaw(pa.angle);
    m_originPoint = {(float)pa.x, (float)pa.y, (float)pa.z + m_camera.getOffsetZ()};
    m_originYaw = pa.angle;
 //    //路基数据
 //    m_roadbed.ReadData();

 //    //读地形数据
 //    m_terrain.ReadSegPile();
 //    //每段地形使用的纹理信息
 //    m_terrain.ReadOrthophotoInfor();
 //    m_terrain.ReadOrthophotoData();
 //    m_terrain.ReadTriData();
    // struct _PosAng pos = m_rl.FindPos(-900);

    // m_camera.SetPos(pos.x, pos.y, pos.z + m_camera.GetOffsetZ());
    // m_camera.SetYaw(pos.angle);
}

void rdScene::computing()
{
    m_roadbed.computing(m_rl);  
}

void rdScene::loadTextures()
{
    m_terrain.loadTerrainTexture();
    m_roadbed.loadTextures();
    m_textures.LoadTextures();
}

void rdScene::loadShaders()
{
    m_shaders.loadShaders();
}

void rdScene::initGeometry()
{ 
    //天空
    m_pSky = new rdSky;
    m_pSky->initGeometry();    
    m_rl.initGeometry();

    QOpenGLTexture* texture = m_textures.getTextureByName("SKY");
    if (texture) m_pSky->setTexture(texture);

    m_cylinder.initGeometry();
    m_cube.initGeometry();

    m_terrain.createVBO();
    m_roadbed.createVBO();
    //测试
    _conicalSlopePara csp;
    csp.h0 = 1.4;
    csp.h = 4;//;
    csp.scale = 1.5;
    csp.platwidth = 1.2;
    csp.middlewidth = 14;
    csp.wallheight = 2.0f;
    csp.leftwidth = 0.5;
    csp.rightwidth = 0.3;
    csp.downwidth = 0.3;
    csp.downheight = 0.5f;
    csp.heightscale1 = 0.2;
    csp.heightscale2 = 0.25;
    csp.downscale = 0.2;

    csp.basewidth = 2.8;
    csp.baseheight = 0.8;
    csp.baseleft = 0.8;//
    csp.baseright = 0.8;//
    csp.bHead = true;
    m_bcs.setPara(csp);
    csp.bHead = false;
    m_tailbcs.setPara(csp);

    m_bcs.initGeometry();
    m_bcs.setTextures(&m_textures);

    m_tailbcs.initGeometry();
    m_tailbcs.setTextures(&m_textures);
}

void rdScene::render(const QMatrix4x4& projection, const QMatrix4x4& view)
{
    // 天空
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLShaderProgram*pShader = m_shaders.getShaderByName("TEXTURE");
    if (pShader)
    {
        pShader->bind();
        m_pSky->Render(pShader);
        // qDebug() << "render sky";
    }
    // return;
    //清除深度缓存
    glClear(GL_DEPTH_BUFFER_BIT);
    // QMatrix4x4 projM = m_camera.getProjectionMatrix();
    // QMatrix4x4 viewM = m_camera.getViewMatrix();
    QMatrix4x4 projM = projection;
    QMatrix4x4 viewM = view;
    //绘制各合同段
    //地形
    pShader = m_shaders.getShaderByName("TERTEXT"); 
    if (pShader)
    {
        pShader->bind();
        QMatrix4x4 MVP = projM * viewM;
        pShader->setUniformValue("MVP", MVP);
        m_terrain.render(pShader);
    }
    // 路线
    glLineWidth(2);
    pShader = m_shaders.getShaderByName("COLOR");
    if (pShader)
    {
        //QMatrix4x4 modelMatrix;
        //modelMatrix.translate(0.0, 0.0, 0.0);
        QMatrix4x4 mvp = projM * viewM;
        pShader->bind();
        pShader->setUniformValue("MVP", mvp);
        pShader->setUniformValue("Color", QVector3D(0.2, 0.2, 0.2));
        m_rl.render(pShader);
    }
    //路面
    pShader = m_shaders.getShaderByName("COLOR");
    if (pShader)
    {
        QMatrix4x4 mvp = projM * viewM;
        pShader->bind();
        pShader->setUniformValue("MVP", mvp);
        pShader->setUniformValue("Color", QVector3D(0.2, 0.2, 0.2));
        m_roadbed.m_slop.renderSurface(pShader);
    }
    //斜坡
    pShader = m_shaders.getShaderByName("TERTEXT");
    if (pShader)
    {
        pShader->bind();
        QMatrix4x4 MVP = projM * viewM;
        pShader->setUniformValue("MVP", MVP);
        m_roadbed.m_slop.renderSlope(pShader);
    }
    //桥
    // pShader = m_shaders.getShaderByName("GouraudDir");
    // if (pShader)
    // {
    //     QMatrix4x4 modelMatrix;
    //     modelMatrix.translate(0.0, 0.0, 0.0);

    //     pShader->bind();
    //     pShader->setUniformValue("ProjectionMatrix", projM);
    //     pShader->setUniformValue("ModelMatrix", modelMatrix);
    //     pShader->setUniformValue("ViewMatrix", viewM);

    //     pShader->setUniformValue("lightColor", m_lightColor);
    //     pShader->setUniformValue("lightDirection", m_lightDirection);

    //     //m_cylinder.render(pShader);
    //     m_sections[0].renderBridge(pShader);
    // }
}

void rdScene::addBridge(rdBridge* pBridge)
{
    int section = pBridge->getSectionNo();
    //复制
    rdBridge* pClonepBridge = new rdBridge;
    pClonepBridge->copy(pBridge);
    pClonepBridge->setCube(&m_cube);
    pClonepBridge->setCylinder(&m_cylinder);

    if (section > 0 && section < 13)
       m_sections[section - 1].addBridge(pClonepBridge);
}

void rdScene::setCameraPosByBridge(rdBridge* pBridge)
{
    QVector3D minPt = pBridge->getMinPt();
    QVector3D maxPt = pBridge->getMaxPt();
    QVector3D center = (minPt + maxPt) / 2;

    m_camera.setPos(center);
    m_camera.setYaw(0);
}
