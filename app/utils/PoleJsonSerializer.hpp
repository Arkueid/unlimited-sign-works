#pragma once

#include <QJsonObject>
#include <pole/Pole.hpp>

#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

class PoleJsonSerializer
{
    static void initCubeJson(Cube *cube, QJsonObject &meshJson);
    static void initCylinderJson(Cylinder *cylinder, QJsonObject &meshJson);
    static void initStiffenerJson(Stiffener *stiffener, QJsonObject &meshJson);
    static void initRoundedCubeJson(RoundedCube *rCube, QJsonObject &meshJson);
    static void initRoundedRegTriPrismJson(RoundedRegTriPrism *rRegTriPrism, QJsonObject &meshJson);
    static void initPartJson(int meshRef, Part *part, QJsonObject &partJson);

    static Mesh* createMesh(QJsonObject &meshJson);
    static Cube* createCube(QJsonObject &meshJson);
    static Cylinder* createCylinder(QJsonObject &meshJson);
    static Stiffener* createStiffener(QJsonObject &meshJson);
    static RoundedCube* createRoundedCube(QJsonObject &meshJson);
    static RoundedRegTriPrism* createRoundedRegTriPrism(QJsonObject &meshJson);
    static Part* createPart(Mesh* mesh, QJsonObject &partJson);

public:
    PoleJsonSerializer() = delete;

    static void toJsonFile(Pole *pole, const QString &filePath);
    static Pole *fromJsonFile(const QString &filePath);
};