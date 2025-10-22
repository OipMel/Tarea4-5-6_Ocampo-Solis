#ifndef Voronoi_h
#define Voronoi_h

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace std; using sf::Vector2f; using std::vector;

// Coordenadas enteras
struct coordenada {
    double x, y;
};

// Clase puntos
class puntos {
private:
    coordenada p;

public:
    puntos() {}
    puntos(const coordenada& c) : p(c) {}
    coordenada getCoordenada() const { return p; }
};

// Clase plano que contiene las semillas
class plano {
    vector<Vector2f> semillas;

public:
    // Constructor que acepta semillas externas
    plano(const vector<Vector2f>& pts) : semillas(pts) {}

    const vector<Vector2f>& getSemillas() const { return semillas; }

    void dibujito(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<puntos>& semillas, const vector<pair<coordenada, coordenada>>& fronteras) {
        
        int cantidadSemillas = semillas.size();
        vector<Vector2f> semillas_pantalla;
        const float R = 4.f;
        sf::ConvexShape regiones;

        for (const auto& punto : semillas) {
            coordenada c = punto.getCoordenada();
            semillas_pantalla.push_back(Vector2f(c.x, c.y));
        }
        while (win.isOpen()) {
            sf::Event e;
            while (win.pollEvent(e)) {
                if (e.type == sf::Event::Closed) win.close();
                if (e.type == Event::MouseButtonPressed &&
                    e.mouseButton.button == Mouse::Left && salirButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    win.setActive(false);
                    menu(win, semillas_pantalla, font);
                }
            }        

            win.clear(Color(25, 25, 32));
            
            for (const auto& pos : semillas_pantalla) {
                sf::CircleShape punto(R);
                punto.setFillColor(sf::Color(180, 180, 180));
                punto.setOrigin(R, R);
                punto.setPosition(pos);
                win.draw(punto);
            }
            for (const auto& f : fronteras) {
                sf::Vertex line[] = {
                    sf::Vertex(Vector2f(f.first.x, f.first.y), sf::Color::Blue),
                    sf::Vertex(Vector2f(f.second.x, f.second.y), sf::Color::Blue)
                };
                win.draw(line, 2, sf::Lines);
            }

			win.draw(salirButton), win.draw(salirText);
            win.display();
        }
    }
};

struct triangulos {
    coordenada V1, V2, V3;
};
struct aristas {
    coordenada a, b;
    bool operator==(const aristas& other) const {
        return ((a.x == other.a.x && a.y == other.a.y && b.x == other.b.x && b.y == other.b.y) ||
            (a.x == other.b.x && a.y == other.b.y && b.x == other.a.x && b.y == other.a.y));
    }
};

class segmentos {
    vector<aristas> aristas_triangulo;
public:
    void agregar_aristas(const triangulos& t) {
        aristas_triangulo.push_back({ t.V1, t.V2 });
        aristas_triangulo.push_back({ t.V2, t.V3 });
        aristas_triangulo.push_back({ t.V3, t.V1 });
    }
    vector<aristas> get_aristas() const { return aristas_triangulo; }
    void clear_aristas() { aristas_triangulo.clear(); }
};

class triangulo {
private:
    vector<triangulos> trianguloscreados;
    vector<triangulos> eliminartriangulos;
    coordenada superA, superB, superC;
public:
    segmentos s;
    void obtenerTrianguloEnglobador(const plano& h);
    void circulo(coordenada p);
    bool estaadentro(triangulos t, coordenada p);
    void crear_triangulo(coordenada p);
    void eliminar_triangulo(int i);
    void eliminarTriangulosSuper();
    void corregirOrientacion(triangulos& t);
    bool esCCW(const triangulos& t);
    const vector<triangulos>& getTriangulos() const { return trianguloscreados; }
};
// Triangulo englobador 
void triangulo::obtenerTrianguloEnglobador(const plano& h) {
    const vector<Vector2f>& semillas = h.getSemillas();
    if (semillas.empty()) return;

    double xmin = semillas[0].x;
    double xmax = xmin;
    double ymin = semillas[0].y;
    double ymax = ymin;

    for (const auto& punto : semillas) {
        coordenada c = { punto.x, punto.y };
        xmin = min(xmin, c.x);
        xmax = max(xmax, c.x);
        ymin = min(ymin, c.y);
        ymax = max(ymax, c.y);
    }

    int dx = xmax - xmin;
    int dy = ymax - ymin;
    int delta = max(dx, dy) * 20;
    // Supertriángulo muy grande que cubre todos los puntos
    superA = { xmin - delta, ymin - delta };
    superB = { xmax + delta, ymin - delta };
    superC = { (xmin + xmax) / 2, ymax + delta };
    triangulos t = { superA, superB, superC };
    corregirOrientacion(t);
    trianguloscreados.push_back(t);
}
// Verifica si el triángulo está orientado a sentido antihrario
bool triangulo::esCCW(const triangulos& t) {
    long long cross = (long long)(t.V2.x - t.V1.x) * (t.V3.y - t.V1.y)
        - (long long)(t.V2.y - t.V1.y) * (t.V3.x - t.V1.x);
    return cross > 0;
}
void triangulo::corregirOrientacion(triangulos& t) {
    if (!esCCW(t)) swap(t.V2, t.V3);
}
// Determina si un punto está dentro del círculo circunscrito
bool triangulo::estaadentro(triangulos t, coordenada p) {
    long long ax = t.V1.x - p.x, ay = t.V1.y - p.y;
    long long bx = t.V2.x - p.x, by = t.V2.y - p.y;
    long long cx = t.V3.x - p.x, cy = t.V3.y - p.y;

    long long det = (ax * ax + ay * ay) * (bx * cy - cx * by)
        - (bx * bx + by * by) * (ax * cy - cx * ay)
        + (cx * cx + cy * cy) * (ax * by - bx * ay);

    bool ccw = esCCW(t);
    if (ccw) return det > 0;
    else return det < 0;
}
// Eliminar triángulo
void triangulo::eliminar_triangulo(int i) {
    if (i >= 0 && i < trianguloscreados.size()) {
        triangulos tri = trianguloscreados[i];
        eliminartriangulos.push_back(tri);
        s.agregar_aristas(tri);
        trianguloscreados.erase(trianguloscreados.begin() + i);
    }
}
// Crear triángulos nuevos con los bordes
void triangulo::crear_triangulo(coordenada p) {
    vector<aristas> todas_aristas = s.get_aristas();
    vector<aristas> aristas_borde;
    for (const auto& arista : todas_aristas) {
        int count = count_if(todas_aristas.begin(), todas_aristas.end(),
            [&arista](const aristas& a) { return a == arista; });
        if (count == 1) aristas_borde.push_back(arista);
    }
    for (const auto& arista : aristas_borde) {
        triangulos nuevo = { arista.a, arista.b, p };
        corregirOrientacion(nuevo);
        trianguloscreados.push_back(nuevo);
    }
    eliminartriangulos.clear();
    s.clear_aristas();
}

void triangulo::circulo(coordenada p) {
    for (int i = 0; i < trianguloscreados.size();) {
        if (estaadentro(trianguloscreados[i], p))
            eliminar_triangulo(i);
        else
            i++;
    }
    crear_triangulo(p);
}
// Elimina los triángulos que contienen vértices del supertriángulo
void triangulo::eliminarTriangulosSuper() {
    auto esSuper = [this](const coordenada& v) {
        return (v.x == superA.x && v.y == superA.y) ||
            (v.x == superB.x && v.y == superB.y) ||
            (v.x == superC.x && v.y == superC.y);
        };
    vector<triangulos> filtrados;
    for (const auto& t : trianguloscreados) {
        if (!(esSuper(t.V1) || esSuper(t.V2) || esSuper(t.V3)))
            filtrados.push_back(t);
    }
    trianguloscreados = filtrados;
}

bool clipRayToBounds(const coordenada& start, const coordenada& dir, double width, double height, coordenada& endPoint) {
    double t_min = 0;
    double t_max = 1e6;

    double px = start.x, py = start.y;
    double dx = dir.x, dy = dir.y;

    if (dx != 0) {
        double t1 = (0 - px) / dx;
        double t2 = (width - px) / dx;
        t_min = max(t_min, min(t1, t2));
        t_max = min(t_max, max(t1, t2));
    }
    else if (px < 0 || px > width) return false;

    if (dy != 0) {
        double t3 = (0 - py) / dy;
        double t4 = (height - py) / dy;
        t_min = max(t_min, min(t3, t4));
        t_max = min(t_max, max(t3, t4));
    }
    else if (py < 0 || py > height) return false;

    if (t_min > t_max) return false;

    endPoint = { px + dx * t_max, py + dy * t_max };
    return true;
}

struct AristaVoronoi {
    coordenada a, b;
};

class Voronoi {
public:
    vector<coordenada> circ;
    vector<AristaVoronoi> aristas;
    void generarFronteras(const vector<triangulos>& triangulosDelaunay, vector<pair<coordenada, coordenada>>& fronteras) {
        fronteras.clear();

        struct AristaClave {
            coordenada a, b;
            bool operator<(const AristaClave& other) const {
                if (a.x != other.a.x) return a.x < other.a.x;
                if (a.y != other.a.y) return a.y < other.a.y;
                if (b.x != other.b.x) return b.x < other.b.x;
                return b.y < other.b.y;
            }
        };

        auto circuncentro = [](const triangulos& t) -> coordenada {
            double x1 = t.V1.x, y1 = t.V1.y;
            double x2 = t.V2.x, y2 = t.V2.y;
            double x3 = t.V3.x, y3 = t.V3.y;

            double d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
            if (fabs(d) < 1e-9) return { 0, 0 };

            double ux = ((x1 * x1 + y1 * y1) * (y2 - y3) +
                (x2 * x2 + y2 * y2) * (y3 - y1) +
                (x3 * x3 + y3 * y3) * (y1 - y2)) / d;

            double uy = ((x1 * x1 + y1 * y1) * (x3 - x2) +
                (x2 * x2 + y2 * y2) * (x1 - x3) +
                (x3 * x3 + y3 * y3) * (x2 - x1)) / d;

            return { ux, uy };
            };

        map<AristaClave, vector<int>> mapaAristas;
        vector<coordenada> centros;

        for (int i = 0; i < triangulosDelaunay.size(); ++i) {
            const auto& t = triangulosDelaunay[i];
            centros.push_back(circuncentro(t));

            auto normalizar = [](coordenada a, coordenada b) -> AristaClave {
                if (a.x < b.x || (a.x == b.x && a.y < b.y))
                    return { a, b };
                else
                    return { b, a };
                };

            mapaAristas[normalizar(t.V1, t.V2)].push_back(i);
            mapaAristas[normalizar(t.V2, t.V3)].push_back(i);
            mapaAristas[normalizar(t.V3, t.V1)].push_back(i);
        }

        for (const auto& [arista, indices] : mapaAristas) {
            if (indices.size() == 2) {
                // Shared edge, internal Voronoi edge
                coordenada c1 = centros[indices[0]];
                coordenada c2 = centros[indices[1]];
                fronteras.push_back({ c1, c2 });
            }
            else if (indices.size() == 1) {
                // Border edge, extend Voronoi edge to bounding box
                const triangulos& t = triangulosDelaunay[indices[0]];
                coordenada mid = { (arista.a.x + arista.b.x) / 2, (arista.a.y + arista.b.y) / 2 };
                double dx = arista.b.y - arista.a.y;
                double dy = -(arista.b.x - arista.a.x);

                double len = sqrt(dx * dx + dy * dy);
                dx /= len; dy /= len;

                coordenada c = centros[indices[0]];

                // Calcula el centroide de las semillas para saber hacia dónde está "fuera"
                double cx = 0, cy = 0;
                int total = 0;
                for (const auto& tri : triangulosDelaunay) {
                    cx += tri.V1.x + tri.V2.x + tri.V3.x;
                    cy += tri.V1.y + tri.V2.y + tri.V3.y;
                    total += 3;
                }
                cx /= total; cy /= total;
                coordenada centroide = { cx, cy };

                // Vector de la arista al circuncentro
                double vx = c.x - mid.x;
                double vy = c.y - mid.y;
                // Vector del centroide al circuncentro
                double wx = c.x - centroide.x;
                double wy = c.y - centroide.y;
                // Producto escalar para saber si la dirección es hacia afuera
                double dot = dx * wx + dy * wy;
                if (dot < 0) { dx = -dx; dy = -dy; }

                // Solo extiende en la dirección exterior
                coordenada far;
                if (clipRayToBounds(c, { dx, dy }, 1920, 1980, far)) {
                    fronteras.push_back({ c, far });
                }
            }
        }
    }
};

int drawVoronoiDiagram(RenderWindow& win, RectangleShape salirButton, Font font, Text salirText, const vector<Vector2f>& pts) {
    plano h(pts); // Usar las semillas externas
    triangulo t;
    Voronoi v;
    v.circ;
    t.obtenerTrianguloEnglobador(h);
    for (const auto& p : h.getSemillas())
        t.circulo({ p.x, p.y });
    t.eliminarTriangulosSuper();
    vector<pair<coordenada, coordenada>> fronteras;
    v.generarFronteras(t.getTriangulos(), fronteras);

    vector<puntos> semillas_puntos;
    for (const auto& v : h.getSemillas())
        semillas_puntos.emplace_back(coordenada{ v.x, v.y });

    h.dibujito(win, salirButton, font, salirText, semillas_puntos, fronteras);
    return 0;
}

#endif // !Voronoi_h