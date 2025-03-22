#pragma once

#include <part/pole/Pole.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/Cube.hpp>
#include <QMatrix4x4>

class Pole168 : public Pole {
public:
    Pole168();
    ~Pole168() override = default;

    Cylinder sideBeam;
    Cylinder mainPole;
    Cylinder flange;
    Cube panel;
    Cube bottom;
    Stiffener sideStiffener;
    Stiffener bottomStiffener;

    QMatrix4x4 sideBeam1Mat;
    QMatrix4x4 sideBeam2Mat;
    QMatrix4x4 mainPoleMat;
    QMatrix4x4 panelMat;
    QMatrix4x4 bottomMat;
    QMatrix4x4 flange1Mat;
    QMatrix4x4 flange2Mat;

    QMatrix4x4 sideStiffener1Mat;
    QMatrix4x4 sideStiffener2Mat;
    QMatrix4x4 sideStiffener3Mat;
    QMatrix4x4 sideStiffener4Mat;

    QMatrix4x4 sideStiffener5Mat;
    QMatrix4x4 sideStiffener6Mat;
    QMatrix4x4 sideStiffener7Mat;
    QMatrix4x4 sideStiffener8Mat;

    QMatrix4x4 bottomStiffener1Mat;
    QMatrix4x4 bottomStiffener2Mat;
    QMatrix4x4 bottomStiffener3Mat;
    QMatrix4x4 bottomStiffener4Mat;
};