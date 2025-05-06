
#ifndef RDSKY_H
#define RDSKY_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>

class rdSky : protected QOpenGLFunctions
{
public:
    rdSky();
    virtual ~rdSky();

    void Render(QOpenGLShaderProgram *program);
    void initGeometry();
    void setProjection(QMatrix4x4 matrix);
    void setTexture(QOpenGLTexture* pTexture);
private: 
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QMatrix4x4 projectionSky;
    QOpenGLTexture* textSky;
};

#endif // RDSKY_H
