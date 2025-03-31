#include "Part.hpp"

Part::Part(Mesh *mesh,
           const std::string &name,
           const Point &origin,
           const Rotation &rotation,
           const std::vector<int> &textureRefs) : name(name),
                                                           mesh(mesh),
                                                           origin(origin),
                                                           rotation(rotation),
                                                           textureRefs(textureRefs)
{
    mesh->incRef();
}

Part::~Part()
{
    // printf("Part::~Part()");
    mesh->decRef();
    if (mesh->getRefCount() == 0)
    {
        delete mesh;
    }
}

Vertex *Part::getVertices()
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

int Part::getType()
{
    return mesh->getType();
}

Mesh *Part::getMesh() const
{
    return mesh;
}
