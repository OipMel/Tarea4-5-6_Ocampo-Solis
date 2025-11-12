#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
using namespace std;

struct registro {
    string nombre, apellido, diagnostico;
    int edad, nivel_emergencia;
};

class heap {
    vector<registro> cola;
public:
    vector<int> nodosIntercambiados;

    int gettamaño() { return cola.size(); }
    registro getRegistro(int i) { return cola[i]; }

    void heapmin(registro r) {
        cola.push_back(r);
        nodosIntercambiados.clear();
        int hijo = gettamaño() - 1;
        int padre = (hijo - 1) / 2;
        while (hijo > 0 && cola[hijo].nivel_emergencia < cola[padre].nivel_emergencia) {
            swap(cola[hijo], cola[padre]);
            nodosIntercambiados.push_back(hijo);
            nodosIntercambiados.push_back(padre);
            hijo = padre;
            padre = (hijo - 1) / 2;
        }
    }

    void reacomodar(int i) {
        int menor = i;
        int izq = 2 * i + 1, der = 2 * i + 2;
        if (izq < gettamaño() && cola[izq].nivel_emergencia < cola[menor].nivel_emergencia) menor = izq;
        if (der < gettamaño() && cola[der].nivel_emergencia < cola[menor].nivel_emergencia) menor = der;
        if (menor != i) {
            swap(cola[i], cola[menor]);
            nodosIntercambiados.push_back(i);
            nodosIntercambiados.push_back(menor);
            reacomodar(menor);
        }
    }

    void sacar_raiz() {
        if (cola.empty()) return;
        nodosIntercambiados.clear();
        cola[0] = cola.back();
        cola.pop_back();
        reacomodar(0);
    }
};

void dibujarHeap(sf::RenderWindow& window, heap& h, sf::Font& font) {
    int n = h.gettamaño();
    vector<sf::Vector2f> posiciones(n);

    for (int i = 0; i < n; ++i) {
        int nivel = log2(i + 1);
        int maxNodos = pow(2, nivel);
        float espacio = 1000.0f / (maxNodos + 1);
        float x = espacio * ((i - pow(2, nivel) + 1) + 1);
        float y = 80 + nivel * 80;
        posiciones[i] = sf::Vector2f(x, y);
    }

    for (int i = 0; i < n; ++i) {
        int padre = (i - 1) / 2;
        if (i > 0) {
            sf::Vertex linea[] = {
                sf::Vertex(posiciones[i], sf::Color::White),
                sf::Vertex(posiciones[padre], sf::Color::White)
            };
            window.draw(linea, 2, sf::Lines);
        }
    }

    for (int i = 0; i < n; ++i) {
        sf::CircleShape nodo(25);
        nodo.setOrigin(25, 25);
        nodo.setPosition(posiciones[i]);

        if (i == 0)
            nodo.setFillColor(sf::Color::Blue);
        else if (find(h.nodosIntercambiados.begin(), h.nodosIntercambiados.end(), i) != h.nodosIntercambiados.end())
            nodo.setFillColor(sf::Color::Red);
        else
            nodo.setFillColor(sf::Color::Green);

        window.draw(nodo);

        sf::Text texto;
        texto.setFont(font);
        texto.setCharacterSize(16);
        texto.setFillColor(sf::Color::White);
        texto.setString(to_string(h.getRegistro(i).nivel_emergencia));
        texto.setPosition(posiciones[i].x - 10, posiciones[i].y - 10);
        window.draw(texto);
    }

    for (int i = 0; i < n; ++i) {
        sf::RectangleShape rect(sf::Vector2f(40, 40));
        rect.setPosition(50 + i * 45, 520);
        rect.setFillColor(sf::Color::Green);
        rect.setOutlineColor(sf::Color::Magenta);
        rect.setOutlineThickness(2);
        window.draw(rect);

        sf::Text texto;
        texto.setFont(font);
        texto.setCharacterSize(14);
        texto.setFillColor(sf::Color::Black);
        texto.setString(to_string(h.getRegistro(i).nivel_emergencia));
        texto.setPosition(rect.getPosition().x + 10, rect.getPosition().y + 10);
        window.draw(texto);
    }
}

void visualizacion(sf::RenderWindow& window, heap& h, sf::Font& font) {
    vector<string> campos = { "Nombre", "Apellido", "Diagnostico", "Edad", "Nivel Emergencia" };
    vector<string> valores(campos.size(), "");
    int campoActual = 0;
    bool ingresoActivo = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (ingresoActivo && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!valores[campoActual].empty())
                        valores[campoActual].pop_back();
                }
                else if (event.text.unicode == '\r') {
                    campoActual++;
                    if (campoActual >= campos.size()) {
                        ingresoActivo = false;
                        campoActual = 0;
                        try {
                            registro r;
                            r.nombre = valores[0];
                            r.apellido = valores[1];
                            r.diagnostico = valores[2];
                            r.edad = stoi(valores[3]);
                            r.nivel_emergencia = stoi(valores[4]);
                            h.heapmin(r);
                        }
                        catch (...) {}
                        valores = vector<string>(campos.size(), "");
                    }
                }
                else if (event.text.unicode < 128) {
                    valores[campoActual] += static_cast<char>(event.text.unicode);
                }
            }

            if (!ingresoActivo && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    h.sacar_raiz();
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    ingresoActivo = true;
                }
            }
        }

        window.clear(sf::Color::Black);

        if (ingresoActivo) {
            for (int i = 0; i < campos.size(); ++i) {
                sf::Text texto;
                texto.setFont(font);
                texto.setCharacterSize(20);
                texto.setFillColor(sf::Color::White);
                stringstream ss;
                ss << "Ingrese " << campos[i] << ": " << valores[i];
                texto.setString(ss.str());
                texto.setPosition(50, 50 + i * 40);
                window.draw(texto);
            }

            sf::Text instruccion;
            instruccion.setFont(font);
            instruccion.setCharacterSize(16);
            instruccion.setFillColor(sf::Color::White);
            instruccion.setString("Presione ENTER para pasar al siguiente campo");
            instruccion.setPosition(50, 50 + campos.size() * 40 + 10);
            window.draw(instruccion);
        }
        else {
            dibujarHeap(window, h, font);
        }

        window.display();
    }
}

int main() {
    heap h;
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Sistema de Atención Prioritaria");
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }
    visualizacion(window, h, font);
    return 0;
}
