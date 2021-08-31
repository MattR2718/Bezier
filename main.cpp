#include <SFML/Graphics.hpp>
#include <iostream>

struct rgb {
    int r;
    int g;
    int b;
};

void HSVtoRGB(float H, float S, float V, rgb& colour) {
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


void drawPoints(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, sf::Vector2i& p3, sf::Vector2i& p4, const int& r) {
    sf::CircleShape point(r);
    point.setFillColor(sf::Color::White);
    point.setPosition(sf::Vector2f(p1.x - r, p1.y - r));
    window.draw(point);
    point.setPosition(sf::Vector2f(p2.x - r, p2.y - r));
    window.draw(point);
    point.setPosition(sf::Vector2f(p3.x - r, p3.y - r));
    window.draw(point);
    point.setPosition(sf::Vector2f(p4.x - r, p4.y - r));
    window.draw(point);
}

void drawLine(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, rgb& colour) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1), sf::Color(colour.r, colour.g, colour.b)),
            sf::Vertex(sf::Vector2f(p2), sf::Color(colour.r, colour.g, colour.b)),
    };

    window.draw(line, 2, sf::LineStrip);
}

int pointDivide(int& x1, int& x2, float& m, float& n) {
    return int((m * x2 + n * x1) / (m + n));
}


int main()
{
    const int WIDTH = 800;
    const int HEIGHT = 800;
    const int r = 5;

    //Ceate the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cubic Bezier");

    sf::Vector2i p1(0, HEIGHT);
    sf::Vector2i p2;
    sf::Vector2i p3(WIDTH / 2, 0);
    sf::Vector2i p4(WIDTH, 0);

    rgb colour;
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;

    //Run the program as long as the window is open
    while (window.isOpen())
    {
        //Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //"close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Clear the window with black color
        window.clear(sf::Color::Black);

        //Set point 2 to mouse position
        p2.x = sf::Mouse::getPosition(window).x;
        p2.y = sf::Mouse::getPosition(window).y;

        //Move points
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            p1.x = sf::Mouse::getPosition(window).x;
            p1.y = sf::Mouse::getPosition(window).y;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
            p3.x = sf::Mouse::getPosition(window).x;
            p3.y = sf::Mouse::getPosition(window).y;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
            p4.x = sf::Mouse::getPosition(window).x;
            p4.y = sf::Mouse::getPosition(window).y;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) {
            p1.x = 0;
            p1.y = HEIGHT;

            p3.x = WIDTH /2;
            p3.y = 0;

            p4.x = WIDTH;
            p4.y = 0;
        }

        // draw everything here...
        // window.draw(...);

        //Draw points on window
        drawPoints(window, p1, p2, p3, p4, r);

        //Draw segments between control points 1->2 2->3 3->4
        drawLine(window, p1, p2, colour);
        drawLine(window, p2, p3, colour);
        drawLine(window, p3, p4, colour);

        //Step through curve
        for (float t = 0; t <= 1; t += 0.05) {
            float m = t;
            float n = 1 - t;

            int p12x = pointDivide(p1.x, p2.x, m, n);
            int p12y = pointDivide(p1.y, p2.y, m, n);
            int p23x = pointDivide(p2.x, p3.x, m, n);
            int p23y = pointDivide(p2.y, p3.y, m, n);
            int p34x = pointDivide(p3.x, p4.x, m, n);
            int p34y = pointDivide(p3.y, p4.y, m, n);

            sf::Vector2i p12(p12x, p12y);
            sf::Vector2i p23(p23x, p23y);
            sf::Vector2i p34(p34x, p34y);

            drawLine(window, p12, p23, colour);
            drawLine(window, p23, p34, colour);

            int p1x = pointDivide(p12x, p23x, m, n);
            int p1y = pointDivide(p12y, p23y, m, n);
            int p2x = pointDivide(p23x, p34x, m, n);
            int p2y = pointDivide(p23y, p34y, m, n);


            sf::Vector2i point1(p1x, p1y);
            sf::Vector2i point2(p2x, p2y);

            HSVtoRGB(t * 360, 100, 100, colour);

            drawLine(window, point1, point2, colour);
        }



        //End the current frame
        window.display();
    }
}