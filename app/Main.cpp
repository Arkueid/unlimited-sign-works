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
#if 0
    // 抗锯齿，电脑有点掉帧了，暂时关闭
    QSurfaceFormat fmt;
    fmt.setSamples(4);  // 四个采样点，图形边界处会更平滑
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
    // 所有窗口共享OpenGL上下文
    // 弹出窗口如果不共享OpenGL上下文，则无法复用 vertexbuffer 和 纹理
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);

    MySQLManager::initialize();

    TextureManager::initSignals();
    TextureManager::loadTexturesPaths();

    QApplication::connect(&app, &QApplication::aboutToQuit,
                          []()
                          {
                              TextureManager::destroySignals();
                              TextureManager::checkTextureLeak();
                          });

    MainWindow w;
    w.show();

    return app.exec();
}