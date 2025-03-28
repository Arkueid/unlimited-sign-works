#include <part/pole/Pole168.hpp>
#include "Pole168.hpp"

Pole168::Pole168() : sideBeam(new Cylinder(108.0f, 500.0f + 1000.0f + 2500.0f)),
                     mainPole(new Cylinder(168.0f, 6700.0f)),
                     flange(new Cylinder(300.0f, 20.0f)),
                     panel(new RoundedCube(2500.0f, 2000.0f, 10.0f, 210.0f)),
                     bottom(new Cube(400.0f, 400.0f, 20.0f)),
                     sideStiffener(new Stiffener(30.0f, 104.0f, 550.0f, 750.0f, 8.0f, 0.0f, 20.0f)),
                     bottomStiffener(new Stiffener(50.0f, 200.0f, 30.0f, 110.0f, 8.0f, 20.0f, 0.0f))
{
    // textures.push_back("../../assets/textures/panel/CN_road_sign.png");
    textures.push_back("../../assets/textures/panel/D25.bmp");
    textures.push_back("../../assets/textures/metal/metal6.bmp");
    textures.push_back("../../assets/textures/metal/metal7.bmp");
    textures.push_back("../../assets/textures/metal/metal3.bmp");
    textures.push_back("../../assets/textures/metal/metal5.bmp");

     parts.push_back(new Part{mainPole, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {3, 3, 1}, &textures});

     parts.push_back(new Part{sideBeam, {-250.0f, 6700.0f - 300.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {3, 3, 1}, &textures});

     parts.push_back(new Part{sideBeam, {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {3, 3, 1}, &textures});

     parts.push_back(new Part{panel, {4000.0f - 250.0f - 1250.0f, 6700.0f - 300.0f - 300.0f, 10.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {0, 4, 4, 4, 4, 4}, &textures});

     parts.push_back(new Part{bottom, {0.0f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{flange, {250.0f, 6700.0f - 300.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {2, 2, 2}, &textures});

     parts.push_back(new Part{flange, {250.0f, 6700.0f - 300.0f - 600.0f, 0.0f}, {0.0f, 0.0f, -90.0f}, {2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f + 54.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f, 0.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f - 54.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f, 0.0f - 54.0f}, {270.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f - 600.0f + 54.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f + 54.0f}, {90.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f - 600.0f - 54.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{sideStiffener, {-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f - 54.0f},{ 270.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{bottomStiffener, {84.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{bottomStiffener, {0.0f, 0.0f, 0.0f - 84.0f}, {0.0f, 90.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{bottomStiffener, {0.0f - 84.0f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});

     parts.push_back(new Part{bottomStiffener, {0.0f, 0.0f, 0.0f + 84.0f}, {0.0f, 270.0f, 0.0f}, {2, 2, 2, 2, 2, 2, 2}, &textures});
}
Pole168::~Pole168()
{
    delete sideBeam;
    delete mainPole;
    delete flange;
    delete panel;
    delete bottom;
    delete sideStiffener;
    delete bottomStiffener;
}