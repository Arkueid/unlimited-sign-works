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
                 "insert or ignore into texture (id, name) values"
                 "(0, 'panel/CN_road_sign.png'),"
                 "(1, 'panel/CN_road_sign2.png'),"
                 "(2, 'metal/metal1.bmp'),"
                 "(3, 'metal/metal2.bmp'),"
                 "(4, 'metal/metal3.bmp'),"
                 "(5, 'metal/metal4.bmp'),"
                 "(6, 'metal/metal5.bmp'),"
                 "(7, 'metal/metal6.bmp'),"
                 "(8, 'metal/metal7.bmp'),"
                 "(9, 'template/D25.bmp');",
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