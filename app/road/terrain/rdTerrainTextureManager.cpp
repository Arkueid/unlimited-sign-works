#include "rdTerrainTextureManager.h"
#include <QImage>
#include <QCoreApplication>

rdTerrainTextureManager::rdTerrainTextureManager()
{
    initializeOpenGLFunctions();

    m_textures.clear();
}

rdTerrainTextureManager::~rdTerrainTextureManager()
{
    while (m_textures.size())
    {
        _terraintexture* first = m_textures.first();
        m_textures.pop_front();
        delete first->pTexture;

        delete first;
    }
}

_terraintexture* rdTerrainTextureManager::searchTexture(QString fname)
{
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
        if ((*it)->sName == fname)
            return *it;
    }

    return 0;
}

bool rdTerrainTextureManager::deleteTexture(QString fname)
{
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
        if ((*it)->sName == fname)
        {
            m_textures.erase(it);
            return true;
        }
    }

    return false;
}

bool rdTerrainTextureManager::addTexture(QString fname)
{
    //先查找是否已经存在
    _terraintexture* ptt = searchTexture(fname);
    if (ptt) return true;

    QString filepath = QCoreApplication::applicationDirPath() + "/../../assets/data/terrain/orthophoto/";
    QString filename = filepath + fname;
    QImage image;
    if (!image.load(filename)) return false;

    QOpenGLTexture *pTexture = new QOpenGLTexture(image.mirrored());
    if (pTexture)
    {
        pTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapNearest);//QOpenGLTexture::Nearest);
        // Set bilinear filtering mode for texture magnification
        pTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);// QOpenGLTexture::Linear);
        // Wrap texture coordinates by repeating
        // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
        pTexture->setWrapMode(QOpenGLTexture::Repeat);

        _terraintexture* tt = new _terraintexture;
        tt->pTexture = pTexture;
        tt->sName = fname;

        m_textures.push_back(tt);
    }
    return true;
}