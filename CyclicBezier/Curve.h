#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#define PI 3.14159265359

class Curve
{
private:

	struct rgb {
		int r;
		int g;
		int b;
	};

	const int WIDTH = 800;
	const int HEIGHT = 800;
	const int cx = int(WIDTH / 2);
	const int cy = int(HEIGHT / 2);
	const int r = int(WIDTH / 2);

	rgb colour;
	int angleStep[4] = { 1, 1, 1, 1 };
	int a[4] = { 0, 0, 0, 180 };
	float arad[4] = { 0, 0, 0, 0 };
	sf::Vector2i p[4];

	void drawLine(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, rgb& colour);
	void HSVtoRGB(float H, float S, float V, rgb& colour);

public:

	bool DOTS = false;
	bool COLOUR = true;

	Curve();
	virtual ~Curve();

	void update(sf::RenderWindow& window);
	int pointDivide(int& x1, int& x2, float& m, float& n);
	void render(sf::RenderWindow& window);
};

