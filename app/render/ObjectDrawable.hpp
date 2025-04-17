#pragma once

#include <QOpenGLFunctions>
#include <QVector>

#include <render/PartDrawable.hpp>

class ObjectDrawable : protected QOpenGLFunctions
{
    QVector<PartDrawable *> partDrawables;

public:
    ObjectDrawable();
    ~ObjectDrawable();
};