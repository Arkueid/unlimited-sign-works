#include <utils/PoleJsonSerializer.hpp>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

void PoleJsonSerializer::toJson(Pole *pole, QJsonObject& root)
{
    root = QJsonObject();
    root["name"] = pole->getName().c_str();
    root["type"] = pole->getType();

    QJsonArray parts;
    QJsonArray meshes;

    QMap<Mesh *, int> mesh2Index;
    int meshIndex = 0;

    for (auto part : pole->getParts())
    {
        Mesh *mesh = part->getMesh();
        if (mesh2Index.find(mesh) == mesh2Index.end())
        {
            mesh2Index[mesh] = meshIndex;
            meshIndex++;
            QJsonObject meshJson;
            switch (mesh->getType())
            {
            case MeshType_Cube:
                initCubeJson(static_cast<Cube *>(mesh), meshJson);
                break;
            case MeshType_Cylinder:
                initCylinderJson(static_cast<Cylinder *>(mesh), meshJson);
                break;
            case MeshType_Stiffener:
                initStiffenerJson(static_cast<Stiffener *>(mesh), meshJson);
                break;
            case MeshType_RoundedCube:
                initRoundedCubeJson(static_cast<RoundedCube *>(mesh), meshJson);
                break;
            case MeshType_RoundedRegTriPrism:
                initRoundedRegTriPrismJson(static_cast<RoundedRegTriPrism *>(mesh), meshJson);
                break;
            default:
                break;
            }
            meshes.append(meshJson);
        }

        const int meshRef = mesh2Index[mesh];

        QJsonObject partJson;
        initPartJson(meshRef, part, partJson);
        parts.append(partJson);
    }

    root["parts"] = parts;
    root["meshes"] = meshes;
}
Pole *PoleJsonSerializer::fromJson(QJsonObject& root)
{
    QJsonArray parts = root["parts"].toArray();
    QJsonArray meshes = root["meshes"].toArray();

    Pole *pole = new Pole();
    pole->setType(root["type"].toInt());
    pole->setName(root["name"].toString().toStdString());

    QMap<int, Mesh *> ref2Mesh;
    for (auto it : parts)
    {
        QJsonObject partJson = it.toObject();
        const int meshRef = partJson["meshRef"].toInt();
        if (ref2Mesh.find(meshRef) == ref2Mesh.end())
        {
            QJsonObject meshJson = meshes[meshRef].toObject();
            Mesh *mesh = createMesh(meshJson);
            ref2Mesh[meshRef] = mesh;
        }
        Mesh *mesh = ref2Mesh[meshRef];
        Part *part = createPart(mesh, partJson);
        pole->addPart(part);
    }

    return pole;
}

void PoleJsonSerializer::toJsonFile(Pole *pole, const QString &filePath)
{
    QJsonObject root;
    toJson(pole, root);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    }
}
Pole *PoleJsonSerializer::fromJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file: " << filePath;
        return nullptr;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    return fromJson(root);
}
void PoleJsonSerializer::initCubeJson(Cube *cube, QJsonObject &mesh)
{
    mesh["a"] = cube->getA();
    mesh["b"] = cube->getB();
    mesh["c"] = cube->getC();
    mesh["type"] = MeshType_Cube;
}

void PoleJsonSerializer::initCylinderJson(Cylinder *cylinder, QJsonObject &mesh)
{
    mesh["d"] = cylinder->getD();
    mesh["h"] = cylinder->getH();
    mesh["type"] = MeshType_Cylinder;
}

void PoleJsonSerializer::initStiffenerJson(Stiffener *stiffener, QJsonObject &mesh)
{
    mesh["a"] = stiffener->getA();
    mesh["b"] = stiffener->getB();
    mesh["c"] = stiffener->getC();
    mesh["h1"] = stiffener->getH1();
    mesh["h2"] = stiffener->getH2();
    mesh["offsetH"] = stiffener->getOffsetH();
    mesh["offsetV"] = stiffener->getOffsetV();
    mesh["type"] = MeshType_Stiffener;
}

void PoleJsonSerializer::initRoundedCubeJson(RoundedCube *rCube, QJsonObject &mesh)
{
    mesh["a"] = rCube->getA();
    mesh["b"] = rCube->getB();
    mesh["c"] = rCube->getC();
    mesh["radius"] = rCube->getRadius();
    mesh["type"] = MeshType_RoundedCube;
}

void PoleJsonSerializer::initRoundedRegTriPrismJson(RoundedRegTriPrism *rRegTriPrism, QJsonObject &mesh)
{
    mesh["a"] = rRegTriPrism->getA();
    mesh["h"] = rRegTriPrism->getH();
    mesh["radius"] = rRegTriPrism->getRadius();
    mesh["type"] = MeshType_RoundedRegTriPrism;
}

void PoleJsonSerializer::initPartJson(int meshRef, Part *part, QJsonObject &partJson)
{
    partJson["name"] = part->getName().c_str();
    partJson["meshRef"] = meshRef;
    partJson["isPoleBase"] = part->isPoleBase();
    
    QJsonArray position;
    QJsonArray rotation;
    QJsonArray textureRefs;
    QJsonArray textureEdits;

    position.append(part->getOriginX());
    position.append(part->getOriginY());
    position.append(part->getOriginZ());

    rotation.append(part->getRotationX());
    rotation.append(part->getRotationY());
    rotation.append(part->getRotationZ());

    const int size = part->getTextureRefs().size();
    const std::vector<int> &textureRefsVec = part->getTextureRefs();
    const std::vector<bool> &textureEditsVec = part->getTextureEdits();
    for (int i = 0; i < size; i++)
    {
        textureRefs.append(textureRefsVec[i]);
        textureEdits.append(textureEditsVec[i]);
    }

    partJson["position"] = position;
    partJson["rotation"] = rotation;
    partJson["textureRefs"] = textureRefs;
    partJson["textureEdits"] = textureEdits;
}

Mesh *PoleJsonSerializer::createMesh(QJsonObject &meshJson)
{
    int type = meshJson["type"].toInt();
    Mesh *mesh = nullptr;
    switch (type)
    {
    case MeshType_Cube:
        mesh = createCube(meshJson);
        break;
    case MeshType_Cylinder:
        mesh = createCylinder(meshJson);
        break;
    case MeshType_Stiffener:
        mesh = createStiffener(meshJson);
        break;
    case MeshType_RoundedCube:
        mesh = createRoundedCube(meshJson);
        break;
    case MeshType_RoundedRegTriPrism:
        mesh = createRoundedRegTriPrism(meshJson);
        break;
    default:
        qDebug() << "Unknown mesh type: " << type;
        break;
    }
    return mesh;
}

Cube *PoleJsonSerializer::createCube(QJsonObject &meshJson)
{
    const float a = meshJson["a"].toDouble();
    const float b = meshJson["b"].toDouble();
    const float c = meshJson["c"].toDouble();

    return Cube::create(a, b, c);
}

Cylinder *PoleJsonSerializer::createCylinder(QJsonObject &meshJson)
{
    const float d = meshJson["d"].toDouble();
    const float h = meshJson["h"].toDouble();

    return Cylinder::create(d, h);
}

Stiffener *PoleJsonSerializer::createStiffener(QJsonObject &meshJson)
{
    const float a = meshJson["a"].toDouble();
    const float b = meshJson["b"].toDouble();
    const float c = meshJson["c"].toDouble();
    const float h1 = meshJson["h1"].toDouble();
    const float h2 = meshJson["h2"].toDouble();
    const float offsetH = meshJson["offsetH"].toDouble();
    const float offsetV = meshJson["offsetV"].toDouble();
    return Stiffener::create(a, b, h1, h2, c, offsetH, offsetV);
}

RoundedCube *PoleJsonSerializer::createRoundedCube(QJsonObject &meshJson)
{
    const float a = meshJson["a"].toDouble();
    const float b = meshJson["b"].toDouble();
    const float c = meshJson["c"].toDouble();
    const float radius = meshJson["radius"].toDouble();
    return RoundedCube::create(a, b, c, radius);
}

RoundedRegTriPrism *PoleJsonSerializer::createRoundedRegTriPrism(QJsonObject &meshJson)
{
    const float a = meshJson["a"].toDouble();
    const float h = meshJson["h"].toDouble();
    const float radius = meshJson["radius"].toDouble();
    return RoundedRegTriPrism::create(a, h, radius);
}

Part *PoleJsonSerializer::createPart(Mesh *mesh, QJsonObject &partJson)
{
    QString name = partJson["name"].toString();
    QJsonArray position = partJson["position"].toArray();
    QJsonArray rotation = partJson["rotation"].toArray();
    QJsonArray textureRefs = partJson["textureRefs"].toArray();
    QJsonArray textureEdits = partJson["textureEdits"].toArray();
    std::vector<int> textureRefsVec;
    std::vector<bool> textureEditsVec;
    const int size = textureRefs.size();
    const int size2 = textureEdits.size();
    for (int i = 0; i < size; i++)
    {
        textureRefsVec.push_back(textureRefs[i].toInt());
        if (i < size2)
        {
            textureEditsVec.push_back(textureEdits[i].toBool());
        }
    }

    Part *part = new Part(mesh, name.toStdString(),
                          {static_cast<float>(position[0].toDouble()), static_cast<float>(position[1].toDouble()), static_cast<float>(position[2].toDouble())},
                          {static_cast<float>(rotation[0].toDouble()), static_cast<float>(rotation[1].toDouble()), static_cast<float>(rotation[2].toDouble())},
                          textureRefsVec,
                          textureEditsVec);
                          
    part->setPoleBase(partJson["isPoleBase"].toBool());
    return part;
}
