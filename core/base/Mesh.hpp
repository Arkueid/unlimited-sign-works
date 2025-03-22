#pragma once

class Mesh {
protected:
public:
    virtual ~Mesh() = default;
    virtual float* getVertices() = 0;
    virtual unsigned short* getIndices() = 0;
    virtual int getVertexCount() = 0;
    virtual int getIndexCount() = 0;
};