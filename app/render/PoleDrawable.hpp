#pragma once

#include <QOpenGLFunctions>
#include <QVector>
#include <QOpenGLShaderProgram>

#include <pole/Pole.hpp>
#include <render/PartDrawable.hpp>

class PoleDrawable : protected QOpenGLFunctions
{
    QVector<PartDrawable *> drawables;

    Pole* pole;

    QMatrix4x4 modelMatrix;

    bool visible;

public:
    PoleDrawable(Pole* pole);
    ~PoleDrawable();

    void draw(QOpenGLShaderProgram &program);

    void setModelMatrix(QMatrix4x4& modelMatrix);

    void setVisible(bool visible)
    {
        this->visible = visible;
    }

    bool isVisible()
    {
        return visible;
    }
};