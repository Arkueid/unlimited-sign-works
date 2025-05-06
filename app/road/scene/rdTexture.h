#ifndef RDTEXTURE_H
#define RDTEXTURE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QString>
#include <QOpenGLTexture>

class rdTexture : protected QOpenGLFunctions
{
public:
    rdTexture ();
    virtual ~rdTexture ();
    
    void setName(QString name);
    QString getName();
    QOpenGLTexture* getTexture();
    bool loadTexture(QString filename,bool bMirror = false);
private:
    QString m_name;
    QOpenGLTexture *m_texture;
};

#endif // RDTEXTURE_H
