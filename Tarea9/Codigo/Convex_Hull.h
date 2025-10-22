#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

// hull_demo.cpp — Convex Hull (monotone chain) con SFML 2.6.x
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
using namespace std; using namespace sf;

constexpr float EPSHull = 1e-6f;
constexpr float RHull = 4.f;

static float crossHull(const Vector2f& a, const Vector2f& b, const Vector2f& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegmentHull(const Vector2f& a, const Vector2f& b, const Vector2f& p) {
    if (fabs(crossHull(a, b, p)) > 1e-5f) return false;
    return min(a.x, b.x) - EPSHull <= p.x && p.x <= max(a.x, b.x) + EPSHull &&
        min(a.y, b.y) - EPSHull <= p.y && p.y <= max(a.y, b.y) + EPSHull;
}

// --- núcleo geométrico ---
vector<Vector2f> convexHull(vector<Vector2f> p) {
    if (p.size() <= 1) return p;
    sort(p.begin(), p.end(),
        [](auto& a, auto& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
    vector<Vector2f> lo, up;
    for (auto& q : p) { // cadena inferior
        while (lo.size() >= 2 && crossHull(lo[lo.size() - 2], lo.back(), q) <= 0) lo.pop_back();
        lo.push_back(q);
    }
    for (int i = (int)p.size() - 1; i >= 0; --i) { // cadena superior
        auto q = p[i];
        while (up.size() >= 2 && crossHull(up[up.size() - 2], up.back(), q) <= 0) up.pop_back();
        up.push_back(q);
    }
    lo.pop_back(); up.pop_back();
    lo.insert(lo.end(), up.begin(), up.end());
    return lo;
}

int Convex_Hull_Window(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, Text convexInstructionText, const vector<Vector2f>& pts) {
    
    vector<Vector2f> H;  // puntos y hull
    const float R = 4.f;  // radio de los puntos

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
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::H) H = convexHull(pts);
            }
        }

        // --- dibujo ---
        win.clear(Color(25, 25, 32));
        win.draw(salirButton), win.draw(salirText), win.draw(convexInstructionText);

        // puntos
        for (const auto& p : pts) {
            CircleShape c(R); c.setOrigin(R, R); c.setPosition(p);
            c.setFillColor(Color(180, 180, 180));
            win.draw(c);
        }

        // hull (líneas naranjas)
        if (H.size() >= 2) {
            for (size_t i = 0; i < H.size(); ++i) {
                Vector2f a = H[i], b = H[(i + 1) % H.size()];
                Vertex L[2] = { Vertex(a, Color(255,120,0)),
                                    Vertex(b, Color(255,120,0)) };
                win.draw(L, 2, sf::Lines);
            }
        }

        /*for (const auto& p : pts) {
            win.draw(salirButton), win.draw(salirText);
        }*/

        win.display();
    }
    return 0;
}

#endif // CONVEX_HULL_H