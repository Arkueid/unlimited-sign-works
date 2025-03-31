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

    virtual ~Part() = default;

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

    Mesh *getMesh() const;

    const std::vector<int> &getTextureRefs() const
    {
        return textureRefs;
    }
};