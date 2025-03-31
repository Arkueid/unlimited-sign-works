#include <render/AxisDrawable.hpp>
#include <utils/VertexBufferManager.hpp>
#include <stddef.h>

static Vertex vertices[] = {
    // position, color
    // x轴
    {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {100000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    // y轴
    {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 100000.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    // z轴
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 100000.0f, 0.0f, 0.0f, 1.0f},
};

AxisDrawable::AxisDrawable(): vertexBuffer(nullptr)
{
	initializeOpenGLFunctions();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/axis_vert.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/axis_frag.glsl");
    program.link();

    vertexBuffer = VertexBufferManager::registerVertexBuffer(vertices, 6);
}

AxisDrawable::~AxisDrawable()
{
    VertexBufferManager::unregisterVertexBuffer(vertices);
}

QOpenGLShaderProgram* AxisDrawable::getProgram()
{
    return &program;
}

void AxisDrawable::draw(QMatrix4x4& projection, QMatrix4x4& view)
{
    program.bind();
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);
    vertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, nx));
    glDrawArrays(GL_LINES, 0, 6);
    vertexBuffer->release();
    program.release();
}
