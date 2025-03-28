#include <QApplication>
#include <win/scene/GLScene2.hpp>
#include <utils/TextureManager.hpp>

#include <win/Designer.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Designer w;
    w.show();
    // TextureManager::releaseTextures();
    return app.exec();
}