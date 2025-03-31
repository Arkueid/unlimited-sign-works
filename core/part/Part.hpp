#pragma once

#include <base/Mesh.hpp>

#include <vector>
#include <string>

// 代理模式
// part
class Part : public Mesh
{
    Point origin;
    Rotation rotation;
    Mesh *mesh;
    std::vector<int> textureRefs;

    std::string name;

public:
    Part(Mesh *mesh,
         const std::string &name,
         const Point &origin,
         const Rotation &rotation,
         const std::vector<int> &textureRefs);

    virtual ~Part();

    std::string getName()
    {
        return name;
    }

    float getOriginX() const
    {
        return origin.x;
    }
    float getOriginY() const
    {
        return origin.y;
    }
    float getOriginZ() const
    {
        return origin.z;
    }
    float getRotationX() const
    {
        return rotation.rx;
    }
    float getRotationY() const
    {
        return rotation.ry;
    }
    float getRotationZ() const
    {
        return rotation.rz;
    }

    // Mesh
    Vertex *getVertices() override;

    int getVertexCount() override;

    int getDrawableCount() override;

    int getDrawableOffset(int index) override;

    int getDrawableSize(int index) override;

    int getType() override;

    void setMesh(Mesh *value)
    {
        mesh = value;
    }

    Mesh *getMesh() const;

    const std::vector<int> &getTextureRefs() const
    {
        return textureRefs;
    }

    void setName(const std::string &value)
    {
        this->name = value;
    }

    void setRotationX(float value)
    {
        this->rotation.rx = value;
    }

    void setRotationY(float value)
    {
        this->rotation.ry = value;
    }

    void setRotationZ(float value)
    {
        this->rotation.rz = value;
    }

    void setOriginX(float value)
    {
        this->origin.x = value;
    }

    void setOriginY(float value)
    {
        this->origin.y = value;
    }

    void setOriginZ(float value)
    {
        this->origin.z = value;
    }
};