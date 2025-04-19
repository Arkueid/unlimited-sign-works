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
    sqlite3_exec(db, "create table if not exists texture (id integer primary key autoincrement, name text not null unique, is_private integer not null);", nullptr, nullptr, &errMsg);

    printIfError(errMsg);

    sqlite3_exec(db,
                 "insert or ignore into texture (id, name, is_private) values"
                 "(0, 'panel/CN_road_sign.png', false),"
                 "(1, 'panel/CN_road_sign2.png', false),"
                 "(2, 'metal/metal1.bmp', false),"
                 "(3, 'metal/metal2.bmp', false),"
                 "(4, 'metal/metal3.bmp', false),"
                 "(5, 'metal/metal4.bmp', false),"
                 "(6, 'metal/metal5.bmp', false),"
                 "(7, 'metal/metal6.bmp', false),"
                 "(8, 'metal/metal7.bmp', false),"
                 "(9, 'template/D25.bmp', false);",
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
        qDebug() << errMsg;
        sqlite3_free(errMsg);
    }
}