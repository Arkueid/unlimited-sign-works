#include <utils/VertexBufferManager.hpp>
#include <QDebug>
#include "VertexBufferManager.hpp"

struct BufferRef
{
    QOpenGLBuffer* buffer;
    int refCount;

    ~BufferRef()
    {
        buffer->destroy();
        delete buffer;
    }
};

static QMap<Vertex*, BufferRef*> vertexBuffers;

QOpenGLBuffer *VertexBufferManager::registerVertexBuffer(Vertex *vertices, int vertexCount)
{
    if (vertexBuffers.contains(vertices)) {
        BufferRef* bufRef = vertexBuffers.value(vertices);
        bufRef->refCount++;
        return bufRef->buffer;
    }

    QOpenGLBuffer *buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (buffer->create())
    {
        vertexBuffers.insert(vertices, new BufferRef{buffer, 1});
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

void VertexBufferManager::unregisterVertexBuffer(Vertex *vertices)
{
    if (vertexBuffers.contains(vertices))
    {
        BufferRef* bufRef = vertexBuffers.value(vertices);
        if (bufRef->refCount > 1)
        {
            bufRef->refCount--;
        }
        else
        {
            vertexBuffers.remove(vertices);
            delete bufRef;
        }
    }
}
void VertexBufferManager::releaseVertexBuffers()
{
    for (auto it = vertexBuffers.begin(); it != vertexBuffers.end(); ++it)
    {
        it.value()->buffer->destroy();
        delete it.value();
    }
    vertexBuffers.clear();
}
