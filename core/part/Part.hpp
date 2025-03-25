#pragma once

#include <base/Mesh.hpp>

#include <QMatrix4x4>

class Part
{
    float originX, originY, originZ;
    float rotationX, rotationY, rotationZ;
    Mesh *mesh;

public:
    Part(Mesh *mesh,
         float originX,
         float originY,
         float originZ,
         float rotationX,
         float rotationY,
         float rotationZ);

    virtual ~Part() = default;

    float getOriginX() const
    {
        return originX;
    }
    float getOriginY() const
    {
        return originY;
    }
    float getOriginZ() const
    {
        return originZ;
    }
    float getRotationX() const
    {
        return rotationX;
    }
    float getRotationY() const
    {
        return rotationY;
    }
    float getRotationZ() const
    {
        return rotationZ;
    }
    Mesh *getMesh() const
    {
        return mesh;
    }
};