#ifndef RDSHADERS_H
#define RDSHADERS_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include <QOpenGLTexture>

#include "rdShader.h"

class rdShaders : protected QOpenGLFunctions
{
public:
    rdShaders ();
    virtual ~rdShaders ();
    QOpenGLShaderProgram* getShaderByName(QString name);
    void loadShaders();
    bool addShader(QString name,QString vfilename,QString ffilename);
private:
    QVector<rdShader*> m_shaders;
};

#endif // RDSHADERS_H
