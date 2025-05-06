// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOCYLINDER_H
#define GEOCYLINDER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class geoCylinder : protected QOpenGLFunctions
{
public:
    geoCylinder();
    virtual ~geoCylinder();

    void releaseGL();
    void initGeometry();
    void render(QOpenGLShaderProgram *program);
private:
    QOpenGLBuffer coordBuf;
    QOpenGLBuffer normaBuf;

    int m_vertsize;
};

#endif // GEOCYLINDER_H
