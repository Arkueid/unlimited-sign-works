#include <QApplication>
#include <win/scene/PoleScene.hpp>
#include <utils/TextureManager.hpp>

#include <win/PoleDesigner.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PoleDesigner w;
    w.show();
    // TextureManager::releaseTextures();
    return app.exec();
}