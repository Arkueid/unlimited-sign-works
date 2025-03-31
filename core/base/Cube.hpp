#pragma once
// 建模中心为正方体中心

#include <base/Mesh.hpp>

#define cubeVertexCount 6 * 2 * 3

class Cube : public Mesh
{
    Vertex vertices[cubeVertexCount];
    float a, b, c;

    Cube(float a, float b, float c);
public:
    ~Cube() override;

    static Cube* create(float a, float b, float c);

    int getType() override
    {
        return MeshType_Cube;
    }
    int getVertexCount() override
    {
        return cubeVertexCount;
    }
    Vertex *getVertices() override
    {
        return vertices;
    }

    int getDrawableCount() override;
    int getDrawableOffset(int index) override;
    int getDrawableSize(int index) override;

    float getA() const
    {
        return a;
    }
    float getB() const 
    {
        return b;
    }

    float getC() const
    {
        return c;
    }
};