#pragma once

#include <core.hpp>

enum MeshType
{
    MeshType_Cube,
    MeshType_Cylinder,
    MeshType_RoundedCube,
    MeshType_RoundedRegTriPrism,
    MeshType_Stiffener,
};

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual int getType() = 0;

    virtual Vertex *getVertices() = 0;

    virtual int getVertexCount() = 0;

    virtual int getDrawableCount() = 0;

    virtual int getDrawableOffset(int index) = 0;

    virtual int getDrawableSize(int index) = 0;
};