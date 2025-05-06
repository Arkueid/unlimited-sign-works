#include "rdShader.h"

rdShader::rdShader()
{
    initializeOpenGLFunctions();
    m_shader = 0;
}

rdShader::~rdShader()
{

}

void rdShader::setName(QString name)
{
    m_name = name;
}

QString rdShader::getName()
{
    return m_name;
}

QOpenGLShaderProgram* rdShader::getShader()
{
    return m_shader;
}

bool rdShader::loadShader(QString vfilename,QString ffilename)
{
    m_shader = new QOpenGLShaderProgram;
    // Compile vertex shader
    if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vfilename))
       return false;
    // Compile fragment shader
    if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ffilename))
       return false;

    // Link shader pipeline
    if (!m_shader->link()) return false;

    return true;
}
