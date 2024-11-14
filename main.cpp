#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace sf;
using namespace std;

const int POINTS_PER_FRAME = 1000,
          MAX_POINTS = 25000;

Color generate_random_color();
CircleShape create_point(Vector2f position, Color color);

int main() {
    srand(time(0));
    int loaded_points = 0;

    Font font;
    if (!font.loadFromFile("FiraSans-Regular.ttf")) {
        cout << "Failed to load font." << endl;
    }

    Text text;
    text.setFont(font);
    text.setString("Left-click 3 times to plot your vertices, then a 4th time to plot the starting point.");
    text.setCharacterSize(30);
    text.setFillColor(Color::White);

    VideoMode screen(1920, 1080);
    RenderWindow window(screen, "Chaos Game !!!!", Style::Default);
    vector<CircleShape> vertices, points;

    while (window.isOpen()) {
        Event event;
        Color color = generate_random_color();

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { window.close(); }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                cout << "(" << event.mouseButton.x << ", " << event.mouseButton.y << ")"
                  << endl;
                
                if (vertices.size() < 3) {
                    Vector2f position(event.mouseButton.x, event.mouseButton.y);
                    vertices.push_back(create_point(position, color));
                } else if (points.size() == 0) {
                    Vector2f position(event.mouseButton.x, event.mouseButton.y);
                    points.push_back(create_point(position, color));
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
        if (points.size() > 0) { 

            if (loaded_points == MAX_POINTS) {
                vector<CircleShape> new_points(points.begin() + 1000, points.end());
                points.swap(new_points);

                loaded_points -= POINTS_PER_FRAME;
            }

            for (int i = 0; i < POINTS_PER_FRAME; i++) {
                int index = rand() % vertices.size();

                CircleShape vertex = vertices.at(index),
                            point = points.back();

                double x_pos = ( (point.getPosition().x + vertex.getPosition().x) / 2.0 ),
                       y_pos = ( (point.getPosition().y + vertex.getPosition().y) / 2.0 );

                Vector2f position(x_pos, y_pos);
                points.push_back(create_point(position, color));
            }

            loaded_points += POINTS_PER_FRAME;
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        window.clear();
        if (points.size() == 0) {

            FloatRect bounds = text.getLocalBounds();
            text.setPosition((window.getSize().x - bounds.width) / 2 - bounds.left, text.getPosition().y);

            window.draw(text);
        }
        for (CircleShape& vertex : vertices) { window.draw(vertex); }

        for (CircleShape& point : points) {
            window.draw(point);
        }
        window.display();
    }

    return 0;
}

Color generate_random_color() {
    int red   = rand() % 256,
        green = rand() % 256,
        blue  = rand() % 256;

    return Color(red, green, blue);
}

CircleShape create_point(Vector2f position, Color color) {
    CircleShape point(2);

    point.setPosition(position);
    point.setFillColor(color);

    return point;
}
