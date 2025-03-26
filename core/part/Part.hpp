#pragma once

#include <base/Mesh.hpp>

#include <vector>
#include <string>


// 代理模式
class Part: public Mesh
{
    Point origin;
    Rotation rotation;
    Mesh *mesh;
    std::vector<int> textureRefs;
    const std::vector<std::string>* texturePaths;

public:
    Part(Mesh *mesh,
         const Point &origin,
         const Rotation &rotation,
         const std::vector<int>& textureRefs, 
         const std::vector<std::string>* texturePaths);

    virtual ~Part() = default;

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

    const std::string& getTexturePath(int index) const;

    // Mesh
    Vertex* getVertices() override;

    int getVertexCount() override;

    int getDrawableCount() override;

    int getDrawableOffset(int index) override;

    int getDrawableSize(int index) override;
};