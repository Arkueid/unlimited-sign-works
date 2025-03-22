#include <base/Cube.hpp>
#include "Cube.hpp"

static unsigned short indices[cubeIndexCount] = {
    0, 1, 2, 0, 2, 3,
    1, 5, 6, 1, 2, 6,
    5, 4, 7, 5, 6, 7,
    4, 0, 3, 4, 3, 7,
    0, 1, 5, 0, 4, 5,
    3, 2, 6, 3, 7, 6};

Cube::Cube(float a, float b, float c)
{
    const float x = -a / 2, z = b / 2, y = -c / 2;
    vertices[0] = x;
    vertices[1] = y;
    vertices[2] = z;

    vertices[3] = -x;
    vertices[4] = y;
    vertices[5] = z;

    vertices[6] = -x;
    vertices[7] = -y;
    vertices[8] = z;

    vertices[9] = x;
    vertices[10] = -y;
    vertices[11] = z;

    vertices[12] = x;
    vertices[13] = y;
    vertices[14] = -z;

    vertices[15] = -x;
    vertices[16] = y;
    vertices[17] = -z;

    vertices[18] = -x;
    vertices[19] = -y;
    vertices[20] = -z;

    vertices[21] = x;
    vertices[22] = -y;
    vertices[23] = -z;
}

unsigned short *Cube::getIndices()
{
    return indices;
}
