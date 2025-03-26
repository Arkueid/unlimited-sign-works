#pragma once
#include <base/Mesh.hpp>

#include <utility>

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

    float halfA, halfB, halfC;
public:
    /// <summary>
    /// </summary>
    /// <param name="a">x方向长度</param>
    /// <param name="b">z方向长度</param>
    /// <param name="c">y方向长度</param>
    /// <param name="radius">xz面圆角大小</param>
    /// <param name="precision">圆角离散精度</param>
    RoundedCube(float a, float b, float c, float radius, float precision = PRECISION_DEFAULT);
    ~RoundedCube();
    // 通过 Mesh 继承
    Vertex* getVertices() override;
    int getVertexCount() override;
    int getDrawableCount() override;
    int getDrawableOffset(int index) override;
    int getDrawableSize(int index) override;
};

