#include "Curve.h"

Curve::Curve()
{

	for (auto& A : this->a) {
		A = rand() % 360;
	}
    for (auto& angle : this->angleStep) {
        angle = rand() % 4;
    }
	this->p[0] = sf::Vector2i(0, this->HEIGHT);
	this->p[1] = sf::Mouse::getPosition();
	this->p[2] = sf::Vector2i(this->WIDTH / 2, this->HEIGHT / 2);
	this->p[3] = sf::Vector2i(WIDTH, 0);

    this->colour.r = 255;
    this->colour.g= 255;
    this->colour.b = 255;

}

Curve::~Curve()
{
}

void Curve::drawLine(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, rgb& colour)
{
    sf::Vertex line[] = {
           sf::Vertex(sf::Vector2f(p1), sf::Color(colour.r, colour.g, colour.b)),
           sf::Vertex(sf::Vector2f(p2), sf::Color(colour.r, colour.g, colour.b)),
    };

    window.draw(line, 2, sf::LineStrip);
}

void Curve::HSVtoRGB(float H, float S, float V, rgb& colour)
{
    if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
        std::cout << "The givem HSV values are not in valid range \n";
        return;
    }
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;

    colour.r = R;
    colour.g = G;
    colour.b = B;
}

void Curve::update(sf::RenderWindow& window)
{
	this->p[1] = sf::Mouse::getPosition(window);

    arad[0] = a[0] * PI / 180;
    p[0].x = cx + this->r * std::cos(arad[0]);
    p[0].y = cy + this->r * std::sin(arad[0]);
    a[0] += angleStep[0];
    a[0] %= 360;

    arad[2] = a[2] * PI / 180;
    p[2].x = cx + this->r / 2 * std::cos(arad[2]);
    p[2].y = cy + this->r / 2 * std::sin(arad[2]);
    a[2] -= angleStep[2];
    a[2] %= 360;

    arad[3] = a[3] * PI / 180;
    p[3].x = cx + this->r * std::cos(arad[3]);
    p[3].y = cy + this->r * std::sin(arad[3]);
    a[3] += angleStep[3];
    a[3] %= 360;

}


int Curve::pointDivide(int& x1, int& x2, float& m, float& n)
{
    return int((m * x2 + n * x1) / (m + n));
}


void Curve::render(sf::RenderWindow& window)
{

    for (float t = 0; t <= 1; t += 0.005) {
        float m = t;
        float n = 1 - t;

        int p12x = pointDivide(p[0].x, p[1].x, m, n);
        int p12y = pointDivide(p[0].y, p[1].y, m, n);
        int p23x = pointDivide(p[1].x, p[2].x, m, n);
        int p23y = pointDivide(p[1].y, p[2].y, m, n);
        int p34x = pointDivide(p[2].x, p[3].x, m, n);
        int p34y = pointDivide(p[2].y, p[3].y, m, n);

        sf::Vector2i p12(p12x, p12y);
        sf::Vector2i p23(p23x, p23y);
        sf::Vector2i p34(p34x, p34y);

        int p1x = pointDivide(p12x, p23x, m, n);
        int p1y = pointDivide(p12y, p23y, m, n);
        int p2x = pointDivide(p23x, p34x, m, n);
        int p2y = pointDivide(p23y, p34y, m, n);


        sf::Vector2i point1(p1x, p1y);
        sf::Vector2i point2(p2x, p2y);

        int lx = pointDivide(point1.x, point2.x, m, n);
        int ly = pointDivide(point1.y, point2.y, m, n);

        if (this->COLOUR) {
            HSVtoRGB(t * 360, 100, 100, colour);
        }
        else {
            colour.r = 255;
            colour.g = 255;
            colour.b = 255;
        }

        if (this->DOTS) {
            sf::CircleShape f(2);
            f.setFillColor(sf::Color(this->colour.r, this->colour.g, this->colour.b));
            f.setPosition(sf::Vector2f(lx, ly));
            window.draw(f);
        }
        else {
            drawLine(window, p12, p23, colour);
            drawLine(window, p23, p34, colour);

            drawLine(window, point1, point2, colour);
        }

    }
}