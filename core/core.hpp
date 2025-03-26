#pragma once

#define PI 3.1415926f
#define PRECISION_DEFAULT 1.0f / 20.0f

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
