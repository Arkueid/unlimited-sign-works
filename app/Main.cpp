#include <QApplication>
#include <win/scene/GLScene2.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GLScene2 w;
    w.show();
    app.exec();
    return 0;
}