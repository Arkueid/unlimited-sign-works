#include <sqlite3/sqlite3.h>
#include <sqlite3/SQLite.hpp>

#include <QDebug>
#include "SQLite.hpp"

namespace
{
    sqlite3 *db = nullptr;
}

void SQLite::initialize(const QString &dbPath) 
{
    sqlite3_open(dbPath.toStdString().c_str(), &db);

    if (db == nullptr)
    {
        qDebug() << "Failed to open database";
    }

    char *errMsg = nullptr;
    sqlite3_exec(db, "create table if not exists texture (id integer primary key autoincrement, name text not null unique);", nullptr, nullptr, &errMsg);

    printIfError(errMsg);

    sqlite3_exec(db,
                    "insert or ignore into texture (name) values"
                    "('panel/CN_road_sign.png'),"
                    "('panel/CN_road_sign2.png'),"
                    "('metal/metal1.bmp'),"
                    "('metal/metal2.bmp'),"
                    "('metal/metal3.bmp'),"
                    "('metal/metal4.bmp'),"
                    "('metal/metal5.bmp'),"
                    "('metal/metal6.bmp'),"
                    "('metal/metal7.bmp'),"
                    "('template/D25.bmp');",
                    nullptr, nullptr, &errMsg);

    printIfError(errMsg);
}

sqlite3 *SQLite::getInstance()
{
    return db;
}

void SQLite::dispose()
{
    sqlite3_close(db);
}

void SQLite::printIfError(char *errMsg)
{
    if (errMsg != nullptr)
    {
        qDebug() << "Failed to insert into table";
        qDebug() << errMsg;
        sqlite3_free(errMsg);
    }
}