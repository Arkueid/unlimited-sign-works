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
    int drawableSize[3];
    int drawableOffset[3];
public:
    /// @brief 圆柱
    /// @param d 直径
    /// @param h 高
    /// @param precision 每单位长度的顶点数 
    Cylinder(float d, float h, float precision = PRECISION_DEFAULT);
    ~Cylinder() override;

    int getType() override
    {
        return MeshType_Cylinder;
    }

    int getVertexCount() override
    {
        return indexCount;
    }
    Vertex* getVertices() override
    {
        return vertices;
    }

    int getDrawableCount() override;
    int getDrawableOffset(int index) override;
    int getDrawableSize(int index) override;

    float getD() const
    {
        return d;
    }

    float getH() const
    {
        return h;
    }
};