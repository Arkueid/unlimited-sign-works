#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <part/Part.hpp>


// 选中包围盒
class BoxDrawable : protected QOpenGLFunctions {

    QOpenGLBuffer* vertexBuffer;
    QOpenGLShaderProgram program;   
    QMatrix4x4 modelMatrix;

public:
    BoxDrawable();
    ~BoxDrawable();
    void wrap(Part *part, const QMatrix4x4& model);
    void draw(const QMatrix4x4& projection, const QMatrix4x4& view);
};