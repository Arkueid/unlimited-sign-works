#include "rdShaders.h"

rdShaders::rdShaders()
{
    initializeOpenGLFunctions();

}

rdShaders::~rdShaders()
{
    while(m_shaders.size())
    {
        rdShader*pShader = m_shaders[0];
        delete pShader;
        m_shaders.erase(m_shaders.begin());
    }
}

QOpenGLShaderProgram* rdShaders::getShaderByName(QString name)
{
    for(int i=0; i<m_shaders.size(); i++)
    {
        if (m_shaders[i]->getName()==name)
            return m_shaders[i]->getShader();
    }

    return 0;
}

void rdShaders::loadShaders()
{
    //纯颜色
    QString root = "../../assets/road/";
    QString vfname = root + "shaders/color.vs";
    QString ffname = root + "shaders/color.fs";
    bool re = addShader("COLOR", vfname, ffname);

    //纹理
    vfname = root + "shaders/vshader.glsl";
    ffname = root + "shaders/fshader.glsl";
    re = addShader("TEXTURE", vfname, ffname);

    //地形纹理
    vfname = root + "shaders/texture.vs";
    ffname = root + "shaders/texture.fs";
    re = addShader("TERTEXT", vfname, ffname);

    //Gouraud方向光gourauddirectlight
    vfname = root + "shaders/gourauddirectlight.vs";
    ffname = root + "shaders/gourauddirectlight.fs";
    re = addShader("GouraudDir", vfname, ffname);
}

bool rdShaders::addShader(QString name,QString vfilename,QString ffilename)
{
    //是否已经存在
    for(int i=0; i<m_shaders.size(); i++)
    {
        if (m_shaders[i]->getName() == name)
            return false;
    }

    rdShader*pShader = new rdShader;
    if (pShader->loadShader(vfilename, ffilename))
    {
        m_shaders.push_back(pShader);
        pShader->setName(name);
        return true;
    }

    delete pShader;
    return false;
}


