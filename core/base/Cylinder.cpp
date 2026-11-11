#include <base/Cylinder.hpp>
#include <core.hpp>
#include <cmath>
#include <vector>

static std::vector<Cylinder *> instances;

Cylinder::Cylinder(float d, float h, float precision) : d(d), h(h), precision(precision)
{
    const int numOnCircle = static_cast<int>(PI * d * precision);
    indexCount = numOnCircle * 3 * 4;
    pointCount = numOnCircle * 2 + 2;

    unsigned short *indices = new unsigned short[indexCount];
    float *points = new float[pointCount * 3];
    vertices = new Vertex[indexCount];

    // 弧度制
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
    // 上
    for (int i = 0; i < numOnCircle; i++)
    {
        indices[index++] = i;
        indices[index++] = (i + 1) % numOnCircle;
        indices[index++] = upperCenter;
    }
    // 下
    for (int i = 0; i < numOnCircle; i++)
    {
        indices[index++] = i + numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
        indices[index++] = lowerCenter;
    }
    // 侧面
    for (int i = 0; i < numOnCircle; i++)
    {
        // 侧面左下三角
        indices[index++] = i;
        indices[index++] = i + numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
        // 侧面右上三角
        indices[index++] = i;
        indices[index++] = (i + 1) % numOnCircle;
        indices[index++] = (i + 1) % numOnCircle + numOnCircle;
    }
    index = 0;
    // 上
    drawableOffset[0] = index;
    drawableSize[0] = numOnCircle * 3;
    for (int i = 0; i < numOnCircle /* 三角形个数 */; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vertices[index].x = points[indices[index] * 3 + 0];
            vertices[index].y = points[indices[index] * 3 + 1];
            vertices[index].z = points[indices[index] * 3 + 2];
            vertices[index].nx = 0;
            vertices[index].ny = 1;
            vertices[index].nz = 0;
            vertices[index].u = 0.5f + vertices[index].x / r * 0.5f;
            vertices[index].v = 0.5f - vertices[index].z / r * 0.5f;
            index++;
        }
    }
    // 下
    drawableOffset[1] = index;
    drawableSize[1] = numOnCircle * 3;
    for (int i = 0; i < numOnCircle /* 三角形个数 */; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vertices[index].x = points[indices[index] * 3 + 0];
            vertices[index].y = points[indices[index] * 3 + 1];
            vertices[index].z = points[indices[index] * 3 + 2];
            vertices[index].nx = 0;
            vertices[index].ny = -1;
            vertices[index].nz = 0;
            vertices[index].u = 0.5f + vertices[index].x / r * 0.5f;
            vertices[index].v = 0.5f - vertices[index].z / r * 0.5f;
            index++;
        }
    }
    // 侧面
    drawableOffset[2] = index;
    drawableSize[2] = numOnCircle * 6;
    for (int i = 0; i < numOnCircle /* 三角形个数 */; i++)
    {
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)i / (float)numOnCircle;
        vertices[index].v = 1.0f;
        index++;
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)i / (float)numOnCircle;
        vertices[index].v = 0.0f;
        index++;
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)(i + 1) / (float)numOnCircle;
        vertices[index].v = 0.0f;
        index++;
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)i / (float)numOnCircle;
        vertices[index].v = 1.0f;
        index++;
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)(i + 1) / (float)numOnCircle;
        vertices[index].v = 1.0f;
        index++;
        vertices[index].x = points[indices[index] * 3 + 0];
        vertices[index].y = points[indices[index] * 3 + 1];
        vertices[index].z = points[indices[index] * 3 + 2];
        vertices[index].nx = vertices[index].x;
        vertices[index].ny = 0;
        vertices[index].nz = vertices[index].z;
        vertices[index].u = (float)(i + 1) / (float)numOnCircle;
        vertices[index].v = 0.0f;
        index++;
    }
    delete[] indices;
    delete[] points;

    indexCount = index;
}

Cylinder::~Cylinder() // 圆柱体销毁时调用
{
    instances.erase(std::find(instances.begin(), instances.end(), this));
    delete[] vertices;
}

Cylinder *Cylinder::create(float d, float h, float precision) // 创建圆柱体
{ 
    for (int i = 0; i < instances.size(); i++)
    {
        if (instances[i]->d == d && instances[i]->h == h && instances[i]->precision == precision)
        {
            return instances[i];
        }
    }

    Cylinder *cylinder = new Cylinder(d, h, precision);
    instances.push_back(cylinder);
    return cylinder;
}
int Cylinder::getDrawableCount()
{
    return 3; // 上下底面、侧面
}

int Cylinder::getDrawableOffset(int index)
{
    return drawableOffset[index];
}

int Cylinder::getDrawableSize(int index)
{
    return drawableSize[index];
}
