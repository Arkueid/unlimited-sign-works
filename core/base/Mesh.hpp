#pragma once

#include <core.hpp>

enum MeshType
{
    MeshType_Cube = 0,
    MeshType_Cylinder,
    MeshType_Stiffener,
    MeshType_RoundedCube,
    MeshType_RoundedRegTriPrism,
};

#include <iostream>

class Mesh
{
    int refCount;

public:
    Mesh() : refCount(0) {}
    virtual ~Mesh()
    {
        // printf("deleted\n");
    }

    virtual int getType() = 0;

    void incRef()
    {
        refCount++;
    }

    void decRef()
    {
        refCount--;
    }

    int getRefCount()
    {
        return refCount;
    }

    virtual Vertex *getVertices() = 0;

    virtual int getVertexCount() = 0;

    virtual int getDrawableCount() = 0;

    virtual int getDrawableOffset(int index) = 0;

    virtual int getDrawableSize(int index) = 0;
};