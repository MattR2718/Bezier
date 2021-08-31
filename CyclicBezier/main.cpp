#include "Curve.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
    const int WIDTH = 800;
    const int HEIGHT = 800;
    const int r = WIDTH / 2;

    const int curveNum = 10;
    Curve curves[curveNum];

    //Ceate the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cyclic Bezier");

    //Curve curve();

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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                for (auto& curve : curves) {
                    curve.DOTS = !curve.DOTS;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                for (auto& curve : curves) {
                    curve.COLOUR = !curve.COLOUR;
                }
            }
        }

        window.setFramerateLimit(60);

        //Clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        sf::CircleShape edge(r);
        edge.setFillColor(sf::Color::Black);
        edge.setOutlineColor(sf::Color::White);
        edge.setOutlineThickness(1.0f);
        window.draw(edge);

        //Draw points on window
        //drawPoints(window, p1, p2, p3);

        //Draw segments between control points 1->2->3
        //drawLine(window, p1, p2, white);
        //drawLine(window, p2, p3, white);
        for (auto& curve : curves) {
            curve.update(window);
            curve.render(window);
        }

        //End the current frame
        window.display();
    }
}
