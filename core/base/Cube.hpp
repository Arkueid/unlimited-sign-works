#pragma once
// 建模中心为正方体中心

#include <base/Mesh.hpp>

#define cubeIndexCount 6 * 2 * 3

class Cube : public Mesh
{
    float vertices[cubeIndexCount * 3];

public:
    Cube(float a, float b, float c);
    ~Cube() override = default;
    int getVertexCount() override
    {
        return cubeIndexCount;
    }
    float *getVertices() override
    {
        return vertices;
    }
    float* getNormals() override;
};