#pragma once

#include <core.hpp>

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual Vertex *getVertices() = 0;

    virtual int getVertexCount() = 0;

    virtual int getDrawableCount() = 0;

    virtual int getDrawableOffset(int index) = 0;

    virtual int getDrawableSize(int index) = 0;
};