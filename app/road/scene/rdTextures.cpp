#include "rdTextures.h"

rdTextures::rdTextures()
{
    initializeOpenGLFunctions();

}

rdTextures::~rdTextures()
{
    releaseTextures();
}

void rdTextures::releaseTextures()
{
    while(m_textures.size())
    {
        rdTexture*pTexture = m_textures[0];
        delete pTexture;
        m_textures.erase(m_textures.begin());
    }
}

QOpenGLTexture* rdTextures::getTextureByName(QString name)
{
    for(int i=0; i<m_textures.size(); i++)
    {
        if (m_textures[i]->getName() == name)
            return m_textures[i]->getTexture();
    }

    return 0;
}

void rdTextures::LoadTextures()
{
    QString fname="../../assets/images/SKY.png";
    bool re = AddTexture("SKY",fname,true);
    if (re) qDebug() << "SKY ok";
    fname = "../../assets/images/stone.bmp";
    re = AddTexture("STONE", fname, true);
    if (re) qDebug() << "STONE ok";
    fname = "../../assets/images/cobblestone.bmp";
    re = AddTexture("COBBLESTONE", fname, true);
    if (re) qDebug() << "COBBLESTONE ok";
    
}

bool rdTextures::AddTexture(QString name,QString filename, bool bMirror)
{
    //是否已经存在
    for(int i=0; i<m_textures.size(); i++)
    {
        if (m_textures[i]->getName()==name)
            return false;
    }

    rdTexture*pTexture = new rdTexture;
    
    if (pTexture->loadTexture(filename, bMirror))
    {
        pTexture->setName(name);
        m_textures.push_back(pTexture);

        return true;
    }

    return false;
}

bool rdTextures::AddTexture(rdTexture* pTexture)
{
    if (!pTexture) return false;
    //是否已经存在
    QString name = pTexture->getName();
    for(int i=0; i<m_textures.size(); i++)
    {
        if (m_textures[i]->getName() == name)
            return false;
    }
    m_textures.push_back(pTexture);

    return true;
}

