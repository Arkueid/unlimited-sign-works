#pragma once

/**
 * 加劲板/四棱柱 原点为 顶点0和顶点5连线的中点
 */

#include <base/Mesh.hpp>

#define siffenerIndexCount 48


/// @brief 加劲板
class Stiffener : public Mesh
{
    float vertices[siffenerIndexCount * 3];
    float normals[siffenerIndexCount * 3];
public:

    /// @brief 加劲板
    /// @param a 短底
    /// @param b 长底
    /// @param h1 短高
    /// @param h2 长高
    /// @param c 厚
    /// @param offset 嵌入偏移
    Stiffener(float a, float b, float h1, float h2, float c, float offsetH = 20.0f, float offsetV = 0.0f);
    ~Stiffener() override = default;

    float* getVertices() override
    {
        return vertices;
    }

    int getVertexCount() override
    {
        return siffenerIndexCount;
    }

    float *getNormals() override
    {
        return normals;
    }

};