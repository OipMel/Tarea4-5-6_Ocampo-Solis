#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "menu.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std; using namespace sf;
using sf::Vector2f; using std::vector;

constexpr float EPSInt = 1e-6f;
constexpr float RInt = 4.f;

static float crossInt(const Vector2f& a, const Vector2f& b, const Vector2f& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegmentInt(const Vector2f& a, const Vector2f& b, const Vector2f& p) {
    if (fabs(crossInt(a, b, p)) > 1e-5f) return false;
    return min(a.x, b.x) - EPSInt <= p.x && p.x <= max(a.x, b.x) + EPSInt &&
        min(a.y, b.y) - EPSInt <= p.y && p.y <= max(a.y, b.y) + EPSInt;
}


bool segmentsIntersect(const Vector2f& a, const Vector2f& b,
    const Vector2f& c, const Vector2f& d, Vector2f* out = nullptr) {
    float c1 = crossInt(a, b, c), c2 = crossInt(a, b, d), c3 = crossInt(c, d, a), c4 = crossInt(c, d, b);
    // Caso general: están en lados opuestos en ambos tests
    if ((c1 * c2 < 0) && (c3 * c4 < 0)) {
        if (out) {
            Vector2f r{ b.x - a.x,b.y - a.y }, s{ d.x - c.x,d.y - c.y };
            float rxs = r.x * s.y - r.y * s.x;
            if (fabs(rxs) > EPSInt) {
                float t = ((c.x - a.x) * s.y - (c.y - a.y) * s.x) / rxs;
                *out = { a.x + t * r.x, a.y + t * r.y };
            }
        }
        return true;
    }
    // Colineales / extremos
    if (fabs(c1) < EPSInt && onSegmentInt(a, b, c)) { if (out) *out = c; return true; }
    if (fabs(c2) < EPSInt && onSegmentInt(a, b, d)) { if (out) *out = d; return true; }
    if (fabs(c3) < EPSInt && onSegmentInt(c, d, a)) { if (out) *out = a; return true; }
    if (fabs(c4) < EPSInt && onSegmentInt(c, d, b)) { if (out) *out = b; return true; }
    return false;
}

struct Seg { Vector2f a, b; };

int Intersection_Window(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<Vector2f>& pts, Text intersectionInstructionText) {
    vector<Seg> segs;
    vector<Vector2f> inters = pts;
    Vector2f tmp; bool haveFirst = false;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) win.close();

            if (e.type == sf::Event::MouseButtonPressed) {
                if (salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    win.setActive(false);
                    menu(win, pts, font);
                }
            }

            // Add this block to create a random segment when pressing 'A'
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::A) {
                if (pts.size() >= 2) {
                    std::uniform_int_distribution<> dis(0, pts.size() - 1);
                    int i = dis(gen), j = dis(gen);
                    while (j == i) j = dis(gen); // Ensure different points
                    segs.push_back({ pts[i], pts[j] });

                    // Recompute intersections
                    //.clear();
                    for (size_t m = 0; m < segs.size(); ++m)
                        for (size_t n = m + 1; n < segs.size(); ++n) {
                            Vector2f p;
                            if (segmentsIntersect(segs[m].a, segs[m].b, segs[n].a, segs[n].b, &p))
                                inters.push_back(p);
                        }
                }
            }

            if (e.type == Event::MouseButtonPressed &&
                e.mouseButton.button == Mouse::Left) {
                Vector2f m = (Vector2f)Mouse::getPosition(win);
                if (!haveFirst) { tmp = m; haveFirst = true; }
                else {
                    segs.push_back({ tmp, m });
                    haveFirst = false;
                    // recomputar todas las intersecciones (naive O(m^2))
                    inters.clear();
                    for (size_t i = 0; i < segs.size(); ++i)
                        for (size_t j = i + 1; j < segs.size(); ++j) {
                            Vector2f p;
                            if (segmentsIntersect(segs[i].a, segs[i].b, segs[j].a, segs[j].b, &p))
                                inters.push_back(p);
                        }
                }
            }
        }

        // ---- dibujo ----
        win.clear(Color(25, 25, 32));
        win.draw(salirButton), win.draw(salirText), win.draw(intersectionInstructionText);

        // segmentos existentes
        for (const auto& s : segs) {
            Vertex L[2] = { Vertex(s.a, Color(200,200,200)),
                                Vertex(s.b, sf::Color(200,200,200)) };
            win.draw(L, 2, Lines);
        }

        // vista previa del segmento en armado
        if (haveFirst) {
            Vector2f m = (Vector2f)Mouse::getPosition(win);
            Vertex L[2] = { Vertex(tmp, Color(120,180,255)),
                                Vertex(m, Color(120,180,255)) };
            win.draw(L, 2, Lines);
            CircleShape c(RInt); c.setOrigin(RInt, RInt); c.setPosition(tmp);
            c.setFillColor(Color(120, 180, 255)); win.draw(c);
        }

        // puntos de interseccion (rojos)
        for (const auto& p : inters) {
            CircleShape c(RInt); c.setOrigin(RInt, RInt); c.setPosition(p);
            c.setFillColor(Color::Red); win.draw(c);
        }

        for (const auto& p : inters) {
            win.draw(salirButton), win.draw(salirText);
        }

        win.display();
    }

    return 0;
}
#endif  //INTERSECTION_H
