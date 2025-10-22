#ifndef MAPA_H
#define MAPA_H

#include "menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
using namespace sf;
using namespace std;

int menu(RenderWindow& window, const vector<Vector2f>& pts, Font font);


int mapa(RenderWindow& window, bool menuPrincipal, Font font) {
	
    Text instructionText("Ingrese, mediante presion del click izquierdo del mouse, los puntos de interés en el mapa vacío.", font, 24);
    instructionText.setFillColor(Color::White);
    instructionText.setPosition(0, 0);

    Text enterText("Al ingresar todos los puntos de interes, presione la tecla 'Enter'", font, 24);
    enterText.setFillColor(Color::White);
    enterText.setPosition(0, 30);

    Text exitText("Presione 'Escape' para salir del programa", font, 24);
    exitText.setFillColor(Color::White);
    exitText.setPosition(0, 60);

    vector<Vector2f> pts;

    const float R = 4.f;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {
                pts.push_back((Vector2f) Mouse::getPosition(window));
            }

            if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)) {
                window.setActive(false);
                menu(window, pts, font);
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }

        window.clear(Color(25, 25, 32));

        for (const auto& p : pts) {
            CircleShape c(R); c.setOrigin(R, R); c.setPosition(p);
            c.setFillColor(Color(180, 180, 180));
            window.draw(c);
        }

        window.draw(instructionText), window.draw(enterText), window.draw(exitText);
        window.display();
    } 
    
    return 0;
}

#endif // MAPA_H