#pragma once

#include <part/pole/Pole168.hpp>

#include <base/Mesh.hpp>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class GLObject : protected QOpenGLFunctions
{
    Mesh *mesh;
    QMatrix4x4 *modelMatrix;
    QOpenGLBuffer vertexBuffer; // TODO: arraybuffer manager
    // TODO: bool visible

public:
    GLObject(Mesh *mesh, QMatrix4x4 *modelMatrix);

    void draw(QOpenGLShaderProgram *program);
};