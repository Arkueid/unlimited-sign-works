#include <part/Part.hpp>

Part::Part(Mesh *mesh,
           const std::string &name,
           const Point &origin,
           const Rotation &rotation,
           const std::vector<int> &textureRefs,
           const std::vector<bool> &textureEdits) : name(name),
                                                    mesh(mesh),
                                                    origin(origin),
                                                    rotation(rotation),
                                                    textureRefs(textureRefs),
                                                    textureEdits(textureEdits)
{
    mesh->incRef();
    if (textureRefs.size() != textureEdits.size())
    {
        this->textureEdits = std::vector<bool>(textureRefs.size(), false);
    }
}

Part::~Part()
{
    // printf("Part::~Part()");
    releaseMesh();
}

void Part::releaseMesh()
{
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

void Part::setMesh(Mesh *value)
{
    value->incRef();
    releaseMesh();
    mesh = value;
}
Mesh *Part::getMesh() const
{
    return mesh;
}
