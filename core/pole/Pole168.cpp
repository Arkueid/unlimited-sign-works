#include <pole/Pole168.hpp>

Pole168::Pole168()
{
	std::vector<Mesh*> shapes;
    shapes.push_back(new Cylinder(108.0f, 500.0f + 1000.0f + 2500.0f));
    shapes.push_back(new Cylinder(168.0f, 6700.0f));
    shapes.push_back(new Cylinder(300.0f, 20.0f));
    shapes.push_back(new RoundedCube(2500.0f, 2000.0f, 10.0f, 100.0f));
    shapes.push_back(new Cube(400.0f, 400.0f, 20.0f));
    shapes.push_back(new Stiffener(30.0f, 104.0f, 550.0f, 750.0f, 8.0f, 0.0f, 20.0f));
    shapes.push_back(new Stiffener(50.0f, 200.0f, 30.0f, 110.0f, 8.0f, 20.0f, 0.0f));

    const int sideBeami = 0;
    const int mainPolei = 1;
    const int flangei = 2;
    const int paneli = 3;
    const int bottomi = 4;
    const int sideStiffeneri = 5;
    const int bottomStiffeneri = 6;

    // textures.push_back("../../assets/textures/panel/D25.bmp");
    // textures.push_back("../../assets/textures/metal/metal6.bmp");
    // textures.push_back("../../assets/textures/metal/metal7.bmp");
    // textures.push_back("../../assets/textures/metal/metal3.bmp");
    // textures.push_back("../../assets/textures/metal/metal5.bmp");

    parts.push_back(new Part{shapes[mainPolei], "mainPole", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {4, 4, 7}});

    parts.push_back(new Part{shapes[sideBeami], "sideBeam1", {-250.0f, 6700.0f - 300.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {4, 4, 7}});

    parts.push_back(new Part{shapes[sideBeami], "sideBeam2", {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {4, 4, 7}});

    parts.push_back(new Part{shapes[paneli], "panel1", {4000.0f - 250.0f - 1250.0f, 6700.0f - 300.0f - 300.0f, 10.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {1, 6, 6, 6, 6, 6}});

    parts.push_back(new Part{shapes[bottomi], "bottom", {0.0f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[flangei], "flange1", {250.0f, 6700.0f - 300.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {8, 8, 8}});

    parts.push_back(new Part{shapes[flangei], "flange2", {250.0f, 6700.0f - 300.0f - 600.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener1", {-250.0f, 6700.0f - 300.0f + 54.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener2", {-250.0f, 6700.0f - 300.0f, 0.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener3", {-250.0f, 6700.0f - 300.0f - 54.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener4", {-250.0f, 6700.0f - 300.0f, 0.0f - 54.0f}, {270.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener5", {-250.0f, 6700.0f - 300.0f - 600.0f + 54.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener6", {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener7", {-250.0f, 6700.0f - 300.0f - 600.0f - 54.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[sideStiffeneri], "sideStiffener8", {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f - 54.0f}, {270.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[bottomStiffeneri], "bottomStiffener1", {84.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[bottomStiffeneri], "bottomStiffener2", {0.0f, 0.0f, 0.0f - 84.0f}, {0.0f, 90.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[bottomStiffeneri], "bottomStiffener3", {0.0f - 84.0f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});

    parts.push_back(new Part{shapes[bottomStiffeneri], "bottomStiffener4", {0.0f, 0.0f, 0.0f + 84.0f}, {0.0f, 270.0f, 0.0f}, {8, 8, 8, 8, 8, 8, 8}});
}
Pole168::~Pole168()
{
}