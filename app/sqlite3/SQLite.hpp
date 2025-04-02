#pragma once
#include "sqlite3.h"


#include <QString>

class SQLite
{
public:
    static void initialize(const QString& dbPath);
    static sqlite3* getInstance();
    static void printIfError(char* errMsg);
    static void dispose();
};
