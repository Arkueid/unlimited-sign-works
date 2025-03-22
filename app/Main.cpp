#include <QApplication>
#include <win/scene/GLScene.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GLScene w;
    w.show();
    app.exec();

    // printf("shit\n");
    return 0;
}