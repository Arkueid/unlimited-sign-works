#pragma once

#define PI 3.1415926f
// 每单位长度的顶点个数
#define PRECISION_DEFAULT 1.0f / 40.0f

struct Vertex
{
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float u;
    float v;
};

struct Point
{
    float x;
    float y;
    float z;
};

struct Rotation
{
    float rx;
    float ry;
    float rz;
};
