#include <SFML/Graphics.hpp>
#include "Prim.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std; using namespace sf;

class Heap {
public:
    enum TipoHeap { MINIMO, MAXIMO };

    Heap(int tamanio_heap, TipoHeap tipo = MINIMO) : capacidad(tamanio_heap), indic_ultimo(0), tipoHeap(tipo) {
        heap = new int[tamanio_heap];
    }

    Heap(const int* nodos, int cantidad, int tamanio_heap, TipoHeap tipo = MINIMO) : capacidad(tamanio_heap), indic_ultimo(0), tipoHeap(tipo) {
        heap = new int[capacidad];
        for (int i = 0; i < cantidad && i < capacidad; ++i) {
            Insertar(nodos[i]);
        }
    }

    ~Heap() { delete[] heap; }

    void Insertar(int node) {
        if (indic_ultimo >= capacidad) Reubicar(capacidad * 2);
        heap[indic_ultimo++] = node;
        Subir(indic_ultimo - 1);
    }

    int ExtraerMinimo() {
        if (Vacio()) return -1;
        int extraer_min = heap[0];
        Borrar(0);
        return extraer_min;
    }

    bool Vacio() const { 
        return indic_ultimo == 0; 
    }

    int Tam() const { 
        return indic_ultimo; 
    }

    int Valor(int i) const { 
        return heap[i]; 
    }

    int Capacidad() const { 
        return capacidad; 
    }

    int Padre(int i) const { 
        return (i - 1) / 2; 
    }

    int Izquierdo(int i) const { 
        return 2 * i + 1; 
    }

    int Derecho(int i) const {
       return 2 * i + 2;
    }

private:

    void Reubicar(int nuevaCapacidad) {
        int* heap2 = new int[nuevaCapacidad];
        for (int i = 0; i < indic_ultimo; i++) heap2[i] = heap[i];
        delete[] heap;
        heap = heap2;
        capacidad = nuevaCapacidad;
    }

    void Intercambia(int nodo1, int nodo2) {
        int aux = heap[nodo1];
        heap[nodo1] = heap[nodo2];
        heap[nodo2] = aux;
    }

    void Subir(int i) {
        int iPadre;
        while (i > 0 && comparar(heap[i], heap[iPadre = Padre(i)])) {
            Intercambia(i, iPadre);
            i = iPadre;
        }
    }

    void Bajar(int nodo) {
        int nodo_izq, nodo_der, extremo;
        extremo = nodo;
        do {
            nodo = extremo;
            nodo_izq = Izquierdo(nodo);
            nodo_der = Derecho(nodo);
            if (nodo_der < indic_ultimo && comparar(heap[nodo_der], heap[extremo])) extremo = nodo_der;
            if (nodo_izq < indic_ultimo && comparar(heap[nodo_izq], heap[extremo])) extremo = nodo_izq;
            if (nodo != extremo) Intercambia(nodo, extremo);
        } while (nodo != extremo && extremo < indic_ultimo / 2);
    }

    void Borrar(int i) {
        if (i < indic_ultimo) {
            heap[i] = heap[--indic_ultimo];
            Bajar(i);
        }
    }

    bool comparar(int nodo1, int nodo2) const {
        if (tipoHeap == MINIMO)
            return nodo1 < nodo2;
        else
            return nodo1 > nodo2;
    }

    int* heap;
    int capacidad;
    int indic_ultimo;
    TipoHeap tipoHeap;
};

// Utilidad para dibujar líneas entre dos puntos
VertexArray dibujarEnlace(Vector2f a, Vector2f b, Color color, float grosor = 2.f) {
    Vector2f direccion = b - a;
    float longitud = sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
    Vector2f unit = direccion / longitud;
    Vector2f normal(-unit.y, unit.x);
    VertexArray va(Quads, 4);
    va[0].position = a + normal * (grosor / 2.f);
    va[1].position = b + normal * (grosor / 2.f);
    va[2].position = b - normal * (grosor / 2.f);
    va[3].position = a - normal * (grosor / 2.f);
    for (int i = 0; i < 4; ++i) va[i].color = color;
    return va;
}

// Dibuja el vector del heap como una fila de nodos
void dibujarVectorHeap(RenderWindow& window, Font& font, const Heap& h, Vector2f pos, float nodoR) {
    for (int i = 0; i < h.Tam(); ++i) {
        CircleShape nodo(nodoR);
        nodo.setOrigin(nodoR, nodoR);
        nodo.setPosition(pos.x + i * (2 * nodoR + 10), pos.y);
        nodo.setFillColor(Color(220, 240, 255));
        nodo.setOutlineThickness(2.f);
        nodo.setOutlineColor(Color(60, 60, 60));
        window.draw(nodo);

        Text t(to_string(h.Valor(i)), font, 18);
        t.setFillColor(Color::Black);
        auto b = t.getLocalBounds();
        t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        t.setPosition(nodo.getPosition());
        window.draw(t);
    }
}

void dibujarArbolHeap(RenderWindow& window, Font& font, const Heap& h, Vector2f raiz, float separacion, float altura, float nodoR) {

    int tamanio = h.Tam();
    vector<Vector2f> posicion(tamanio);
    if (tamanio == 0) return;

    // Calcula posiciones de los nodos
    posicion[0] = raiz;
    for (int i = 1; i < tamanio; ++i) {
        int p = h.Padre(i);
        float dx = separacion / (1 << (int)log2(i + 1));
        float sign = (i % 2 == 0) ? 1.f : -1.f;
        posicion[i] = posicion[p] + Vector2f(sign * dx, altura);
    }

    // Dibuja aristas
    for (int i = 1; i < tamanio; ++i) {
        int p = h.Padre(i);
        window.draw(dibujarEnlace(posicion[p], posicion[i], Color(120, 180, 255), 3.f));
    }

    // Dibuja nodos
    for (int i = 0; i < tamanio; ++i) {
        CircleShape node(nodoR);
        node.setOrigin(nodoR, nodoR);
        node.setPosition(posicion[i]);
        node.setFillColor(Color::White);
        node.setOutlineThickness(3.f);
        node.setOutlineColor(Color(60, 60, 60));
        window.draw(node);

        Text t(to_string(h.Valor(i)), font, 18);
        t.setFillColor(Color::Black);
        auto b = t.getLocalBounds();
        t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        t.setPosition(node.getPosition());
        window.draw(t);
    }
}

int window(int* nodos_heap, int max_cantidad_nodos, Heap::TipoHeap tipoHeap) {

    srand((unsigned)time(nullptr));
    Heap h1(max_cantidad_nodos, tipoHeap);
    vector<int> pesos_restantes(nodos_heap, nodos_heap + max_cantidad_nodos);   //Pesos originales restantes
    vector<int> heap_binario;   //Orden binario
    vector<int> binario_ordenado;   //Vector ordenado por heapsotr
    int indice_insercion = 0;   //Siguiente insercion
    bool heap_completo = false; //Condicional

    // Window
    const unsigned WIN_W = 1100, WIN_H = 700;
    RenderWindow window(VideoMode(WIN_W, WIN_H), "Visualización de Heap (vector y árbol)");
    window.setFramerateLimit(60);

    // Fuente
    Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }

    const float nodoR = 28.f;
    const float Altura = 80.f;
    const float Separacion = 350.f;

    string instrucciones = "ESPACIO: Insertar siguiente peso | ENTER: Extraer mínimo (cuando heap esté completo) | ESC: Salir";

    while (window.isOpen()) {   
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) window.close();
                if (event.key.code == Keyboard::Space) {
                    if (indice_insercion < max_cantidad_nodos && !heap_completo) {        // Insertar siguiente peso del heap
                        h1.Insertar(pesos_restantes[indice_insercion]);
                        pesos_restantes.erase(pesos_restantes.begin() + indice_insercion);       // Eliminar el elemento insertado del vector "Restantes"
                        if (h1.Tam() == max_cantidad_nodos) {
                            heap_completo = true;
                            indice_insercion = 0; //Dato que se debería insertar una vez apretado "Espacio"
                        }
                    }
                }
                if (event.key.code == Keyboard::Enter) {    //Peritir sacar solo si el arbol heap esta completo
                    if (heap_completo && !h1.Vacio()) {
                        int maximo = h1.ExtraerMinimo();
                        binario_ordenado.push_back(maximo);
                    }
                }
            }
        }

        heap_binario.clear();                   //Rehago vector con el orden correspondiente
        for (int i = 0; i < h1.Tam(); ++i) {
            heap_binario.push_back(h1.Valor(i));
        }
      
        window.clear(Color(245, 247, 250));

        Text t0("Pesos originales restantes:", font, 18);  // Dibuja el vector de pesos originales (sin borrar)
        t0.setFillColor(Color(60, 60, 60));
        t0.setPosition(60.f, 20.f);
        window.draw(t0);

        for (size_t i = 0; i < pesos_restantes.size(); ++i) {
            CircleShape node(nodoR);
            node.setOrigin(nodoR, nodoR);
            node.setPosition(80.f + i * (2 * nodoR + 10), 80.f);
            node.setFillColor(Color(220, 240, 255));
            node.setOutlineThickness(2.f);
            node.setOutlineColor(Color(60, 60, 60));
            window.draw(node);

            Text t(to_string(pesos_restantes[i]), font, 18);
            t.setFillColor(Color::Black);
            auto b = t.getLocalBounds();
            t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
            t.setPosition(node.getPosition());
            window.draw(t);
        }

        dibujarVectorHeap(window, font, h1, { 80.f, 180.f }, nodoR);
        dibujarArbolHeap(window, font, h1, { WIN_W / 2.f, 300.f }, Separacion, Altura, nodoR);

        Text t(instrucciones, font, 18);
        t.setFillColor(Color(40, 40, 40));  //instruccion
        t.setPosition(40.f, WIN_H - 40.f);
        window.draw(t);

        Text t2("Vector ordenado:", font, 18);
        t2.setFillColor(Color(60, 60, 60));    //Dibuja el vector ordenado mediante la extraccion desde el arbol
        t2.setPosition(60.f, 500.f);
        window.draw(t2);

        for (size_t i = 0; i < binario_ordenado.size(); ++i) {
            CircleShape node(nodoR);
            node.setOrigin(nodoR, nodoR);
            node.setPosition(80.f + i * (2 * nodoR + 10), 580.f);   
            node.setFillColor(Color(255, 230, 220));
            node.setOutlineThickness(2.f);
            node.setOutlineColor(Color(60, 60, 60));
            window.draw(node);

            Text t(to_string(binario_ordenado[i]), font, 18);
            t.setFillColor(Color::Black);
            auto b = t.getLocalBounds();
            t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
            t.setPosition(node.getPosition());
            window.draw(t);
        }
        window.display();
    }
    cout << "\n\t\t-----------------------------------------------------------------\n";
    cout << "\t\t* Orden de conexion: \n";
    for (int i = 0; i < binario_ordenado.size(); ++i) {
        cout << "\t\tConexion de peso: " << binario_ordenado[i] << "\n";
    }
    cout << "\t\t-----------------------------------------------------------------\n";
    return 0;
}