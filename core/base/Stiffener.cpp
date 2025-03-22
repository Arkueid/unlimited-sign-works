#include <base/Stiffener.hpp>

// 加劲板共用索引
static unsigned short indices[siffenerIndexCount] = {
    0, 1, 4, 1, 2, 4, 2, 3, 4, // 梯形面1
    5, 6, 9, 6, 7, 9, 7, 8, 9, // 梯形面2
    1, 2, 6, 2, 6, 7, // 侧面 ac
    0, 1, 5, 1, 5, 6, // 侧面 h2-c
    2, 3, 7, 3, 7, 8, // 侧面 斜边-c
    3, 4, 8, 4, 9, 8, // 侧面 h1-c
    0, 4, 5, 4, 5, 9, // 侧面 bc
};

Stiffener::Stiffener(float a, float b, float h1, float h2, float c, float offsetH, float offsetV)
{
    float z = c / 2;
    int index = 0;
    for (int i = 0; i < 2; i++)
    {
        // x, y, z
        if (i == 1) 
        {
            z = -z;
        }

        // 0~4;5~9
        vertices[index++] = -offsetH; vertices[index++] = -offsetV, vertices[index++] = z;
        vertices[index++] = h2; vertices[index++] = -offsetV, vertices[index++] = z;
        vertices[index++] = h2; vertices[index++] = a, vertices[index++] = z;
        vertices[index++] = h1; vertices[index++] = b, vertices[index++] = z;
        vertices[index++] = -offsetH; vertices[index++] = b, vertices[index++] = z;
    }
}
unsigned short *Stiffener::getIndices()
{
    return indices;
}