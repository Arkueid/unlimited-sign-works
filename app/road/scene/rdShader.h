#ifndef RDSHADER_H
#define RDSHADER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QString>
#include <QOpenGLTexture>

class rdShader : protected QOpenGLFunctions
{
public:
    rdShader();
    virtual ~rdShader();
    
    void setName(QString name);
    QString getName();
    QOpenGLShaderProgram* getShader();
    bool loadShader(QString vfilename,QString ffilename);
private:
    QString m_name;
    QOpenGLShaderProgram* m_shader;
};

#endif // RDSHADER_H
