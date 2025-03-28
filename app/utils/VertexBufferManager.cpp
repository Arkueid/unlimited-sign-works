#include <utils/VertexBufferManager.hpp>
#include <QDebug>


static QMap<Vertex*, QOpenGLBuffer*> vertexBuffers;

QOpenGLBuffer *VertexBufferManager::registerVertexBuffer(Vertex *vertices, int vertexCount)
{
    if (vertexBuffers.contains(vertices)) return vertexBuffers.value(vertices);
    
    QOpenGLBuffer *buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (buffer->create())
    {
        vertexBuffers.insert(vertices, buffer);
        buffer->bind();
        buffer->allocate(vertices, sizeof(Vertex) * vertexCount);
        buffer->release();
        return buffer;
    }
    else
    {
        delete buffer;
        qDebug() << "Failed to create vertex buffer";
        return nullptr;
    }
    return buffer;
}

void VertexBufferManager::releaseVertexBuffers()
{
    for (auto it = vertexBuffers.begin(); it != vertexBuffers.end(); ++it)
    {
        it.value()->destroy();
        delete it.value();
    }
    vertexBuffers.clear();
}
