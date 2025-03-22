#pragma once
#include <base/Mesh.hpp>

#define cubeIndexCount 6 * 2 * 3
#define cubeVertexCount 8

class Cube : public Mesh
{
    float vertices[cubeVertexCount * 3];

public:
    Cube(float a, float b, float c);
    ~Cube() override = default;
    int getIndexCount() override
    {
        return cubeIndexCount;
    }
    unsigned short *getIndices() override;
    int getVertexCount() override
    {
        return cubeVertexCount;
    }
    float *getVertices() override
    {
        return vertices;
    }
};