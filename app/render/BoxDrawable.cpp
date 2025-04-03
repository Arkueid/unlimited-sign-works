#include <render/BoxDrawable.hpp>
#include <core.hpp>
#include <float.h>

namespace
{
    // 242, 241, 12
    // 黄色
    constexpr float R = 242.0f / 255.0f;
    constexpr float G = 241.0f / 255.0f;
    constexpr float B = 12.0f / 255.0f;
}

// 上底面 | 下底面
// 底面：左上角 | 右上角 | 右下角 | 左下角
static Vertex vertices[8] = {
    {-0.5f, 0.5f, -0.5f, R, G, B},
    {0.5f, 0.5f, -0.5f,R, G, B},
    {0.5f, 0.5f, 0.5f, R, G, B},
    {-0.5f, 0.5f, 0.5f, R, G, B},
    {-0.5f, -0.5f, -0.5f, R, G, B},
    {0.5f, -0.5f, -0.5f, R, G, B},
    {0.5f, -0.5f, 0.5f, R, G, B},
    {-0.5f, -0.5f, 0.5f, R, G, B},
};

static unsigned short indices[24] = {
    0, 1, 1, 2, 2, 3, 3, 0, 
    0, 4, 1, 5, 2, 6, 3, 7,
    4, 5, 5, 6, 6, 7, 7, 4
};

BoxDrawable::BoxDrawable() : vertexBuffer(nullptr)
{
    initializeOpenGLFunctions();
    vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertexBuffer->create();
    vertexBuffer->bind();
    vertexBuffer->allocate(vertices, sizeof(vertices));
    vertexBuffer->release();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/box_vert.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/box_frag.glsl");
    program.link();

    modelMatrix.setToIdentity();
}

BoxDrawable::~BoxDrawable()
{
    delete vertexBuffer;
}

void BoxDrawable::wrap(Part *part, const QMatrix4x4& model)
{
    modelMatrix = model;
    const int count = part->getVertexCount();
    float maxX = -FLT_MAX;
    float maxZ = -FLT_MAX;
    float maxY = -FLT_MAX;
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    Vertex* meshVertices = part->getVertices();
    for (int i = 0; i < count; i++) {
        if (meshVertices[i].x > maxX)
        {
            maxX = meshVertices[i].x;
        }
        else if (meshVertices[i].x < minX)
        {
            minX = meshVertices[i].x;
        }

        if (meshVertices[i].z > maxZ)
        {
            maxZ = meshVertices[i].z;
        }
        else if (meshVertices[i].z < minZ)
        {
            minZ = meshVertices[i].z;
        }

        if (meshVertices[i].y > maxY)
        {
            maxY = meshVertices[i].y;
        }
        else if (meshVertices[i].y < minY)
        {
            minY = meshVertices[i].y;
        }
    }

    minX -= 0.2f;
    minZ -= 0.2f;
    minY -= 0.2f;
    maxX += 0.2f;
    maxZ += 0.2f;
    maxY += 0.2f;

    vertices[0].x = minX;
    vertices[0].y = maxY;
    vertices[0].z = minZ;
    vertices[1].x = maxX;
    vertices[1].y = maxY;
    vertices[1].z = minZ;
    vertices[2].x = maxX;
    vertices[2].y = maxY;
    vertices[2].z = maxZ;
    vertices[3].x = minX;
    vertices[3].y = maxY;
    vertices[3].z = maxZ;
    vertices[4].x = minX;
    vertices[4].y = minY;
    vertices[4].z = minZ;
    vertices[5].x = maxX;
    vertices[5].y = minY;
    vertices[5].z = minZ;
    vertices[6].x = maxX;
    vertices[6].y = minY;
    vertices[6].z = maxZ;
    vertices[7].x = minX;
    vertices[7].y = minY;
    vertices[7].z = maxZ;

    vertexBuffer->bind();
    vertexBuffer->write(0, vertices, sizeof(vertices));
    vertexBuffer->release();
}
void BoxDrawable::draw(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    program.bind();
    program.setUniformValue("model", modelMatrix);
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);

    vertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, nx));
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);
    vertexBuffer->release();
    program.release();
}
