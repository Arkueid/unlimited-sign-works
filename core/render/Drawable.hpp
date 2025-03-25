#pragma once

#include <render/Renderer.hpp>

class Drawable
{
protected:
    unsigned int vao;
    unsigned int textureId;
public:
    virtual void draw(Renderer* renderer) = 0;
};