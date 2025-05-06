
#ifndef RDTERRAINTEXTUREMANAGER_H
#define RDTERRAINTEXTUREMANAGER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QVector>

struct _terraintexture
{
    QOpenGLTexture* pTexture;
    QString sName;
};

class rdTerrainTextureManager : protected QOpenGLFunctions
{
public:
    rdTerrainTextureManager();
    virtual ~rdTerrainTextureManager();

    bool addTexture(QString fname);
    bool deleteTexture(QString fname);
    _terraintexture* searchTexture(QString fname);
private:
    QVector<_terraintexture *> m_textures;
};

#endif // RDTERRAINTEXTUREMANAGER_H
