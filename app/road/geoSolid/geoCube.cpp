// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geoCube.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;//几何坐标
    QVector3D norVector;//法向量
   // QVector2D texCoord;//纹理坐标
};

//! [0]
geoCube::geoCube()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    //initializeOpenGLFunctions();

    //// Generate 2 VBOs
    //arrayBuf.create();
    //indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    //initCube();

    //m_ka = QVector3D(0.1,0.1,0.1);//环境光反射系数
    //m_kd = QVector3D(0.5,0.5,0.5);//漫反射反射系数
}

geoCube::~geoCube()
{
    releaseGL();
}

void geoCube::releaseGL()
{
    arrayBuf.destroy();
    indexBuf.destroy();
    boxBuf.destroy();
    normBuf.destroy();
}

void geoCube::initGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    float w = 1.0f;
    float halfw = w/2.0f;
    float third = 1/3.0f;
    float third2 = 2/3.0f;
    QVector3D norm1 = QVector3D(third,third,third);
    norm1.normalize();

    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-halfw, -halfw, halfw), QVector3D(-norm1.x(), -norm1.x(), norm1.x())},  // v0
        {QVector3D( halfw, -halfw, halfw), QVector3D(norm1.x(),  -norm1.x(), norm1.x())}, // v1
        {QVector3D(-halfw,  halfw, halfw), QVector3D(-norm1.x(),  norm1.x(), norm1.x())},  // v2
        {QVector3D( halfw,  halfw, halfw), QVector3D(norm1.x(),   norm1.x(), norm1.x())}, // v3

        // Vertex data for face 1
        {QVector3D( halfw, -halfw,  halfw), QVector3D(norm1.x(), -norm1.x(),  norm1.x())}, // v4
        {QVector3D( halfw, -halfw, -halfw), QVector3D(norm1.x(), -norm1.x(), -norm1.x())}, // v5
        {QVector3D( halfw,  halfw,  halfw), QVector3D(norm1.x(),  norm1.x(),  norm1.x())}, // v6
        {QVector3D( halfw,  halfw, -halfw), QVector3D(norm1.x(),  norm1.x(), -norm1.x())}, // v7

        // Vertex data for face 2
        {QVector3D( halfw, -halfw, -halfw), QVector3D( norm1.x(), -norm1.x(), -norm1.x())}, // v8
        {QVector3D(-halfw, -halfw, -halfw), QVector3D(-norm1.x(), -norm1.x(), -norm1.x())},  // v9
        {QVector3D( halfw,  halfw, -halfw), QVector3D( norm1.x(),  norm1.x(), -norm1.x())}, // v10
        {QVector3D(-halfw,  halfw, -halfw), QVector3D(-norm1.x(),  norm1.x(), -norm1.x())},  // v11

        // Vertex data for face 3
        {QVector3D(-halfw, -halfw, -halfw), QVector3D(-norm1.x(), -norm1.x(), -norm1.x())}, // v12
        {QVector3D(-halfw, -halfw,  halfw), QVector3D(-norm1.x(), -norm1.x(),  norm1.x())},  // v13
        {QVector3D(-halfw,  halfw, -halfw), QVector3D(-norm1.x(),  norm1.x(), -norm1.x())}, // v14
        {QVector3D(-halfw,  halfw,  halfw), QVector3D(-norm1.x(),  norm1.x(),  norm1.x())},  // v15

        // Vertex data for face 4
        {QVector3D(-halfw, -halfw, -halfw), QVector3D(-norm1.x(), -norm1.x(), -norm1.x())}, // v16
        {QVector3D( halfw, -halfw, -halfw), QVector3D( norm1.x(), -norm1.x(), -norm1.x())}, // v17
        {QVector3D(-halfw, -halfw,  halfw), QVector3D(-norm1.x(), -norm1.x(),  norm1.x())}, // v18
        {QVector3D( halfw, -halfw,  halfw), QVector3D( norm1.x(), -norm1.x(),  norm1.x())}, // v19

        // Vertex data for face 5
        {QVector3D(-halfw,  halfw,  halfw), QVector3D(-norm1.x(), norm1.x(),  norm1.x())}, // v20
        {QVector3D( halfw,  halfw,  halfw), QVector3D( norm1.x(), norm1.x(),  norm1.x())}, // v21
        {QVector3D(-halfw,  halfw, -halfw), QVector3D(-norm1.x(), norm1.x(), -norm1.x())}, // v22
        {QVector3D( halfw,  halfw, -halfw), QVector3D( norm1.x(), norm1.x(), -norm1.x())}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]

    QVector3D boxvertices[] = {
    // 底面
    QVector3D(-halfw, -halfw, -halfw),  // v0
    QVector3D(-halfw,  halfw, -halfw), // v3 
    QVector3D( halfw,  halfw, -halfw),  // v2
    QVector3D( halfw, -halfw, -halfw),   // v1
      
    //顶面
    QVector3D(-halfw, -halfw, halfw),  // v4
    QVector3D( halfw, -halfw, halfw), // v5
    QVector3D( halfw,  halfw, halfw),  // v6
    QVector3D(-halfw,  halfw, halfw), // v7

    //左侧面-X方向
    QVector3D(-halfw, -halfw, -halfw), // v0
    QVector3D(-halfw, -halfw,  halfw),  // v4
    QVector3D(-halfw,  halfw,  halfw),  // v7
    QVector3D(-halfw,  halfw, -halfw), // v3

     //右侧面+X方向
    QVector3D( halfw, -halfw, -halfw), // v1
    QVector3D( halfw,  halfw, -halfw),  // v2
    QVector3D( halfw,  halfw,  halfw),  // v6
    QVector3D( halfw, -halfw,  halfw), // v5  

    // 前面-Y方向
    QVector3D(-halfw, -halfw, -halfw),  // v0
    QVector3D( halfw, -halfw, -halfw),   // v1
    QVector3D( halfw, -halfw,  halfw),  // v5
    QVector3D(-halfw, -halfw,  halfw),   // v4

    // 背面--正Y方向
    QVector3D(-halfw,  halfw,  halfw),  // v7
    QVector3D( halfw,  halfw,  halfw), // v6
    QVector3D( halfw,  halfw, -halfw),  // v2
    QVector3D(-halfw,  halfw, -halfw), // v3
    };
    boxBuf.create();
  
    boxBuf.bind();
    boxBuf.allocate(boxvertices, 24 * sizeof(QVector3D));

    float nn = sqrt(3) / 3;
    QVector3D boxverticesN[] = {
        // 底面
        QVector3D(-nn, -nn, -nn),  // v0
        QVector3D(-nn,  nn, -nn), // v3 
        QVector3D( nn,  nn, -nn),  // v2
        QVector3D( nn, -nn, -nn),   // v1

        //顶面
        QVector3D(-nn, -nn, nn),  // v4
        QVector3D(nn, -nn, nn), // v5
        QVector3D(nn,  nn, nn),  // v6
        QVector3D(-nn,  nn, nn), // v7

        //左侧面-X方向
        QVector3D(-nn, -nn, -nn), // v0
        QVector3D(-nn, -nn,  nn),  // v4
        QVector3D(-nn,  nn,  nn),  // v7
        QVector3D(-nn,  nn, -nn), // v3

         //右侧面+X方向
        QVector3D(nn, -nn, -nn), // v1
        QVector3D(nn,  nn, -nn),  // v2
        QVector3D(nn,  nn,  nn),  // v6
        QVector3D(nn, -nn,  nn), // v5  

        // 前面-Y方向
        QVector3D(-nn, -nn, -nn),  // v0
        QVector3D(nn, -nn, -nn),   // v1
        QVector3D(nn, -nn,  nn),  // v5
        QVector3D(-nn, -nn,  nn),   // v4

        // 背面--正Y方向
        QVector3D(-nn,  nn,  nn),  // v7
        QVector3D(nn,  nn,  nn), // v6
        QVector3D(nn,  nn, -nn),  // v2
        QVector3D(-nn,  nn, -nn), // v3
    };

    normBuf.create();
    normBuf.bind();
    normBuf.allocate(boxverticesN, 24 * sizeof(QVector3D));
}

void geoCube::draw(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
  /*  program->setUniformValue("Ka",m_ka);
    program->setUniformValue("Kd",m_kd);*/

   // arrayBuf.bind();
   // indexBuf.bind();
    boxBuf.bind();
    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("VertexPosition");
    program->enableAttributeArray(vertexLocation);
   // program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);
    normBuf.bind();
    // Tell OpenGL programmable pipeline how to locate vertex normal data
    int normalLocation = program->attributeLocation("VertexNormal");
    program->enableAttributeArray(normalLocation);
   // program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
    // Draw cube geometry using indices from VBO 1
    //glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
    glDrawArrays(GL_QUADS, 0, 24);
}

void geoCube::drawBox(QOpenGLShaderProgram* program)
{
    boxBuf.bind();
   // indexBuf.bind();

    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("vVertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    //// Offset for texture coordinate
    //offset += sizeof(QVector3D);

    //// Tell OpenGL programmable pipeline how to locate vertex normal data
    //int normalLocation = program->attributeLocation("VertexNormal");
    //program->enableAttributeArray(normalLocation);
    //program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
    glDrawArrays(GL_QUADS, 0, 24);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
