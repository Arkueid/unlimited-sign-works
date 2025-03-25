#include "Part.hpp"

Part::Part(Mesh *mesh, float originX, float originY, float originZ, float rotationX, float rotationY, float rotationZ):
    mesh(mesh), originX(originX), originY(originY), originZ(originZ), rotationX(rotationX), rotationY(rotationY), rotationZ(rotationZ)
{
    modelMatrix.setToIdentity();
    modelMatrix.translate(originX, originY, originZ);
    modelMatrix.rotate(rotationX, 1, 0, 0);
    modelMatrix.rotate(rotationY, 0, 1, 0);
    modelMatrix.rotate(rotationZ, 0, 0, 1);
}