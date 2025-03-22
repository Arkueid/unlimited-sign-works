#include <render/Drawable.hpp>
#include "Drawable.hpp"

Drawable::Drawable(Mesh *mesh, QMatrix4x4 *modelMatrix) : mesh(mesh),
                                                          vbo(QOpenGLBuffer::VertexBuffer),
                                                          modelMatrix(modelMatrix)
{
    initializeOpenGLFunctions();

    vbo.create();

    vbo.bind();
    vbo.allocate(mesh->getVertices(), mesh->getVertexCount() * 3 * sizeof(float));
    vbo.release();
}

void Drawable::draw(QOpenGLShaderProgram *program)
{
    QMatrix4x4 scale;
    scale.setToIdentity();
    scale.scale(0.001f);
    scale = scale * *modelMatrix;
    program->setUniformValue("model", scale);
    vbo.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void *)0);
    glDrawElements(GL_LINES, mesh->getIndexCount(), GL_UNSIGNED_SHORT, mesh->getIndices());
    vbo.release();
}
