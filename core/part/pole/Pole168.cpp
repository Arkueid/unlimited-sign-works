#include <part/pole/Pole168.hpp>

Pole168::Pole168() : sideBeam(108.0f, 500.0f + 1000.0f + 2500.0f),
                     mainPole(168.0f, 6700.0f),
                     flange(300.0f, 20.0f),
                     panel(2500.0f, 10.0f, 2000.0f),
                     bottom(400.0f, 400.0f, 20.0f),
                     sideStiffener(30.0f, 104.0f, 550.0f, 750.0f, 8.0f, 0.0f, 20.0f),
                     bottomStiffener(50.0f, 200.0f, 30.0f, 110.0f, 8.0f, 20.0f, 0.0f)
{
    parts.append({&mainPole, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&sideBeam, -250.0f, 6700.0f - 300.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    parts.append({&sideBeam, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    parts.append({&panel, 4000.0f - 250.0f - 1250.0f, 6700.0f - 300.0f - 300.0f, 10.0f + 54.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&bottom, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&flange, 250.0f, 6700.0f - 300.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    parts.append({&flange, 250.0f, 6700.0f - 300.0f - 600.0f, 0.0f, 0.0f, 0.0f, -90.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f + 54.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f + 54.0f, 0.0f, 90.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 54.0f, 0.0f, 180.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f, 0.0f - 54.0f, 270.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f + 54.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f + 54.0f, 90.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f - 54.0f, 0.0f, 180.0f, 0.0f, 0.0f});

    parts.append({&sideStiffener, -250.0f, 6700.0f - 300.0f - 600.0f, 0.0f - 54.0f, 270.0f, 0.0f, 0.0f});

    parts.append({&bottomStiffener, 84.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    parts.append({&bottomStiffener, 0.0f, 0.0f, 0.0f - 84.0f, 0.0f, 90.0f, 0.0f});

    parts.append({&bottomStiffener, 0.0f - 84.0f, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f});

    parts.append({&bottomStiffener, 0.0f, 0.0f, 0.0f + 84.0f, 0.0f, 270.0f, 0.0f});
}