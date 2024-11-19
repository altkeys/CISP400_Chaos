#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace sf;
using namespace std;

const int POINTS_PER_FRAME = 1000;

Color generate_random_color() {
    int red   = rand() % 256,
        green = rand() % 256,
        blue  = rand() % 256;

    return Color(red, green, blue);
}

CircleShape create_point(float x, float y, Color color = Color::White) {
    CircleShape point(1);

    point.setPosition(x, y);
    point.setFillColor(color);

    return point;
}

float calculate_scalar(int vertices) {
    float scalar = 1.0;
    int bound = floor(vertices / 4);

    for (int k = 1; k < bound + 1; k++) {
        scalar += cos((2 * M_PI * k) / vertices);
    }

    return 1.0 / (2.0 * scalar);
}

void update_frame(vector<CircleShape>& points, const vector<CircleShape>& vertices) {
    Color color = generate_random_color();
    int previous = -1;

    for (int i = 0; i < POINTS_PER_FRAME; i++) {
        int index = rand() % vertices.size();

        if (vertices.size() == 4) {
            while (index == previous) { index = rand() % vertices.size(); }
            previous = index;
        }

        Vector2f vertex = vertices.at(index).getPosition(),
                    point  = points.back().getPosition();

        float scalar = calculate_scalar(vertices.size()),
              x_pos = vertex.x + ((point.x - vertex.x) * scalar),
              y_pos = vertex.y + ((point.y - vertex.y) * scalar);

        points.push_back(create_point(x_pos, y_pos, color));
    }
}

int main() {
    srand(time(0));

    Font font;
    if (!font.loadFromFile("./FiraSans-Regular.ttf")) {
        cout << "Failed to load font." << endl;
    }

    Text text("Left-click to select 3 vertices then select a starting point.", font, 30);
    text.setFillColor(Color::White);

    Music music;
    if (!music.openFromFile("01 Title Theme.mp3")) {
        cout << "Failed to load easter egg music." << endl;
    }

    VideoMode screen(1920, 1080);
    RenderWindow window(screen, "Chaos Game !!!!!", Style::Default);
    vector<CircleShape> vertices, points;

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { window.close(); }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    float x = event.mouseButton.x,
                          y = event.mouseButton.y;
                    
                    cout << "(" << x << ", " << y << ")" << endl;

                    if (vertices.size() < 6) { vertices.push_back(create_point(x, y)); }
                    else if (points.size() == 0) { 
                        if (vertices.size() == 3) { music.play(); }
                        points.push_back(create_point(x, y));
                    }
                }
            }
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }

        window.clear();
        if (points.size() > 0) { update_frame(points, vertices); }

        if (points.size() == 0) {
            FloatRect bounds = text.getLocalBounds();
            text.setPosition((window.getSize().x - bounds.width) / 2 - bounds.left, text.getPosition().y);
            window.draw(text);
        }
        for (CircleShape& vertex : vertices) { window.draw(vertex); }
        for (CircleShape& point : points) { window.draw(point); }
        window.display();
    }
    return 0;
}