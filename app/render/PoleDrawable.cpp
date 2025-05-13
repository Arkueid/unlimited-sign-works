#include "PoleDrawable.hpp"

PoleDrawable::PoleDrawable(Pole* pole): pole(pole)
{
    initializeOpenGLFunctions();

    const std::vector<Part *> &parts = pole->getParts();
    for (auto part : parts)
    {
        PartDrawable *drawable = new PartDrawable(part);
        drawables.append(drawable);
    }

    modelMatrix.setToIdentity();

    innerScale.setToIdentity();
    innerScale.scale(10 * 1.2); // 单位m
}

PoleDrawable::~PoleDrawable()
{
    qDeleteAll(drawables);
    drawables.clear();
    delete pole;
}

void PoleDrawable::draw(QOpenGLShaderProgram &program)
{
    for (auto d : drawables)
    {
        if (d->isVisible())
        {
            d->draw(program, modelMatrix);
        }
    }
}

void PoleDrawable::setModelMatrix(QMatrix4x4 &modelMatrix)
{
    this->modelMatrix = modelMatrix * innerScale;
}

void PoleDrawable::setPole(Pole *pole)
{
    if (this->pole != pole)
    {
        for (auto d : drawables)
        {
            d->releaseOldBuffer();
            delete d;
        }
        drawables.clear();

        for (auto& part : pole->getParts())
        {
            PartDrawable* p = new PartDrawable(part);
            drawables.push_back(p);
        }

        delete this->pole;
        this->pole = pole;
    }
    else
    {
        for (auto d : drawables)
        {
            d->releaseOldBuffer();
            d->updatePart();
        }
    }
}
