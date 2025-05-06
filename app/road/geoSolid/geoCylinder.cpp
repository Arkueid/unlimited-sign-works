// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geoCylinder.h"

#include <QVector2D>
#include <QVector3D>

geoCylinder::geoCylinder()    
{
    ////initializeOpenGLFunctions();

    //// Generate 2 VBOs
    //coordBuf.create();    
    //normaBuf.create();
    // Initializes cube geometry and transfers it to VBOs
    
    //m_ka = QVector3D(0.3,0.3,0.3);//环境光反射系数
    //m_kd = QVector3D(0.5,0.5,0.5);//漫反射反射系数
}

geoCylinder::~geoCylinder()
{
    
}

void geoCylinder::releaseGL()
{
    coordBuf.destroy();
    normaBuf.destroy();
}

void geoCylinder::initGeometry()
{
    float r = 1.0f;
    float h = r;
    int m = 40;
    float dalpha = 2 * 3.14159 / m;
    float alpha = 0;
    float bottomZ = 0;
    float topZ = r;

    QVector3D* pVerts = new QVector3D[3 * m * 2 + 2 * m*3];
    QVector3D* pNormals = new QVector3D[3 * m * 2 + 2 * m*3];

    int indexV = 0;
    int indexN = 0;
    //侧面
    for (int i = 0; i < m; i++)
    {
        float x1 = r * cos(alpha);
        float y1 = r * sin(alpha);

        float alpha1 = alpha + dalpha;
        float x2 = r * cos(alpha1);
        float y2 = r * sin(alpha1);

        pVerts[indexV++] = QVector3D(x1, y1, bottomZ);
        pVerts[indexV++] = QVector3D(x2, y2, bottomZ);
        pVerts[indexV++] = QVector3D(x2, y2, topZ);
        pNormals[indexN++] = QVector3D(x1, y1, 0) / r;
        pNormals[indexN++] = QVector3D(x2, y2, 0) / r;
        pNormals[indexN++] = QVector3D(x2, y2, 0) / r;

        pVerts[indexV++] = QVector3D(x1, y1, topZ);
        pVerts[indexV++] = QVector3D(x1, y1, bottomZ);
        pVerts[indexV++] = QVector3D(x2, y2, topZ);
        pNormals[indexN++] = QVector3D(x1, y1, 0) / r;
        pNormals[indexN++] = QVector3D(x1, y1, 0) / r;
        pNormals[indexN++] = QVector3D(x2, y2, 0) / r;   

        alpha = alpha1;
    }

    //底面
    alpha = 0;
    for (int i = 0; i < m; i++)
    {
        float x1 = r * cos(alpha);
        float y1 = r * sin(alpha);

        float alpha1 = alpha + dalpha;
        float x2 = r * cos(alpha1);
        float y2 = r * sin(alpha1);

        pVerts[indexV++] = QVector3D(x2, y2, bottomZ);
        pVerts[indexV++] = QVector3D(x1, y1, bottomZ);        
        pVerts[indexV++] = QVector3D(0, 0, bottomZ);
        pNormals[indexN++] = QVector3D(0, 0, -1);
        pNormals[indexN++] = QVector3D(0, 0, -1);
        pNormals[indexN++] = QVector3D(0, 0, -1);

        alpha = alpha1;
    }
    //顶面
    alpha = 0;
    for (int i = 0; i < m; i++)
    {
        float x1 = r * cos(alpha);
        float y1 = r * sin(alpha);

        float alpha1 = alpha + dalpha;
        float x2 = r * cos(alpha1);
        float y2 = r * sin(alpha1);

        pVerts[indexV++] = QVector3D(x1, y1, topZ);
        pVerts[indexV++] = QVector3D(x2, y2, topZ);
        pVerts[indexV++] = QVector3D(0, 0, topZ);
        pNormals[indexN++] = QVector3D(0, 0, 1);
        pNormals[indexN++] = QVector3D(0, 0, 1);
        pNormals[indexN++] = QVector3D(0, 0, 1);

        alpha = alpha1;
    }

    m_vertsize = 3 * m * 2 + 2 * m * 3;    
    
    initializeOpenGLFunctions();

    bool re = coordBuf.create();
    normaBuf.create();

    //Transfer vertex coord to VBO    
    coordBuf.bind();
    coordBuf.allocate(pVerts, m_vertsize * sizeof(QVector3D));

    //Transfer vertex normal to VBO
    normaBuf.bind();
    normaBuf.allocate(pNormals, m_vertsize * sizeof(QVector3D));

    delete[] pVerts;
    delete[] pNormals;
}

void geoCylinder::render(QOpenGLShaderProgram *program)
{
    //// Tell OpenGL which VBOs to use
    //program->setUniformValue("Ka",m_ka);
    //program->setUniformValue("Kd",m_kd);

    coordBuf.bind();
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("VertexPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Tell OpenGL programmable pipeline how to locate vertex normal data
    normaBuf.bind();
    int normalLocation = program->attributeLocation("VertexNormal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Draw cube geometry using indices from VBO 1
    glDrawArrays(GL_TRIANGLES, 0, m_vertsize);
}
