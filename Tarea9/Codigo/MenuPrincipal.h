#include <SFML/Graphics.hpp>
#include <string>
#include "menu.h"
#include "mapa.h"
#include <iostream>
using namespace sf; using namespace std;

int menuInicio(RenderWindow& window, bool menuPrincipal, Font font) {

	RectangleShape salirButton(Vector2f(400, 160));
	salirButton.setFillColor(Color(85, 52, 43));
	salirButton.setPosition(1200, 580);

	RectangleShape comenzarButton(Vector2f(400, 160));
	comenzarButton.setFillColor(Color(85, 52, 43));
	comenzarButton.setPosition(320, 580);

	sf::Text tituloText("¡Bienvenido al visualizador de mapas de R.O.S!", font, 60);
	tituloText.setFillColor(sf::Color(85, 52, 43));
	tituloText.setPosition(325, 80);

	sf::Text salirText("Salir del Programa", font, 40);
	salirText.setFillColor(sf::Color::White);
	salirText.setPosition(1240, 640);

	sf::Text comenzarText("Comenzar", font, 40);
	comenzarText.setFillColor(sf::Color::White);
	comenzarText.setPosition(430, 640);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::MouseButtonPressed) {

				if (menuPrincipal == true && comenzarButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					mapa(window, menuPrincipal, font);
				}
				if (menuPrincipal == true && salirButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					window.close();
				}
			}
		}

		window.clear(sf::Color(206, 159, 111));
		window.draw(tituloText), window.draw(salirButton), window.draw(comenzarButton), window.draw(salirText), window.draw(comenzarText);
		window.display();
	}
	return 0;
}