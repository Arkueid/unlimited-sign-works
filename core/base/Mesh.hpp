#pragma once

#include <core.hpp>
#include <render/Drawable.hpp>

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual Vertex *getVertices() = 0;

    virtual int getVertexCount() = 0;
};