// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOCUBE_H
#define GEOCUBE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class geoCube : protected QOpenGLFunctions
{
public:
    geoCube();
    virtual ~geoCube();

    void releaseGL();
    void draw(QOpenGLShaderProgram *program);
    void drawBox(QOpenGLShaderProgram* program);
    void initGeometry();
private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer boxBuf;
    QOpenGLBuffer normBuf;
};

#endif // GEOCUBE_H
