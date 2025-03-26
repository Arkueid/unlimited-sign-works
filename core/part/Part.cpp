#include "Part.hpp"

Part::Part(Mesh *mesh, const Point& origin, const Rotation& rotation, const std::vector<int>& textureRefs, const std::vector<std::string>* texturePaths): 
    mesh(mesh), origin(origin), rotation(rotation), textureRefs(textureRefs), texturePaths(texturePaths)
{
}

const std::string& Part::getTexturePath(int index) const
{
    return (*texturePaths)[textureRefs[index]];
}

Vertex* Part::getVertices()
{
    return mesh->getVertices();
}

int Part::getVertexCount()
{
    return mesh->getVertexCount();
}

int Part::getDrawableCount()
{
    return mesh->getDrawableCount();
}

int Part::getDrawableOffset(int index)
{
    return mesh->getDrawableOffset(index);
}

int Part::getDrawableSize(int index)
{
    return mesh->getDrawableSize(index);
}
