/*
 *    CISP 400 Chaos Game !!!
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int POINTS_PER_FRAME = 10,
          MAX_POINTS       = 50000;

/* SFML stuff */
using namespace sf;
Color generate_random_color();
void draw_points(RenderWindow& scene, std::vector<Vector2f> points);


int main() {
    srand(time(0));

    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Chaos Game !!!", Style::Default);
    std::vector<Vector2f> vertices, points;

    while (window.isOpen()) {
        Event event;
        while(window.pollEvent(event)) {
            if (event.type == Event::Closed) { window.close(); }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                std::cout << "(" << event.mouseButton.x << ", " << event.mouseButton.y
                  << std::endl;

                if (vertices.size() < 3) {
                    vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                } else if (points.size() == 0){
                    points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
        if (points.size() > 0) {
            for (int i = 0; i < POINTS_PER_FRAME; i++) {
                int index = rand() % vertices.size();

                double x_pos = ( (points.back().x + vertices.at(index).x) / 2.0 );
                double y_pos = ( (points.back().y + vertices.at(index).y) / 2.0 );

                points.push_back(Vector2f(x_pos, y_pos));
            }
        }

        window.clear();
        draw_points(window, vertices);
        draw_points(window, points);
        window.display();
    }

    return 0;
}

Color generate_random_color() {

    int red   = rand() % 256,
        green = rand() % 256,
        blue  = rand() % 256;

    /* There is an alpha channel for transparancy but nahhh */
    return Color(red, green, blue);
}

void draw_points(RenderWindow& scene, std::vector<Vector2f> points) {
    for (size_t i = 0; i < points.size(); i++) {
        CircleShape point(5);
        point.setPosition(Vector2f(points.at(i).x, points.at(i).y));
        point.setFillColor(generate_random_color());
        scene.draw(point);
    }
}
