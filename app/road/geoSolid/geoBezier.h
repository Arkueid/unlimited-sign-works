// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOBEZIER_H
#define GEOBEZIER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class geoBezier : protected QOpenGLFunctions
{
public:
    geoBezier();
    virtual ~geoBezier();

    void draw(QOpenGLShaderProgram *program);
    void drawWithTexture(QOpenGLShaderProgram* program);
    void drawCurve(QOpenGLShaderProgram* program);
    void drawCtrlPts(QOpenGLShaderProgram* program);
private:
    void initBezier();

    //曲面
    QOpenGLBuffer coordBuf;
    QOpenGLBuffer normaBuf;
    QOpenGLBuffer textuBuf;
    //控制顶点
    QOpenGLBuffer ctrlPtBuf;
    //曲线
    QOpenGLBuffer curveBuf;

    QVector3D m_ka;//环境光反射系数
    QVector3D m_kd;//漫反射反射系数

    int m_vertsize;
};

#endif // GEOBEZIER_H
