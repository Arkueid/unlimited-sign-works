#include <base/Cylinder.hpp>
#include <core.hpp>
#include <cmath>

Cylinder::Cylinder(float d, float h, float precision) : d(d), h(h)
{
    const int numOnCircle = static_cast<int>(PI * d * precision);
    indexCount = numOnCircle * 3 * 4;
    pointCount = numOnCircle * 2 + 2;

    unsigned short *indices = new unsigned short[indexCount];
    float *points = new float[pointCount * 3];
    vertices = new Vertex[indexCount];

    const float angleStep = 2 * PI / numOnCircle;
    const int upperCenter = pointCount - 2;
    const int lowerCenter = pointCount - 1;
    const float r = d / 2;
    // 上圆顶点|底圆顶点|上圆心|下圆心|
    for (int i = 0; i < pointCount - 2; i++)
    {
        const float angle = i * angleStep;
        points[i * 3] = r * cos(angle);
        if (i < numOnCircle)
        {
            points[i * 3 + 1] = h;
        }
        else
        {
            points[i * 3 + 1] = 0;
        }
        points[i * 3 + 2] = r * sin(angle);
    }
    points[upperCenter * 3 + 0] = 0;
    points[upperCenter * 3 + 1] = h;
    points[upperCenter * 3 + 2] = 0;
    points[lowerCenter * 3 + 0] = 0;
    points[lowerCenter * 3 + 1] = 0;
    points[lowerCenter * 3 + 2] = 0;

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

    index = 0;
    for (int i = 0; i < numOnCircle; i++)
    {
        // 上
        for (int j = 0; j < 3; j++)
        {
            vertices[index].x = points[indices[index] * 3 + 0];
            vertices[index].y = points[indices[index] * 3 + 1];
            vertices[index].z = points[indices[index] * 3 + 2];
            vertices[index].nx = 0;
            vertices[index].ny = 1;
            vertices[index].nz = 0;
            index++;
        }

        // 下
        for (int j = 0; j < 3; j++)
        {
            vertices[index].x = points[indices[index] * 3 + 0];
            vertices[index].y = points[indices[index] * 3 + 1];
            vertices[index].z = points[indices[index] * 3 + 2];
            vertices[index].nx = 0;
            vertices[index].ny = -1;
            vertices[index].nz = 0;
            index++;
        }

        // 侧面1，侧面2
        for (int j = 0; j < 6; j++)
        {
            vertices[index].x = points[indices[index] * 3 + 0];
            vertices[index].y = points[indices[index] * 3 + 1];
            vertices[index].z = points[indices[index] * 3 + 2];
            vertices[index].nx = vertices[index].x;
            vertices[index].ny = 0;
            vertices[index].nz = vertices[index].z;
            index++;
        }
    }
    delete[] indices;
    delete[] points;
}

Cylinder::~Cylinder()
{
    delete[] vertices;
}