#pragma once

#include <core.hpp>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class AxisDrawable: protected QOpenGLFunctions
{
    QOpenGLBuffer* vertexBuffer;
    QOpenGLShaderProgram program;

public:
    AxisDrawable();
    ~AxisDrawable();

    QOpenGLShaderProgram* getProgram();

    void draw(QMatrix4x4& projection, QMatrix4x4& view);
};