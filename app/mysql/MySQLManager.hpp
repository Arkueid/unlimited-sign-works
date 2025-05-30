#pragma once

#include <pole/Pole.hpp>
#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class MySQLManager 
{

public:
    static void initialize();
    static QSqlDatabase getDatabase();
};