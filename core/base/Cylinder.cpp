#include <base/Cylinder.hpp>
#include <core.hpp>
#include <cmath>

Cylinder::Cylinder(float d, float h, float precision) : d(d), h(h)
{
    const int numOnCircle = static_cast<int>(PI * d * precision);
    indexCount = numOnCircle * 3 * 4;
    vertexCount = numOnCircle * 2 + 2;

    indices = new unsigned short[indexCount];
    vertices = new float[vertexCount * 3];

    const float angleStep = 2 * PI / numOnCircle;
    const int upperCenter = vertexCount - 2;
    const int lowerCenter = vertexCount - 1;
    const float r = d / 2;
    // 上圆顶点|底圆顶点|上圆心|下圆心|
    for (int i = 0; i < vertexCount - 2; i++)
    {
        const float angle = i * angleStep;
        vertices[i * 3] = r * cos(angle);
        if (i < numOnCircle)
        {
            vertices[i * 3 + 1] = h;
        }
        else
        {
            vertices[i * 3 + 1] = 0;
        }
        vertices[i * 3 + 2] = r * sin(angle);
    }
    vertices[vertexCount * 3 - 6] = 0;
    vertices[vertexCount * 3 - 5] = 0;
    vertices[vertexCount * 3 - 4] = 0;
    vertices[vertexCount * 3 - 3] = 0;
    vertices[vertexCount * 3 - 2] = h;
    vertices[vertexCount * 3 - 1] = 0;

    int index = 0;
    for (int i = 0; i < numOnCircle; i++)
    {
        // 上
        indices[index++] = i;
        indices[index++] = (i + 1) % numOnCircle;
        indices[index++] = upperCenter;
        // 下
        indices[index++] = i + numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
        indices[index++] = lowerCenter;
        // 侧面1
        indices[index++] = i;
        indices[index++] = i + numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
        // 侧面2
        indices[index++] = i;
        indices[index++] = (i + 1) % numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
    }
}

Cylinder::~Cylinder()
{
    delete[] indices;
    delete[] vertices;
}