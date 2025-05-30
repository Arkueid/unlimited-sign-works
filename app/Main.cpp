#include <QApplication>
#include <win/scene/PoleScene.hpp>
#include <utils/TextureManager.hpp>

#include <win/MainWindow.hpp>

#include <mysql/MySQLManager.hpp>

#include <utils/TextureManager.hpp>

#include <QSurfaceFormat>

#include <mysql/MySQLManager.hpp>

int main(int argc, char *argv[])
{
#ifndef _DEBUG
    // 抗锯齿，电脑有点掉帧了，暂时关闭
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
    // 所有窗口共享OpenGL上下文
    // 弹出窗口如果不共享OpenGL上下文，则无法复用 vertexbuffer 和 纹理
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);

    MySQLManager::initialize();

    QString cd = QCoreApplication::applicationDirPath();
    
    TextureManager::initSignals();
    TextureManager::loadTexturesPaths();

    MainWindow w;
    w.show();

    QApplication::connect(&app, &QApplication::aboutToQuit,
                          []()
                          {
                              TextureManager::destroySignals();
                              TextureManager::checkTextureLeak();
                          });

    return app.exec();
}