#ifndef MENU_H
#define MENU_H

#include "mapa.h"
#include "Nearest_Neighbor.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include "Voronoi.h"
#include "Convex_Hull.h"
#include "Intersection.h"
#include "Point_In_Polygon.h"
#include "Range_Search.h"

using namespace sf; using namespace std;

int menu(RenderWindow& window, const vector<Vector2f>& pts, Font font) {

    //Distintos votones que permiten ingresar a las diferentes pantallas del programa
    RectangleShape intersectionButton(Vector2f(300, 160));
    intersectionButton.setFillColor(Color(85, 52, 43));
    intersectionButton.setPosition(90, 530);

    RectangleShape convexButton(Vector2f(300, 160));
    convexButton.setFillColor(Color(85, 52, 43));
    convexButton.setPosition(570, 530);

    RectangleShape PIPButton(Vector2f(300, 160));
    PIPButton.setFillColor(Color(85, 52, 43));
    PIPButton.setPosition(1050, 530);

    RectangleShape rangeButton(Vector2f(300, 160));
    rangeButton.setFillColor(Color(85, 52, 43));
    rangeButton.setPosition(1530, 530);

    RectangleShape nearestNeighborButton(Vector2f(300, 160));
    nearestNeighborButton.setFillColor(sf::Color(85, 52, 43));
    nearestNeighborButton.setPosition(170, 860);

    RectangleShape voronoiButton(Vector2f(300, 160));
    voronoiButton.setFillColor(Color(85, 52, 43));
    voronoiButton.setPosition(810, 860);

    RectangleShape salirButtonMenu(Vector2f(300, 160));
    salirButtonMenu.setFillColor(Color(85, 52, 43));
    salirButtonMenu.setPosition(1450, 860);

    RectangleShape salirButtonOpc(Vector2f(300, 160));
    salirButtonOpc.setFillColor(Color(25, 25, 32));
    salirButtonOpc.setPosition(1810, 1000);


	//diferentes textos que indican las instrucciones para cada una de las pantallas
    sf::Text titulo("Seleccione una de las opciones presentes.", font, 60);
    titulo.setFillColor(sf::Color(85, 52, 43));
    titulo.setStyle(sf::Text::Bold);
    titulo.setPosition(350, 130);

    sf::Text subtitulo("Cada una de ellas representa una función distinta que podemos encontrar en un S.I.G.", font, 40);
    subtitulo.setFillColor(sf::Color(85, 52, 43));
    subtitulo.setStyle(sf::Text::Bold);
    subtitulo.setPosition(150, 200);

    sf::Text intersectionText("Interseccion de aristas", font, 24);
    intersectionText.setFillColor(sf::Color::White);
    intersectionText.setStyle(sf::Text::Bold);
    intersectionText.setPosition(100, 586);

    sf::Text convexText("Convex Hull", font, 24);
    convexText.setFillColor(sf::Color::White);
    convexText.setStyle(sf::Text::Bold);
    convexText.setPosition(650, 586);

    sf::Text PIPtext("Punto en Polígono", font, 24);
    PIPtext.setFillColor(sf::Color::White);
    PIPtext.setStyle(sf::Text::Bold);
    PIPtext.setPosition(1100, 586);

    sf::Text rangeText("Busqueda por rango", font, 24);
    rangeText.setFillColor(sf::Color::White);
    rangeText.setStyle(sf::Text::Bold);
    rangeText.setPosition(1560, 586);

    sf::Text nearestNeighborText("Vecino mas Proximo", font, 24);
    nearestNeighborText.setFillColor(sf::Color::White);
    nearestNeighborText.setStyle(sf::Text::Bold);
    nearestNeighborText.setPosition(190, 916);

    sf::Text voronoiText("Division por Voronoi", font, 24);
    voronoiText.setFillColor(sf::Color::White);
    voronoiText.setStyle(sf::Text::Bold);
    voronoiText.setPosition(830, 916);

    sf::Text exitText("Salir", font, 24);
    exitText.setFillColor(sf::Color::White);
    exitText.setStyle(sf::Text::Bold);
    exitText.setPosition(1550, 916);

    Text salirText("Salir", font, 40);
    salirText.setFillColor(sf::Color::White );
    salirText.setStyle(sf::Text::Bold);
    salirText.setPosition(1810, 1000);

    //diferentes textos de instrucciones que indican que hacer en cada pantalla al usuario
    sf::Text intersectionInstructionText("Puede dibujar distintas aristas entre los puntos ingresados apretando la tecla 'A' de su teclado", font, 24);
    intersectionInstructionText.setFillColor(sf::Color::White);
    intersectionInstructionText.setStyle(sf::Text::Bold);
    intersectionInstructionText.setPosition(0, 0);

    sf::Text convexInstructionText("Presione la tecla 'H' en su teclado para activar el funcionamiento del 'Convex Hull'", font, 24);
    convexInstructionText.setFillColor(sf::Color::White);
    convexInstructionText.setStyle(sf::Text::Bold);
    convexInstructionText.setPosition(0, 0);

    sf::Text PIPInstructionText1("Puede generar los vértices del polígono apretando el 'Click Izquierdo' de su ratón.", font, 24);
    PIPInstructionText1.setFillColor(sf::Color::White);
    PIPInstructionText1.setStyle(sf::Text::Bold);
    PIPInstructionText1.setPosition(0, 0);

    sf::Text PIPInstructionText2("Una vez ha generado todos los vertices del polígono, apriete el 'Click Derecho' y el polígono se completará", font, 24);
    PIPInstructionText2.setFillColor(sf::Color::White);
    PIPInstructionText2.setStyle(sf::Text::Bold);
    PIPInstructionText2.setPosition(0, 30);

    sf::Text rangeInstructionText1("Puede dibujar la región en la cual desea hacer zoom, manteniendo apretado la tecla 'Shift' y el 'Click Izquierdo' mientras arrastra el cursor por la pantalla", font, 24);
    rangeInstructionText1.setFillColor(sf::Color::White);
    rangeInstructionText1.setStyle(sf::Text::Bold);
    rangeInstructionText1.setPosition(0, 0);

    sf::Text rangeInstructionText2("Una vez se realiza el zoom, apriete la tecla 'R' para regresar al zoom original", font, 24);
    rangeInstructionText2.setFillColor(sf::Color::White);
    rangeInstructionText2.setStyle(sf::Text::Bold);
    rangeInstructionText2.setPosition(0, 30);
    
    //ciclo principal de ejecucion de la pantalla principal, y casos de cambio de pantalla
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {

                if (intersectionButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    Intersection_Window(window, salirButtonOpc, font, salirText, pts, intersectionInstructionText);
                }

                if (convexButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    Convex_Hull_Window(window, salirButtonOpc, font, salirText, convexInstructionText, pts);
                }

                if (PIPButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    Point_in_Polygon_Window(window, salirButtonOpc, font, salirText, pts, PIPInstructionText1, PIPInstructionText2);
                }
                
                if (rangeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    drawRangeSearch(window, salirButtonOpc, font, salirText, pts, rangeInstructionText1, rangeInstructionText2);
                }

                if (nearestNeighborButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    drawNearestNeighbor(window, salirButtonOpc, font, salirText, pts);
                }

                if (voronoiButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    drawVoronoiDiagram(window, salirButtonOpc, font, salirText, pts);
                }

                if (salirButtonMenu.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    window.close();
                }
            }
        }
        window.clear(sf::Color(206, 159, 111));

        window.draw(titulo), window.draw(subtitulo);
        window.draw(intersectionButton), window.draw(intersectionText);
        window.draw(convexButton), window.draw(convexText);
        window.draw(PIPButton), window.draw(PIPtext);
        window.draw(rangeButton), window.draw(rangeText);
        window.draw(nearestNeighborButton), window.draw(nearestNeighborText);
        window.draw(voronoiButton), window.draw(voronoiText);
        window.draw(salirButtonMenu), window.draw(exitText);
        window.display();
    }
    return 0;
}

#endif //MENU_H