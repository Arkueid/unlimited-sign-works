#include <base/Cube.hpp>
#include "Cube.hpp"

static unsigned short indices[cubeIndexCount] = {0, 1, 2, 0, 2, 3,
                                          1, 5, 6, 1, 2, 6,
                                          5, 4, 7, 5, 6, 7,
                                          4, 0, 3, 4, 3, 7,
                                          0, 1, 5, 0, 4, 5,
                                          3, 2, 6, 3, 7, 6};

static float normals[cubeIndexCount * 3] = {
    // x, y, z
    //  0, 1, 2, 0, 2, 3,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    //  1, 5, 6, 1, 2, 6,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    //  5, 4, 7, 5, 6, 7,
    0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
    //  4, 0, 3, 4, 3, 7,
    -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
    //  0, 1, 5, 0, 4, 5,
    0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
    //  3, 2, 6, 3, 7, 6
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};

Cube::Cube(float a, float b, float c)
{
    float points[8 * 3];

    const float x = -a / 2, z = b / 2, y = -c / 2;

    points[0] = x;
    points[1] = y;
    points[2] = z;
    points[3] = -x;
    points[4] = y;
    points[5] = z;
    points[6] = -x;
    points[7] = -y;
    points[8] = z;
    points[9] = x;
    points[10] = -y;
    points[11] = z;
    points[12] = x;
    points[13] = y;
    points[14] = -z;
    points[15] = -x;
    points[16] = y;
    points[17] = -z;
    points[18] = -x;
    points[19] = -y;
    points[20] = -z;
    points[21] = x;
    points[22] = -y;
    points[23] = -z;

    int index = 0;

    for (int i = 0; i < cubeIndexCount; i++)
    {
        vertices[index++] = points[indices[i] * 3 + 0];
        vertices[index++] = points[indices[i] * 3 + 1];
        vertices[index++] = points[indices[i] * 3 + 2];
    }
}

float *Cube::getNormals()
{
    return normals;
}
