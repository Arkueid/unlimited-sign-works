#include <part/pole/Pole89.hpp>

Pole89::Pole89()
{
	shapes.push_back(new Cylinder(89.0f, 4500.0f));
	shapes.push_back(new RoundedRegTriPrism(900.0f, 10.0f, 40.0f));
	shapes.push_back(new Cube(200.0f, 200.0f, 20.0f));
	shapes.push_back(new Stiffener(30.0f, 150.0f, 20.0f, 50.0f, 20.0f, 0.0f));

	int mainPolei = 0;
	int paneli = 1;
	int bottomi = 2;
	int bottomStiffeneri = 3;

	textures.push_back("../../assets/textures/panel/CN_road_sign.png");
	textures.push_back("../../assets/textures/metal/metal4.bmp");
	textures.push_back("../../assets/textures/metal/metal6.bmp");
	textures.push_back("../../assets/textures/metal/metal7.bmp");

	parts.push_back(new Part{ shapes[mainPolei] , "mainPole", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2}, &textures});
	parts.push_back(new Part{ shapes[paneli] , "panel", {0.0f, 4500.0f - 900.0f / 2.0f / 1.72, 44.5f}, {90.0f, 0.0f, 0.0f}, {0, 1, 1}, &textures});

	parts.push_back(new Part{ shapes[bottomi] , "bottom", {0.0f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {3, 3, 3, 3, 3, 3}, &textures});

	parts.push_back(new Part{ shapes[bottomStiffeneri] , "bottomStiffener1", {44.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures});

	parts.push_back(new Part{ shapes[bottomStiffeneri] , "bottomStiffener2", {0.0f, 0.0f, 0.0f - 44.5f}, {0.0f, 90.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back(new Part{ shapes[bottomStiffeneri] , "bottomStiffener3", {0.0f - 44.5f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back(new Part{ shapes[bottomStiffeneri] , "bottomStiffener4", {0.0f, 0.0f, 0.0f + 44.5f}, {0.0f, 270.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });
}

Pole89::~Pole89()
{
}
