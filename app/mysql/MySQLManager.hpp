#pragma once

#include <pole/Pole.hpp>
#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

#include <QSqlQuery>

class MySQLManager 
{

public:
    static void initialize();
    static void savePole(Pole* pole);
    static void saveRoad(const QString& name, QVector<Pole*>& poles);

};