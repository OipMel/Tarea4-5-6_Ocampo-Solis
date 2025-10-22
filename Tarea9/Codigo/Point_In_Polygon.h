
// pip_demo.cpp — main mínimo para Punto-en-Polígono con SFML 2.6.x
#include "menu.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

constexpr float EPSPIP = 1e-6f;
constexpr float RPIP = 4.f;

static float crossPIP(const Vector2f& a, const Vector2f& b, const Vector2f& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegmentPIP(const Vector2f& a, const Vector2f& b, const Vector2f& p) {
    if (fabs(crossPIP(a, b, p)) > 1e-5f) return false;
    return min(a.x, b.x) - EPSPIP <= p.x && p.x <= max(a.x, b.x) + EPSPIP &&
        min(a.y, b.y) - EPSPIP <= p.y && p.y <= max(a.y, b.y) + EPSPIP;
}


bool pointInPolygon(const vector<Vector2f>& poly, const Vector2f& p) {
    bool in = false; int n = (int)poly.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        const auto& a = poly[j], & b = poly[i];
        bool hit = ((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y + 1e-12f) + a.x);
        if (hit) in = !in;
    }
    for (int i = 0; i < n; i++) if (onSegmentPIP(poly[i], poly[(i + 1) % n], p)) return true;
    return in;
}

int Point_in_Polygon_Window(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<Vector2f>& pts, Text PIPInstructionText1, Text PIPInstructionText2) {
    
    vector<Vector2f> poly;      // vertices del poligono
    bool closed = false;          // poligono cerrado?

    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) win.close();

            if (e.type == Event::MouseButtonPressed &&
                e.mouseButton.button == Mouse::Left) {
                if (salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    win.setActive(false);
                    menu(win, pts, font);
                }
            }

            if (e.type == Event::MouseButtonPressed) {
                Vector2f m = (Vector2f)Mouse::getPosition(win);
                if (e.mouseButton.button == Mouse::Left) {
                    if (!closed) poly.push_back(m);   // agregar vertice
                }



                if (e.mouseButton.button == Mouse::Right) {
                    if (!closed && poly.size() >= 3) closed = true; // cerrar poligono
                }
            }
        }

        // ---- Dibujo ----
        win.clear(Color(25, 25, 32));
        win.draw(salirButton), win.draw(salirText), win.draw(PIPInstructionText1), win.draw(PIPInstructionText2);

        // aristas del poligono
        for (size_t i = 0; i + 1 < poly.size(); ++i) {
            Vertex L[2] = { Vertex(poly[i], Color::Yellow),
                              Vertex(poly[i + 1], Color::Yellow) };
            win.draw(L, 2, Lines);
        }
        if (closed && poly.size() >= 3) {
            Vertex L[2] = { Vertex(poly.back(), Color::Yellow),
                              Vertex(poly.front(), Color::Yellow) };
            win.draw(L, 2, Lines);
        }
        // vertices
        for (auto& p : poly) {
            CircleShape c(RPIP); c.setOrigin(RPIP, RPIP); c.setPosition(p);
            c.setFillColor(Color::Yellow); win.draw(c);
        }

        // puntos de consulta (verde si dentro/borde, rojo si fuera)
        for (auto& q : pts) {
            bool in = closed && pointInPolygon(poly, q);
            CircleShape c(RPIP); c.setOrigin(RPIP, RPIP); c.setPosition(q);
            c.setFillColor(in ? Color(0, 220, 120) : Color(220, 60, 60));
            win.draw(c);
        }

        for (const auto& p : pts) {
            win.draw(salirButton), win.draw(salirText);
        }

        win.display();
    }
    return 0;
}
