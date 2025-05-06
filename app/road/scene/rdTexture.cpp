#include "rdTexture.h"

rdTexture::rdTexture()
{
    initializeOpenGLFunctions();
}

rdTexture::~rdTexture()
{
    delete m_texture;
}

void rdTexture::setName(QString name)
{
    m_name = name;
}

QString rdTexture::getName()
{
    return m_name;
}

QOpenGLTexture* rdTexture::getTexture()
{
    return m_texture;
}

bool rdTexture::loadTexture(QString filename,bool bMirror)
{
    if (bMirror) m_texture = new QOpenGLTexture(QImage(filename).mirrored());
    else m_texture = new QOpenGLTexture(QImage(filename));

    GLuint id = m_texture->textureId();
    if (id <= 0)
    {
        delete m_texture;
        m_texture = 0;
        return false;
    }
    
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    return true;
}
