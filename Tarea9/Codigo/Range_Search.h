#ifndef RANGE_SEARCH_H
#define RANGE_SEARCH_H

#include "menu.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
using namespace std; using namespace sf;

int drawRangeSearch(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<Vector2f>& pts, Text rangeInstructionText1, Text rangeInstructionText2) {
    bool dragging = false;
    bool zoomed = false;
    Vector2f a, b; // esquinas opuestas del rectángulo
    const float R = 4.f;

    sf::View view = win.getDefaultView();

    while (win.isOpen()) {
        Event e;
        while (win.pollEvent(e)) {
            if (e.type == Event::Closed) win.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::R) {
                    view = win.getDefaultView(); // <-- Restablece el zoom
                    win.setView(view);
                    a = b = {}; // Borra el rectángulo
                    dragging = false;
                    zoomed = false;
                }

            }

            if (e.type == Event::MouseButtonReleased &&
                e.mouseButton.button == Mouse::Left) {
                dragging = false;
                if (a != b) {
                    float x1 = min(a.x, b.x), x2 = max(a.x, b.x);
                    float y1 = min(a.y, b.y), y2 = max(a.y, b.y);
                    sf::FloatRect zoomRect(x1, y1, x2 - x1, y2 - y1);
                    if (zoomRect.width > 10 && zoomRect.height > 10) { // Evita zooms accidentales
                        view.setCenter(zoomRect.left + zoomRect.width / 2, zoomRect.top + zoomRect.height / 2);
                        view.setSize(zoomRect.width, zoomRect.height);
                        win.setView(view);
                        zoomed = true;
                    }
                }
            }

            if (e.type == Event::MouseButtonPressed &&
                e.mouseButton.button == Mouse::Left && salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                win.setActive(false);
                menu(win, pts, font);
            }

            if (e.type == Event::MouseButtonPressed &&
                e.mouseButton.button == Mouse::Left && zoomed == false) {
                // Si estás manteniendo SHIFT, inicia rectángulo; si no, agrega punto
                if (Keyboard::isKeyPressed(Keyboard::LShift) ||
                    Keyboard::isKeyPressed(Keyboard::RShift)) {
                    dragging = true;
                    a = (Vector2f)Mouse::getPosition(win);
                    b = a;
                }
                else if (salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y) ){
                    win.setActive(false);
                    menu(win, pts, font);
                }
            }

            if (e.type == Event::MouseMoved && dragging) {
                b = (Vector2f)Mouse::getPosition(win);
            }
        }

        // ---- Dibujo ----
        win.clear(Color(25, 25, 32));

        // Calcular AABB actual (si existe)
        FloatRect box;
        bool hasBox = (dragging || a != b);
        if (hasBox) {
            float x1 = min(a.x, b.x), x2 = max(a.x, b.x);
            float y1 = min(a.y, b.y), y2 = max(a.y, b.y);
            box = { x1, y1, x2 - x1, y2 - y1 };

            RectangleShape rect;
            rect.setPosition(x1, y1);
            rect.setSize({ x2 - x1, y2 - y1 });
            rect.setFillColor(Color(100, 100, 220, 40));
            rect.setOutlineThickness(1.f);
            rect.setOutlineColor(Color(100, 100, 220));
            win.draw(rect);
        }

        // Dibujar puntos (celeste si están dentro del AABB)
        for (const auto& p : pts) {
            bool inside = hasBox && box.contains(p);
            CircleShape c(R); c.setOrigin(R, R); c.setPosition(p);
            c.setFillColor(inside ? Color(100, 200, 255) : Color(180, 180, 180));
            win.draw(c);
        }

        if (zoomed == false)
			win.draw(rangeInstructionText1), win.draw(rangeInstructionText2);
        win.draw(salirButton), win.draw(salirText);
        win.display();
    }
    return 0;
}

#endif // RANGE_SEARCH_H