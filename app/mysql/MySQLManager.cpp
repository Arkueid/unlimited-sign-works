#include "MySQLManager.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

void MySQLManager::initialize()
{
    if (!QSqlDatabase::contains("mysql"))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "mysql");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("1234");
        db.setDatabaseName("roadbim");
        db.setPort(3306);
        db.open();

        if (!db.isOpen())
        {
            qDebug() << db.lastError();
            return;
        }

        db.exec("insert ignore into roadbim.texture (id, path, is_private) values"
                 "(0, 'panel/CN_road_sign.png', false),"
                 "(1, 'panel/CN_road_sign2.png', false),"
                 "(2, 'metal/metal1.bmp', false),"
                 "(3, 'metal/metal2.bmp', false),"
                 "(4, 'metal/metal3.bmp', false),"
                 "(5, 'metal/metal4.bmp', false),"
                 "(6, 'metal/metal5.bmp', false),"
                 "(7, 'metal/metal6.bmp', false),"
                 "(8, 'metal/metal7.bmp', false),"
                 "(9, 'template/D25.bmp', false);");
        db.commit();
    }
}

void MySQLManager::savePole(Pole *pole)
{
    QSqlDatabase db = QSqlDatabase::database("mysql");

}

void MySQLManager::saveRoad(const QString &name, QVector<Pole *> &poles)
{
}
