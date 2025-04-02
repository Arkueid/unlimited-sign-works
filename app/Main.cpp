#include <QApplication>
#include <win/scene/PoleScene.hpp>
#include <utils/TextureManager.hpp>

#include <win/MainWindow.hpp>

#include <sqlite3/SQLite.hpp>

#include <utils/TextureManager.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString cd = QCoreApplication::applicationDirPath();

    SQLite::initialize(cd + "/../../settings.db");

    TextureManager::loadTexturesPaths();

    MainWindow w;
    w.show();

    SQLite::dispose();

    return app.exec();
}