#include <render/Drawable.hpp>

Drawable::Drawable(Mesh *mesh, QMatrix4x4 *modelMatrix) : mesh(mesh),
                                                          vertexBuffer(QOpenGLBuffer::VertexBuffer),
                                                          modelMatrix(modelMatrix)
{
    initializeOpenGLFunctions();

    vertexBuffer.create();

    vertexBuffer.bind();
    vertexBuffer.allocate(mesh->getVertices(), mesh->getVertexCount() * 8 * sizeof(float));
    
    vertexBuffer.release();
}

void Drawable::draw(QOpenGLShaderProgram *program)
{
    QMatrix4x4 scale;
    scale.setToIdentity();
    scale.scale(0.001f);
    scale = scale * *modelMatrix;
    program->setUniformValue("model", scale);
    program->setUniformValue("normalMatrix", scale.normalMatrix());
    vertexBuffer.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void *)(3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
    vertexBuffer.release();
}