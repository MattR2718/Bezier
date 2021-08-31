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


void drawPoints(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, sf::Vector2i& p3, const int& r) {
    sf::CircleShape point(r);
    point.setFillColor(sf::Color::White);
    point.setPosition(sf::Vector2f(p1.x - r, p1.y - r));
    window.draw(point);
    point.setPosition(sf::Vector2f(p2.x - r, p2.y - r));
    window.draw(point);
    point.setPosition(sf::Vector2f(p3.x - r, p3.y - r));
    window.draw(point);
}

void drawSegments(sf::RenderWindow& window, sf::Vector2i& p1, sf::Vector2i& p2, sf::Vector2i& p3, rgb& colour) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1), sf::Color(colour.r, colour.g, colour.b)),
            sf::Vertex(sf::Vector2f(p2), sf::Color(colour.r, colour.g, colour.b)),
            sf::Vertex(sf::Vector2f(p3), sf::Color(colour.r, colour.g, colour.b))
    };

    window.draw(line, 3, sf::LineStrip);
}

int pointDivide(int& x1, int& x2,  float& m, float& n) {
    return int((m * x2 + n * x1) / (m + n));
}


int main()
{
    const int WIDTH = 800;
    const int HEIGHT = 800;
    const int r = 5;

    //Ceate the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Quadratic Bezier");

    sf::Vector2i p1(0, HEIGHT / 2);
    sf::Vector2i p2;
    sf::Vector2i p3(WIDTH, HEIGHT / 2);

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

        // draw everything here...
        // window.draw(...);

        //Draw points on window
        drawPoints(window, p1, p2, p3, r);

        //Draw segments between control points 1->2->3
        drawSegments(window, p1, p2, p3, colour);

        //Step through curve
        for (float t = 0; t <= 1; t += 0.05) {
            float m = t;
            float n = 1 - t;
            
            int p1x = pointDivide(p1.x, p2.x, m, n);
            int p1y = pointDivide(p1.y, p2.y, m, n);
            int p2x = pointDivide(p2.x, p3.x, m, n);
            int p2y = pointDivide(p2.y, p3.y, m, n);


            sf::Vector2i point1(p1x, p1y);
            sf::Vector2i point2(p2x, p2y);

            HSVtoRGB(t * 360, 100, 100, colour);

            drawSegments(window, point1, point1, point2, colour);
        }
        


        //End the current frame
        window.display();
    }
}