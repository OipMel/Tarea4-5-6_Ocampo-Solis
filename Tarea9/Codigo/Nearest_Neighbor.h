#ifndef NN_H
#define NN_H

#include "menu.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <limits>

using namespace std; using namespace sf;
using sf::Vector2f; using std::vector;

int menu(RenderWindow& window, const vector<Vector2f>& pts, Font font);

float dist(const Vector2f& a, const Vector2f& b) {
    float dx = a.x - b.x, dy = a.y - b.y; return (dx * dx + dy * dy);
}

int nearestIdx(const vector<Vector2f>& pts, const Vector2f& q) {
    if (pts.empty()) return -1;
    float best = std::numeric_limits<float>::infinity(); int idx = -1;
    for (int i = 0; i < (int)pts.size(); ++i) {
        float d = dist(pts[i], q);
        if (d < best) { best = d; idx = i; }
    }
    return idx;
}

int drawNearestNeighbor(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<Vector2f>& pts) {
    const float R = 4.f;

    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) win.close();
            
            if (e.type == Event::MouseButtonPressed &&
                e.mouseButton.button == Mouse::Left && salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                win.setActive(false);
                menu(win, pts, font);
            }
         }
        

        // ---- Dibujo ----
        win.clear(Color(25, 25, 32));


        Vector2f q = (Vector2f)Mouse::getPosition(win);
        int idx = nearestIdx(pts, q);

        // dibujar puntos (verde si es el NN; gris si no)
        for (int i = 0; i < (int)pts.size(); ++i) {
            Color col = (i == idx ? Color(0, 255, 120) : Color(180, 180, 180));
            CircleShape c(R); c.setOrigin(R, R); c.setPosition(pts[i]); c.setFillColor(col);
            win.draw(c);
        }
        // línea desde q al más cercano
        
        if (idx != -1 && (!salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))) {
            Vertex L[2] = { Vertex(q, Color(0,255,120)),
            Vertex(pts[idx], Color(0,255,120)) };
            win.draw(L, 2, Lines);
        }
        
        win.draw(salirButton), win.draw(salirText);
        win.display();
    }
    return 0;
}

#endif //NN_H