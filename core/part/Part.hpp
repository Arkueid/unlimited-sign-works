#pragma once

#include <base/Mesh.hpp>

#include <QMatrix4x4>

class Part
{
    float originX, originY, originZ;
    float rotationX, rotationY, rotationZ;
    Mesh* mesh;
    QMatrix4x4 modelMatrix;

public:
    Part(Mesh* mesh, float originX, float originY, float originZ, float rotationX, float rotationY, float rotationZ);
    virtual ~Part() = default;

    QMatrix4x4 getModelMatrix()
    {
        return modelMatrix;
    }

    Vertex *getVertices() const
    {
        return mesh->getVertices();
    }

    int getVertexCount() const
    {
        return mesh->getVertexCount();
    }

    float getOriginX()
    {
        return originX;
    }
    float getOriginY()
    {
        return originY;
    }
    float getOriginZ()
    {
        return originZ;
    }
    float getRotationX()
    {
        return rotationX;
    }
    float getRotationY()
    {
        return rotationY;
    }
    float getRotationZ()
    {
        return rotationZ;
    }
};