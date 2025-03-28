#include <base/Stiffener.hpp>
#include "Stiffener.hpp"

// 加劲板共用索引
static unsigned short indices[siffenerIndexCount] = {
    0, 1, 4, 1, 2, 4, 2, 3, 4, // 梯形面1
    5, 6, 9, 6, 7, 9, 7, 8, 9, // 梯形面2
    1, 2, 7, 1, 6, 7,          // 侧面 ac
    0, 5, 6, 0, 1, 6,          // 侧面 h2-c
    2, 3, 8, 2, 7, 8,          // 侧面 斜边-c
    3, 4, 9, 3, 8, 9,          // 侧面 h1-c
    5, 9, 4, 5, 0, 4,          // 侧面 bc
};

static int drawableSize[7] = { 9, 9, 6, 6, 6, 6, 6 };
static int drawableOffset[7] = { 0, 9, 18, 24, 30, 36, 42 };

Stiffener::Stiffener(float a, float b, float h1, float h2, float c, float offsetH, float offsetV) : a(a), b(b), c(c), h1(h1), h2(h2), offsetH(offsetH), offsetV(offsetV)
{
    float z = c / 2;
    int index = 0;
    int uvIndex = 0;
    float points[10 * 3];
    float uvs[10 * 2];
    for (int i = 0; i < 2; i++)
    {
        // x, y, z
        if (i == 1)
        {
            z = -z;
        }

        // 0~4;5~9
        points[index++] = -offsetH;
        points[index++] = -offsetV, points[index++] = z;
        points[index++] = h2;
        points[index++] = -offsetV, points[index++] = z;
        points[index++] = h2;
        points[index++] = a, points[index++] = z;
        points[index++] = h1;
        points[index++] = b, points[index++] = z;
        points[index++] = -offsetH;
        points[index++] = b, points[index++] = z;
    }

    index = 0;
    for (int i = 0; i < siffenerIndexCount; i++)
    {
        vertices[index].x = points[indices[i] * 3 + 0];
        vertices[index].y = points[indices[i] * 3 + 1];
        vertices[index].z = points[indices[i] * 3 + 2];
        index++;
    }

    // 梯形面1
    for (int i = 0; i < 9; i++)
    {
        vertices[i].nx = 0;
        vertices[i].ny = 0;
        vertices[i].nz = 1;
    }
    // 梯形面2
    for (int i = 9; i < 18; i++)
    {
        vertices[i].nx = 0;
        vertices[i].ny = 0;
        vertices[i].nz = -1;
    }
    // 侧面 ac
    for (int i = 18; i < 24; i++)
    {
        vertices[i].nx = 1;
        vertices[i].ny = 0;
        vertices[i].nz = 0;
    }
    // 侧面 h2-c
    for (int i = 24; i < 30; i++)
    {
        vertices[i].nx = 0;
        vertices[i].ny = -1;
        vertices[i].nz = 0;
    }
    // 侧面 斜边-c
    for (int i = 30; i < 36; i++)
    {
        vertices[i].nx = b - a;
        vertices[i].ny = h2 - h1;
        vertices[i].nz = 0;
    }
    // 侧面 h1-c
    for (int i = 36; i < 42; i++)
    {
        vertices[i].nx = 0;
        vertices[i].ny = 1;
        vertices[i].nz = 0;
    }
    // 侧面 bc
    for (int i = 42; i < 48; i++)
    {
        vertices[i].nx = -1;
        vertices[i].ny = 0;
        vertices[i].nz = 0;
    }

    // 纹理
    // 梯形面1，梯形面2
    for (int i = 0; i < 2; i++)
    {
        vertices[i * 9 + 0].u = 0;
        vertices[i * 9 + 0].v = 0;
        vertices[i * 9 + 1].u = 1;
        vertices[i * 9 + 1].v = 0;
        vertices[i * 9 + 2].u = 0;
        vertices[i * 9 + 2].v = 1;

        vertices[i * 9 + 3].u = 1;
        vertices[i * 9 + 3].v = 0;
        vertices[i * 9 + 4].u = 1;
        vertices[i * 9 + 4].v = a / b;
        vertices[i * 9 + 5].u = 0;
        vertices[i * 9 + 5].v = 1;

        vertices[i * 9 + 6].u = 1;
        vertices[i * 9 + 6].v = a / b;
        vertices[i * 9 + 7].u = h1 / h2;
        vertices[i * 9 + 7].v = 1;
        vertices[i * 9 + 8].u = 0;
        vertices[i * 9 + 8].v = 1;
    }
    // 侧面
    for (int i = 2; i < 7 /* 五个面 */; i++)
    {
        vertices[i * 9 + 0].u = 0;
        vertices[i * 9 + 0].v = 0;
        vertices[i * 9 + 1].u = 1;
        vertices[i * 9 + 1].v = 0;
        vertices[i * 9 + 2].u = 1;
        vertices[i * 9 + 2].v = 1;

        vertices[i * 9 + 0].u = 0;
        vertices[i * 9 + 0].v = 0;
        vertices[i * 9 + 1].u = 1;
        vertices[i * 9 + 1].v = 0;
        vertices[i * 9 + 2].u = 1;
        vertices[i * 9 + 2].v = 1;
    }
}

Stiffener::~Stiffener()
{
}
int Stiffener::getDrawableCount()
{
    return 7;
}

int Stiffener::getDrawableOffset(int index)
{
    return drawableOffset[index];
}

int Stiffener::getDrawableSize(int index)
{
    return drawableSize[index];
}
