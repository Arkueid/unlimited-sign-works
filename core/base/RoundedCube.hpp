#pragma once
#include <base/Mesh.hpp>

// 上下面为圆角矩形
// 侧面为矩形+圆柱面
// 建模中心：底面中心

class RoundedCube :
    public Mesh
{
    float a, b, c, radius;
    Vertex* vertices;
    int vertexCount;

    int drawableSize[3];
    int drawableOffset[3];

public:
    /// @brief 
    /// @param a x方向长度
    /// @param b z方向长度
    /// @param c y方向长度
    /// @param radius xz面圆角大小
    /// @param precision 圆角离散精度
    RoundedCube(float a, float b, float c, float radius, float precision = PRECISION_DEFAULT);
    ~RoundedCube() override;
    // 通过 Mesh 继承
    Vertex* getVertices() override;
    int getVertexCount() override;
    int getDrawableCount() override;
    int getDrawableOffset(int index) override;
    int getDrawableSize(int index) override;
};

