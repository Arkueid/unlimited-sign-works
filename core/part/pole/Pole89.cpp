#include <part/pole/Pole89.hpp>

Pole89::Pole89():
	mainPole(89.0f, 4500.0f),
	panel(900.0f, 10.0f, 40.0f),
	bottom(200.0f, 200.0f, 20.0f),
	bottomStiffener(30.0f, 150.0f, 20.0f, 50.0f, 20.0f, 0.0f)
{
	textures.push_back("../../assets/textures/panel/CN_road_sign.png");
	textures.push_back("../../assets/textures/metal/metal4.bmp");
	textures.push_back("../../assets/textures/metal/metal6.bmp");
	textures.push_back("../../assets/textures/metal/metal7.bmp");

	parts.push_back({ &mainPole, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2, 2, 2}, &textures});
	parts.push_back({ &panel, {0.0f, 4500.0f - 900.0f / 2.0f / 1.72, 44.5f}, {90.0f, 0.0f, 0.0f}, {0, 1, 1}, &textures });

	parts.push_back({ &bottom, {0.0f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back({ &bottomStiffener, {44.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back({ &bottomStiffener, {0.0f, 0.0f, 0.0f - 44.5f}, {0.0f, 90.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back({ &bottomStiffener, {0.0f - 44.5f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });

	parts.push_back({ &bottomStiffener, {0.0f, 0.0f, 0.0f + 44.5f}, {0.0f, 270.0f, 0.0f}, {3, 3, 3, 3, 3, 3, 3}, &textures });
}

Pole89::~Pole89()
{
}
