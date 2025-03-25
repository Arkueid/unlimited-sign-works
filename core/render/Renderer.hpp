#pragma once

#include <QMatrix4x4>

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void onDraw(
        unsigned int vao,
        unsigned int textureId,
        int vertexCount,
        QMatrix4x4 &modelMatrix) = 0;
};