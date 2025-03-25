#include <part/pole/Pole168.hpp>

Pole168::Pole168() : sideBeam(108.0f, 500.0f + 1000.0f + 2500.0f),
                     mainPole(168.0f, 6700.0f),
                     flange(300.0f, 20.0f),
                     panel(2500.0f, 10.0f, 2000.0f),
                     bottom(400.0f, 400.0f, 20.0f),
                     sideStiffener(30.0f, 104.0f, 550.0f, 750.0f, 8.0f, 0.0f, 20.0f),
                     bottomStiffener(50.0f, 200.0f, 30.0f, 110.0f, 8.0f, 20.0f, 0.0f)
{
    mainPoleMat.setToIdentity();
    parts.append({&mainPole, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    sideBeam1Mat.setToIdentity();
    sideBeam1Mat.translate(-250.0f, 6700.0f - 300.0f, 0.0f); // originX, originY, originZ
    sideBeam1Mat.rotate(-90.0f, 0.0f, 0.0f, 1.0f); // rotateX, rotateY, rotateZ
    parts.append({&sideBeam, -250.0f, 6700.0f - 300.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    sideBeam2Mat.setToIdentity();
    sideBeam2Mat.translate(-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f);
    sideBeam2Mat.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
    parts.append({&sideBeam, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    panelMat.setToIdentity();
    panelMat.translate(4000.0f -250.0f - 1250.0f, 6700.0f - 300.0f - 300.0f, 10.0f + 54.0f);
    parts.append({&panel, 4000.0f - 250.0f - 1250.0f, 6700.0f - 300.0f - 300.0f, 10.0f + 54.0f, 0.0f, 0.0f, 0.0f});

    bottomMat.setToIdentity();
    bottomMat.translate(0.0f, 10.0f, 0.0f);
    parts.append({&bottom, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    flange1Mat.setToIdentity();
    flange1Mat.translate(250.0f, 6700.0f - 300.0f, 0.0f);
    flange1Mat.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
    parts.append({&flange, 250.0f, 6700.0f - 300.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    flange2Mat.setToIdentity();
    flange2Mat.translate(250.0f, 6700.0f - 300.0f - 600.0f, 0.0f);
    flange2Mat.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
    parts.append({&flange, 250.0f, 6700.0f - 300.0f - 600.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    sideStiffener1Mat.setToIdentity();
    sideStiffener1Mat.translate(-250.0f, 6700.0f - 300.0f + 54.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f + 54.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    sideStiffener2Mat.setToIdentity();
    sideStiffener2Mat.translate(-250.0f, 6700.0f - 300.0f, 0.0f + 54.0f);
    sideStiffener2Mat.rotate(90.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f + 54.0f, 0.0f, 90.0f, 0.0f, 0.0f});

    sideStiffener3Mat.setToIdentity();
    sideStiffener3Mat.translate(-250.0f, 6700.0f - 300.0f - 54.0f, 0.0f);
    sideStiffener3Mat.rotate(180.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 54.0f, 0.0f, 180.0f, 0.0f, 0.0f});

    sideStiffener4Mat.setToIdentity();
    sideStiffener4Mat.translate(-250.0f, 6700.0f - 300.0f, 0.0f - 54.0f);
    sideStiffener4Mat.rotate(270.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f, 0.0f - 54.0f, 270.0f, 0.0f, 0.0f});

    sideStiffener5Mat.setToIdentity();
    sideStiffener5Mat.translate(-250.0f, 6700.0f - 300.0f - 600.0f + 54.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f + 54.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    sideStiffener6Mat.setToIdentity();
    sideStiffener6Mat.translate(-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f + 54.0f);
    sideStiffener6Mat.rotate(90.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f + 54.0f, 90.0f, 0.0f, 0.0f});

    sideStiffener7Mat.setToIdentity();
    sideStiffener7Mat.translate(-250.0f, 6700.0f - 300.0f - 600.0f - 54.0f, 0.0f);
    sideStiffener7Mat.rotate(180.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f - 54.0f, 0.0f, 180.0f, 0.0f, 0.0f});

    sideStiffener8Mat.setToIdentity();
    sideStiffener8Mat.translate(-250.0f, 6700.0f - 300.0f - 600.0f, 0.0f - 54.0f);
    sideStiffener8Mat.rotate(270.0f, 1.0f, 0.0f, 0.0f);
    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f - 54.0f, 270.0f, 0.0f, 0.0f});

    bottomStiffener1Mat.setToIdentity();
    bottomStiffener1Mat.translate(84.0f, 0.0f, 0.0f);
    parts.append({&bottomStiffener, 84.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    bottomStiffener2Mat.setToIdentity();
    bottomStiffener2Mat.translate(0.0f, 0.0f, 0.0f - 84.0f);
    bottomStiffener2Mat.rotate(90.0f, 0.0f, 1.0f, 0.0f);
    parts.append({&bottomStiffener, 0.0f, 0.0f, 0.0f - 84.0f, 0.0f, 90.0f, 0.0f});

    bottomStiffener3Mat.setToIdentity();
    bottomStiffener3Mat.translate(0.0f - 84.0f, 0.0f, 0.0f);
    bottomStiffener3Mat.rotate(180.0f, 0.0f, 1.0f, 0.0f);
    parts.append({&bottomStiffener, 0.0f - 84.0f, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f});

    bottomStiffener4Mat.setToIdentity();
    bottomStiffener4Mat.translate(0.0f, 0.0f, 0.0f + 84.0f);
    bottomStiffener4Mat.rotate(270.0f, 0.0f, 1.0f, 0.0f);
    parts.append({&bottomStiffener, 0.0f, 0.0f, 0.0f + 84.0f, 0.0f, 270.0f, 0.0f});
}