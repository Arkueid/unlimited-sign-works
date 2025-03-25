#pragma once

#include <part/pole/Pole168.hpp>

#include <base/Mesh.hpp>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Drawable : protected QOpenGLFunctions
{
    Mesh *mesh;
    QMatrix4x4 *modelMatrix;
    QOpenGLBuffer vertexBuffer; // TODO: arraybuffer manager
    // TODO: bool visible

public:
    Drawable(Mesh *mesh, QMatrix4x4 *modelMatrix);

    void draw(QOpenGLShaderProgram *program);
};