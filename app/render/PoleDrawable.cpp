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
    this->modelMatrix = modelMatrix;
}
