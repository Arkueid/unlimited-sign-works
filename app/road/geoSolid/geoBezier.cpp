// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geoBezier.h"

#include <QVector2D>
#include <QVector3D>

geoBezier::geoBezier()
{
    initializeOpenGLFunctions();

    //曲面
    coordBuf.create();    
    normaBuf.create();
    textuBuf.create();

    //控制顶点
    ctrlPtBuf.create();
    //曲线
    curveBuf.create();
    // Initializes cube geometry and transfers it to VBOs
    initBezier();

    m_ka = QVector3D(0.3,0.3,0.3);//环境光反射系数
    m_kd = QVector3D(0.5,0.5,0.5);//漫反射反射系数
}

geoBezier::~geoBezier()
{
    coordBuf.destroy();
    normaBuf.destroy();
    textuBuf.destroy();

    ctrlPtBuf.destroy();
    curveBuf.destroy();
}

void geoBezier::initBezier()
{
    QVector3D CtrlPts[] = {{0,0,0},{4,6,0},{8,8,0},{10,2,0}};

    int num = 10;
    float dt = 1.0f / num;

    QVector3D* pBezierVerts = new QVector3D[num + 1];
    float t = 0.0f;
    float coef[4];
    for (int i = 0; i <= num; i++)
    {
        float t2 = t * t;
        float t3 = t2 * t;
        coef[0] = 1 - 3 * t + 3 * t2 - t3;
        coef[1] = 3 * t - 6 * t2 + 3 * t3;
        coef[2] = 3 * t2 - 3 * t3;
        coef[3] = t3;

        QVector3D pt = { 0,0,0 };
        for (int j = 0; j < 4; j++)
            pt += coef[j] * CtrlPts[j];

        pBezierVerts[i] = pt;

        t += dt;
    }

    QVector3D* pBezierAllVerts = new QVector3D[num + 1+4];
   /* ctrlPtBuf.bind();
    ctrlPtBuf.allocate(CtrlPts, 4 * sizeof(QVector3D));*/
    for (int i = 0; i < 4; i++)pBezierAllVerts[i] = CtrlPts[i];
    for (int i = 0; i < num + 1; i++)pBezierAllVerts[i+4] = pBezierVerts[i];
    curveBuf.bind();
    curveBuf.allocate(pBezierAllVerts, (num + 1 + 4) * sizeof(QVector3D));
    delete[] pBezierAllVerts;

    int indexV = 0;
    int indexN = 0;
    float *pCtrlPtsAlpha = new float[num+1];
    for (int i = 0; i < num; i++)
    {
        pCtrlPtsAlpha[i] = atan((pBezierVerts[i + 1].y() - pBezierVerts[i].y()) /
            (pBezierVerts[i + 1].x() - pBezierVerts[i].x()));
        pCtrlPtsAlpha[i] = pCtrlPtsAlpha[i] / 3.14159 * 180;//转换为角度
    }
    pCtrlPtsAlpha[num] = pCtrlPtsAlpha[num-1];

    //YZ坐标轴半圆
    int n = 20;
    float r = 2;
    float dbeta = 3.14259 / n;
    float beta = 0;
    QVector3D* pCircleVerts = new QVector3D[n + 1];
    for (int i = 0; i <= n; i++)
    {
        float y = r * cos(beta);
        float z = r * sin(beta);

        pCircleVerts[i] = QVector3D(0, y, z);

        beta += dbeta;
    }
    
   
    //生成曲面
    QVector3D* pSurfaceVertPos = new QVector3D[n * num * 4];
    QVector3D* pSurfaceVertNormal = new QVector3D[n * num * 4];
    QVector2D* pSurfaceVertTexture = new QVector2D[n * num * 4];

    QVector3D* pCircleVertsI1 = new QVector3D[n + 1];
    QVector3D* pCircleVertsI2 = new QVector3D[n + 1];

    int SurfaceVertPosIndex = 0;
    int SurfaceVertNormalIndex = 0;
    int SurfaceVertTextureIndex = 0;

    for (int i = 0; i < num; i++)
    {
        float alpha1 = pCtrlPtsAlpha[i];
        float alpha2 = pCtrlPtsAlpha[i+1];
        QVector3D pos1 = pBezierVerts[i];
        QVector3D pos2 = pBezierVerts[i+1];

        float s1 = i / float(num);
        float s2 = (i+1) / float(num);

        QMatrix4x4 rotatMatrix1;
        rotatMatrix1.rotate(alpha1, QVector3D(0, 0, 1));        
        QMatrix4x4 rotatMatrix2;
        rotatMatrix2.rotate(alpha2, QVector3D(0, 0, 1));

        QMatrix4x4 transMatrix1;
        transMatrix1.translate(pos1);
        QMatrix4x4 transMatrix2;
        transMatrix2.translate(pos2);

        QMatrix4x4 m1 = transMatrix1 * rotatMatrix1;
        QMatrix4x4 m2 = transMatrix2 * rotatMatrix2;

        QVector4D aa;
        QVector4D bb;
        //前后断面点
        for (int j = 0; j <= n; j++)
        {
            aa = QVector4D(pCircleVerts[j], 1);

            bb = m1 * aa;// pCircleVerts[j];
            pCircleVertsI1[j] = QVector3D(bb.x(), bb.y(), bb.z());
            bb = m2 * aa;// pCircleVerts[j];
            pCircleVertsI2[j] = QVector3D(bb.x(), bb.y(), bb.z());
        }

        //前后断面点连接
        for (int j = 0; j < n; j++)
        {
            //顶点几何坐标
            pSurfaceVertPos[SurfaceVertPosIndex++] = pCircleVertsI1[j];
            pSurfaceVertPos[SurfaceVertPosIndex++] = pCircleVertsI1[j + 1];
            pSurfaceVertPos[SurfaceVertPosIndex++] = pCircleVertsI2[j + 1];
            pSurfaceVertPos[SurfaceVertPosIndex++] = pCircleVertsI2[j];
            //顶点法向量
            QVector3D v = pos1 - pCircleVertsI1[j];
            v.normalize();
            pSurfaceVertNormal[SurfaceVertNormalIndex++] = v;
            v = pos1 - pCircleVertsI1[j + 1];
            v.normalize();
            pSurfaceVertNormal[SurfaceVertNormalIndex++] = v;
            v = pos2 - pCircleVertsI2[j + 1];
            v.normalize();
            pSurfaceVertNormal[SurfaceVertNormalIndex++] = v;
            v = pos2 - pCircleVertsI2[j ];
            v.normalize();
            pSurfaceVertNormal[SurfaceVertNormalIndex++] = v;

            //顶点纹理坐标
            float t1 = j / float(n);
            float t2 = (j + 1) / float(n);
            pSurfaceVertTexture[SurfaceVertTextureIndex++] = QVector2D(s1, t1);
            pSurfaceVertTexture[SurfaceVertTextureIndex++] = QVector2D(s1, t2);
            pSurfaceVertTexture[SurfaceVertTextureIndex++] = QVector2D(s2, t2);
            pSurfaceVertTexture[SurfaceVertTextureIndex++] = QVector2D(s2, t1);
        }
    }

    m_vertsize = n * num * 4;
    //Transfer vertex coord to VBO
    coordBuf.bind();
    coordBuf.allocate(pSurfaceVertPos, m_vertsize * sizeof(QVector3D));

    normaBuf.bind();
    normaBuf.allocate(pSurfaceVertNormal, m_vertsize * sizeof(QVector3D));
        
    textuBuf.bind();
    textuBuf.allocate(pSurfaceVertTexture, m_vertsize * sizeof(QVector2D));

    delete[] pBezierVerts;
    delete[] pCtrlPtsAlpha;
    delete[] pCircleVerts;

    delete[] pSurfaceVertPos;
    delete[] pSurfaceVertNormal;
    delete[] pSurfaceVertTexture;
    
    delete[] pCircleVertsI1 ;
    delete[] pCircleVertsI2;
}

void geoBezier::drawCtrlPts(QOpenGLShaderProgram* program)
{
    int vertexLocation = program->attributeLocation("VertexPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDisable(GL_DEPTH_TEST);

    glPointSize(6);
    program->setUniformValue("color", QVector3D(1, 0, 0));   
    ctrlPtBuf.bind();
    glDrawArrays(GL_POINTS, 0, 4);
  
    glEnable(GL_DEPTH_TEST);
}

void geoBezier::drawCurve(QOpenGLShaderProgram* program)
{
       // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("VertexPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
    
    glDisable(GL_DEPTH_TEST);
    program->setUniformValue("color", QVector3D(0, 0, 0));
    curveBuf.bind();

    glPointSize(4);
    glDrawArrays(GL_POINTS, 0, 4);
    glLineWidth(1);
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    glLineWidth(2);
    glDrawArrays(GL_LINE_STRIP, 4, 11);
   /* glPointSize(6);
    program->setUniformValue("color", QVector3D(1,0,0));

    vertexLocation = program->attributeLocation("VertexPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    ctrlPtBuf.bind();
    glDrawArrays(GL_POINTS, 0, 4);*/
   
    glEnable(GL_DEPTH_TEST);
}

void geoBezier::draw(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    program->setUniformValue("Ka",m_ka);
    program->setUniformValue("Kd",m_kd);

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
    glDrawArrays(GL_QUADS, 0, 80);// m_vertsize);
}

void geoBezier::drawWithTexture(QOpenGLShaderProgram* program)
{
    // Tell OpenGL which VBOs to use
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

    textuBuf.bind();
    int texcoordLocation = program->attributeLocation("VertexTexCoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));

    // Draw cube geometry using indices from VBO 1
    glDrawArrays(GL_QUADS, 0, m_vertsize);
}
