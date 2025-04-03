#include <QApplication>
#include <win/scene/PoleScene.hpp>
#include <utils/TextureManager.hpp>

#include <win/MainWindow.hpp>

#include <sqlite3/SQLite.hpp>

#include <utils/TextureManager.hpp>

#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString cd = QCoreApplication::applicationDirPath();

    SQLite::initialize(cd + "/../../settings.db");

    TextureManager::initSignals();
    TextureManager::loadTexturesPaths();


    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.show();

    QApplication::connect(&app, &QApplication::aboutToQuit,
                          []()
                          {
                              SQLite::dispose();
                              TextureManager::destroySignals();
                          });

    return app.exec();
}