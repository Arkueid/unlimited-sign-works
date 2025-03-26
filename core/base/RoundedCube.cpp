#include <base/RoundedCube.h>
#include <algorithm>
#include <base/Cube.hpp>
#include <base/Cylinder.hpp>

RoundedCube::RoundedCube(float a, float b, float c, float r, float precision)
    : a(a), b(b), c(c), radius(std::min(std::min(a, b) / 2, r)), vertices(nullptr)
{
    // 圆周上离散点个数
    const float perimeter_1_4 = PI / 2.0f * radius;
    const int numOnCorner = static_cast<int>(perimeter_1_4 * precision);

    // 8 个圆心，顺序：上底面左上角顺时针
    const float halfA = a / 2, halfB = b / 2, halfC = c / 2;
    float center0X = -(halfA - radius);
    float center0Z = -(halfB - radius);
    Point centers[8] = {
        {center0X, c, center0Z},
        {-center0X, c, center0Z},
        {-center0X, c, -center0Z},
        {center0X, c, -center0Z},
        {center0X, 0, center0Z},
        {-center0X, 0, center0Z},
        {-center0X, 0, -center0Z},
        {center0X, 0, -center0Z},
    };

    vertexCount = numOnCorner * 8 * 3 + (3 * 2 + 4) * 2 * 3 + 4 * numOnCorner * 2 * 3;
    vertices = new Vertex[vertexCount];

    // 上底面
    drawableOffset[0] = 0;
    // 左圆
    float angle = PI;
    const float angleStep = PI / 2.0f / numOnCorner;
    int vertexIndex = 0;
    for (int i = 0; i < numOnCorner; i++)
    {
        vertices[vertexIndex].x = cosf(angle) * radius + centers[0].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle) * radius + centers[0].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = 1 - (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[0].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[0].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = 1 - (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = centers[0].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = centers[0].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = radius / a;
        vertices[vertexIndex].v = (abs(centers[0].z) + halfB) / b;
        angle += angleStep;
        vertexIndex++;
    }
    // 中间大矩形
    vertices[vertexIndex++] = {centers[0].x, c, -halfB, 0, 1, 0, radius / a, 1};
    vertices[vertexIndex++] = {centers[1].x, c, -halfB, 0, 1, 0, (a - radius) / a, 1};
    vertices[vertexIndex++] = {centers[0].x, c, halfB, 0, 1, 0, radius / a, 0};
    vertices[vertexIndex++] = {centers[1].x, c, halfB, 0, 1, 0, (a - radius) / a, 0};
    vertices[vertexIndex++] = {centers[1].x, c, -halfB, 0, 1, 0, (a - radius) / a, 1};
    vertices[vertexIndex++] = {centers[0].x, c, halfB, 0, 1, 0, radius / a, 0};
    // 右上角圆
    for (int i = 0; i < numOnCorner; i++)
    {
        vertices[vertexIndex].x = cosf(angle) * radius + centers[1].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle) * radius + centers[1].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[1].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[1].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = centers[1].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = centers[1].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (a - radius) / a;
        vertices[vertexIndex].v = (abs(centers[1].z) + halfB) / b;
        vertexIndex++;
        angle += angleStep;
    }
    // 右矩形
    vertices[vertexIndex++] = {centers[1].x, c, centers[1].z, 0, 1, 0, (a - radius) / a, (abs(centers[1].z) + halfB) / b};
    vertices[vertexIndex++] = {halfA, c, centers[1].z, 0, 1, 0, 1, (abs(centers[1].z) + halfB) / b};
    vertices[vertexIndex++] = {centers[1].x, c, centers[2].z, 0, 1, 0, (a - radius) / a, radius / b};
    vertices[vertexIndex++] = {halfA, c, centers[2].z, 0, 1, 0, 1, radius / b};
    vertices[vertexIndex++] = {halfA, c, centers[1].z, 0, 1, 0, 1, (abs(centers[1].z) + halfB) / b};
    vertices[vertexIndex++] = {centers[1].x, c, centers[2].z, 0, 1, 0, (a - radius) / a, radius / b};
    // 右下角圆
    for (int i = 0; i < numOnCorner; i++)
    {
        vertices[vertexIndex].x = cosf(angle) * radius + centers[2].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle) * radius + centers[2].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = 1 - (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[2].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[2].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = 1 - (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = centers[2].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = centers[2].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = (a - radius) / a;
        vertices[vertexIndex].v = radius / b;
        vertexIndex++;
        angle += angleStep;
    }
    // 左下角圆
    for (int i = 0; i < numOnCorner; i++)
    {
        vertices[vertexIndex].x = cosf(angle) * radius + centers[3].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle) * radius + centers[3].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = 1 - (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = 1 - (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[3].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[3].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = 1 - (abs(vertices[vertexIndex].x) + halfA) / a;
        vertices[vertexIndex].v = 1 - (abs(vertices[vertexIndex].z) + halfB) / b;
        vertexIndex++;
        vertices[vertexIndex].x = centers[3].x;
        vertices[vertexIndex].y = c;
        vertices[vertexIndex].z = centers[3].z;
        vertices[vertexIndex].nx = 0;
        vertices[vertexIndex].ny = 1;
        vertices[vertexIndex].nz = 0;
        vertices[vertexIndex].u = radius / a;
        vertices[vertexIndex].v = radius / b;
        vertexIndex++;
        angle += angleStep;
    }
    // 左矩形
    vertices[vertexIndex++] = {-halfA, c, centers[0].z, 0, 1, 0, 0, 1 - radius / b};
    vertices[vertexIndex++] = {centers[0].x, c, centers[0].z, 0, 1, 0, radius / a, 1 - radius / b};
    vertices[vertexIndex++] = {-halfA, c, centers[3].z, 0, 1, 0, 0, radius / b};
    vertices[vertexIndex++] = {centers[3].x, c, centers[3].z, 0, 1, 0, radius / a, radius / b};
    vertices[vertexIndex++] = {centers[0].x, c, centers[0].z, 0, 1, 0, radius / a, 1 - radius / b};
    vertices[vertexIndex++] = {-halfA, c, centers[3].z, 0, 1, 0, 0, radius / b};

    drawableSize[0] = vertexIndex;

    drawableOffset[1] = vertexIndex;
    // 底面
    for (int i = drawableOffset[0]; i < drawableSize[0]; i++)
    {
        vertices[vertexIndex] = vertices[i];
        vertices[vertexIndex].y = 0;
        vertices[vertexIndex].ny = -1;
        vertexIndex++;
    }

    drawableSize[1] = drawableSize[0];
    drawableOffset[2] = vertexIndex;

    // 侧面
    angle = PI;
    const float innerA = a - radius * 2;
    const float innerB = b - radius * 2;
    const float totalLength = perimeter_1_4 * 4 + (innerA) * 2 + (innerB) * 2;
    const float uStep = perimeter_1_4 / totalLength / numOnCorner;
    float u = 0;
    float uStage[4] = {0, 
        (perimeter_1_4 + innerA) / totalLength, 
        (perimeter_1_4 * 2 + innerA + innerB) / totalLength, 
        (perimeter_1_4 * 3 + innerA + innerB + innerA) / totalLength};
    for (int i = 0; i < 4; i++)
    {
        u = uStage[i];
        for (int j = 0; j < numOnCorner; j++)
        {
            // 上三角
            vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
            vertices[vertexIndex].y = c;
            vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle);
            vertices[vertexIndex].u = u;
            vertices[vertexIndex].v = 1;
            vertexIndex++;
            vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
            vertices[vertexIndex].y = c;
            vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle + angleStep);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle + angleStep);
            vertices[vertexIndex].u = u + uStep;
            vertices[vertexIndex].v = 1;
            vertexIndex++;
            vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
            vertices[vertexIndex].y = 0;
            vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle);
            vertices[vertexIndex].u = u;
            vertices[vertexIndex].v = 0;
            vertexIndex++;
            // 下三角
            vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
            vertices[vertexIndex].y = 0;
            vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle + angleStep);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle + angleStep);
            vertices[vertexIndex].u = u + uStep;
            vertices[vertexIndex].v = 0;
            vertexIndex++;
            vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
            vertices[vertexIndex].y = 0;
            vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle);
            vertices[vertexIndex].u = u;
            vertices[vertexIndex].v = 0;
            vertexIndex++;
            vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
            vertices[vertexIndex].y = c;
            vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
            vertices[vertexIndex].nx = cosf(angle + angleStep);
            vertices[vertexIndex].ny = 0;
            vertices[vertexIndex].nz = sinf(angle + angleStep);
            vertices[vertexIndex].u = u + uStep;
            vertices[vertexIndex].v = 1;
            vertexIndex++;
            u += uStep;
            angle += angleStep;
        }
    }
    float uStage2[4] = {perimeter_1_4 / totalLength, 
        (perimeter_1_4 * 2 + innerA) / totalLength, 
        (perimeter_1_4 * 3 + innerA + innerB) / totalLength, 
        (perimeter_1_4 * 4 + innerA + innerB + innerA) / totalLength};
    // 侧面矩形x4
    // 后侧面
    vertices[vertexIndex++] = {centers[0].x, c, -halfB, 0, 0, -1, uStage2[0], 1};
    vertices[vertexIndex++] = {centers[1].x, c, -halfB, 0, 0, -1, uStage[1], 1};
    vertices[vertexIndex++] = {centers[0].x, 0, -halfB, 0, 0, -1, uStage2[0], 0};
    vertices[vertexIndex++] = {centers[1].x, 0, -halfB, 0, 0, -1, uStage[1], 0};
    vertices[vertexIndex++] = {centers[1].x, c, -halfB, 0, 0, -1, uStage[1], 1};
    vertices[vertexIndex++] = {centers[0].x, 0, -halfB, 0, 0, -1, uStage2[0], 0};
    // 右侧面
    vertices[vertexIndex++] = {halfA, c, centers[1].z, 1, 0, 0, uStage2[1], 1};
    vertices[vertexIndex++] = {halfA, c, centers[3].z, 1, 0, 0, uStage[2], 1};
    vertices[vertexIndex++] = {halfA, 0, centers[1].z, 1, 0, 0, uStage2[1], 0};
    vertices[vertexIndex++] = {halfA, 0, centers[3].z, 1, 0, 0, uStage[2], 0};
    vertices[vertexIndex++] = {halfA, c, centers[3].z, 1, 0, 0, uStage[2], 1};
    vertices[vertexIndex++] = {halfA, 0, centers[1].z, 1, 0, 0, uStage2[1], 0};
    // 前面
    vertices[vertexIndex++] = {centers[2].x, c, halfB, 0, 0, 1, uStage2[2], 1};
    vertices[vertexIndex++] = {centers[2].x, 0, halfB, 0, 0, 1, uStage2[2], 0};
    vertices[vertexIndex++] = {centers[3].x, c, halfB, 0, 0, 1, uStage[3], 1};
    vertices[vertexIndex++] = {centers[3].x, 0, halfB, 0, 0, 1, uStage[3], 0};
    vertices[vertexIndex++] = {centers[2].x, 0, halfB, 0, 0, 1, uStage2[2], 0};
    vertices[vertexIndex++] = {centers[3].x, c, halfB, 0, 0, 1, uStage[3], 1};
    // 左面
    vertices[vertexIndex++] = {-halfA, c, centers[3].z, -1, 0, 0, uStage2[3], 1};
    vertices[vertexIndex++] = {-halfA, c, centers[0].z, -1, 0, 0, uStage[0], 1};
    vertices[vertexIndex++] = {-halfA, 0, centers[3].z, -1, 0, 0, uStage2[3], 0};
    vertices[vertexIndex++] = {-halfA, 0, centers[0].z, -1, 0, 0, uStage[0], 0};
    vertices[vertexIndex++] = {-halfA, c, centers[0].z, -1, 0, 0, uStage[0], 1};
    vertices[vertexIndex++] = {-halfA, 0, centers[3].z, -1, 0, 0, uStage2[3], 0};

    drawableSize[2] = vertexIndex - drawableOffset[2];

    vertexCount = vertexIndex;
}

RoundedCube::~RoundedCube()
{
    delete[] vertices;
}

Vertex *RoundedCube::getVertices()
{
    return vertices;
}

int RoundedCube::getVertexCount()
{
    return vertexCount; // 返回计算出的总顶点数
}

int RoundedCube::getDrawableCount()
{
    return 3;
}

int RoundedCube::getDrawableOffset(int index)
{
    return drawableOffset[index];
}

int RoundedCube::getDrawableSize(int index)
{
    return drawableSize[index];
}
