#include <base/Stiffener.hpp>

// 加劲板共用索引
static unsigned short indices[siffenerIndexCount] = {
    0, 1, 4, 1, 2, 4, 2, 3, 4, // 梯形面1
    5, 6, 9, 6, 7, 9, 7, 8, 9, // 梯形面2
    1, 2, 6, 2, 6, 7,          // 侧面 ac
    0, 1, 5, 1, 5, 6,          // 侧面 h2-c
    2, 3, 7, 3, 7, 8,          // 侧面 斜边-c
    3, 4, 8, 4, 9, 8,          // 侧面 h1-c
    0, 4, 5, 4, 5, 9,          // 侧面 bc
};

Stiffener::Stiffener(float a, float b, float h1, float h2, float c, float offsetH, float offsetV) : a(a), b(b), c(c), h1(h1), h2(h2), offsetH(offsetH), offsetV(offsetV)
{
    float z = c / 2;
    int index = 0;
    float points[10 * 3];
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
}