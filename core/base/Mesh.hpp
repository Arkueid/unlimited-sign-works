#pragma once

#include <core.hpp>

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual Vertex *getVertices() = 0;

    virtual int getVertexCount() = 0;
};