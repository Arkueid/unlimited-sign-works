#ifndef RDTEXTURES_H
#define RDTEXTURES_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include <QOpenGLTexture>

#include "rdTexture.h"

class rdTextures : protected QOpenGLFunctions
{
public:
    rdTextures ();
    virtual ~rdTextures ();
    void releaseTextures();
    QOpenGLTexture* getTextureByName(QString name);
    void LoadTextures();
    bool AddTexture(QString name,QString filename, bool bMirror=false);
    bool AddTexture(rdTexture* pTexture);
private:
    QVector<rdTexture*> m_textures;
};

#endif // RDTEXTURES_H
