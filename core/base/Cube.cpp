#include <base/Cube.hpp>

static unsigned short indices[cubeVertexCount] = {0, 1, 2, 0, 2, 3,
                                          1, 5, 6, 1, 2, 6,
                                          5, 4, 7, 5, 6, 7,
                                          4, 0, 3, 4, 3, 7,
                                          0, 1, 5, 0, 4, 5,
                                          3, 2, 6, 3, 7, 6};

Cube::Cube(float a, float b, float c): a(a), b(b), c(c)
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

    for (int i = 0; i < cubeVertexCount; i++)
    {
        vertices[i].x = points[indices[i] * 3 + 0];
        vertices[i].y = points[indices[i] * 3 + 1];
        vertices[i].z = points[indices[i] * 3 + 2];
    }

    // front
    for (int i = 0; i < 6; i++)
    {
        vertices[i].nx = 0.0f;
        vertices[i].ny = 0.0f;
        vertices[i].nz = 1.0f;
    }

    // right
    for (int i = 6; i < 12; i++)
    {
        vertices[i].nx = 1.0f;
        vertices[i].ny = 0.0f;
        vertices[i].nz = 0.0f;
    }

    // back
    for (int i = 12; i < 18; i++)
    {
        vertices[i].nx = 0.0f;
        vertices[i].ny = 0.0f;
        vertices[i].nz = -1.0f;
    }

    // left
    for (int i = 18; i < 24; i++)
    {
        vertices[i].nx = -1.0f;
        vertices[i].ny = 0.0f;
        vertices[i].nz = 0.0f;
    }

    // bottom
    for (int i = 24; i < 30; i++)
    {
        vertices[i].nx = 0.0f;
        vertices[i].ny = -1.0f;
        vertices[i].nz = 0.0f;
    }

    // top
    for (int i = 30; i < 36; i++)
    {
        vertices[i].nx = 0.0f;
        vertices[i].ny = 1.0f;
        vertices[i].nz = 0.0f;
    }
}
