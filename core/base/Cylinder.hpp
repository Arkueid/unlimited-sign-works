#pragma once
/**
 * 建模原点：底圆圆心
 */

#include <base/Mesh.hpp>

class Cylinder : public Mesh {

    int indexCount;
    int pointCount;
    Vertex *vertices;
    float h;
    float d;
public:
    /// @brief 圆柱
    /// @param d 直径
    /// @param h 高
    /// @param precision 每单位长度的顶点数 
    Cylinder(float d, float h, float precision = 1.0f / 20.0f);
    ~Cylinder() override;

    int getVertexCount() override
    {
        return indexCount;
    }
    Vertex* getVertices() override
    {
        return vertices;
    }
};