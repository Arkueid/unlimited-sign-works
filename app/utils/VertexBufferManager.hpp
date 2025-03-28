#pragma once

#include <QOpenGLBuffer>
#include <core.hpp>

class VertexBufferManager {
public:
    static QOpenGLBuffer* registerVertexBuffer(Vertex* vertices, int vertexCount);
    static void releaseVertexBuffers();
};