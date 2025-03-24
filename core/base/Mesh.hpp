#pragma once

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual float *getVertices() = 0;

    virtual int getVertexCount() = 0;

    virtual float *getUvs() 
    {
        return nullptr;
    }

    virtual float *getNormals()
    {
        return nullptr;
    }
};