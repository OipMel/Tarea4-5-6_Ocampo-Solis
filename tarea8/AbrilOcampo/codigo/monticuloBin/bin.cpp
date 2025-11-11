#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp> // para la parte gráfica
using namespace std;

// función para intercambiar dos valores ( sirve en el heap)
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

//  clase Heap, maneja los pedidos del restaurante
class Heap {
    vector<int> arr; // se guardan todos los pedidos
public:
    Heap() {} // constructor vacío

    // insertar un pedido en el heap
    void insertar(int val) {
        arr.push_back(val); // lo agregamos al final
        int i = arr.size() - 1;
        // ahora lo subimos hasta su lugar correcto
        while (i > 0) {
            int padre = (i - 1) / 2; // índice del padre
            if (arr[i] < arr[padre]) { // MinHeap: si es más urgente que el padre, intercambiamos
                swap(arr[i], arr[padre]);
                i = padre;
            }
            else break; // si está bien, paramos
        }
    }

    // función para que el usuario ingrese pedidos
    void insertarPedidos() {
        char resp;
        int val;
        do {
            cout << "Ingrese prioridad del pedido (menor = mas urgente): ";
            cin >> val;
            insertar(val); // llamamos a la función que inserta en el heap

            // preguntamos si quiere seguir agregando
            do {
                cout << "Desea ingresar otro pedido? (s/n): ";
                cin >> resp;
                resp = tolower(resp);
                if (resp != 's' && resp != 'n') cout << "Error: solo 's' o 'n'.\n";
            } while (resp != 's' && resp != 'n');

        } while (resp == 's');

        cout << "Pedidos agregados.\n";
    }

    // mostrar todos los pedidos actuales
    void mostrar() {
        if (arr.empty()) {
            cout << "No hay pedidos en el restaurante.\n";
        }
        else {
            cout << "Pedidos actuales: ";
            for (int val : arr) cout << val << " ";
            cout << endl;
        }
    }

    // devolver el pedido más urgente (la raíz del heap)
    int mostrarMasUrgente() {
        if (!arr.empty()) return arr[0];
        throw runtime_error("No hay pedidos.");
    }

    // devolver el pedido menos urgente (el máximo del vector)
    int mostrarMenosUrgente() {
        if (!arr.empty()) {
            int maxVal = arr[0];
            for (int v : arr) if (v > maxVal) maxVal = v;
            return maxVal;
        }
        throw runtime_error("No hay pedidos.");
    }

    // extraer el pedido más urgente (raíz)
    int extraerRaiz() {
        if (arr.empty()) throw runtime_error("No hay pedidos.");
        int root = arr[0]; // guardamos la raíz para devolverla
        arr[0] = arr.back(); // ponemos el último nodo en la raíz
        arr.pop_back(); // eliminamos el último

        // ahora bajamos el nodo hasta su lugar correcto (heapify)
        int i = 0;
        while (true) {
            int izquierda = 2 * i + 1;
            int derecha = 2 * i + 2;
            int target = i;

            if (izquierda < arr.size() && arr[izquierda] < arr[target])
                target = izquierda;
            if (derecha < arr.size() && arr[derecha] < arr[target])
                target = derecha;
            if (target != i) {
                swap(arr[i], arr[target]);
                i = target;
            }
            else break; // ya está bien
        }
        return root; // devolvemos el pedido atendido
    }

    // ---------------- VISUALIZACIÓN CON SFML ----------------
    void dibujarHeap() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Heap - Pedidos");
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) { // cargamos la fuente
            cout << "Error cargando fuente arial.ttf\n";
            return;
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close(); // cerramos ventana si el usuario lo cierra
            }

            window.clear(sf::Color::White); // fondo blanco

            int n = arr.size();
            for (int i = 0; i < n; i++) {
                // calculamos el nivel del nodo
                int level = 0, idx = i + 1;
                while (idx >>= 1) level++;

                // calculamos posición x e y del nodo
                float gap = 400 / (1 << level);
                float x = 400 + (2 * (i - (1 << level) + 1) - 1) * gap;
                float y = 50 + level * 80;

                // dibujar círculo del nodo
                sf::CircleShape nodo(20);
                nodo.setFillColor(sf::Color::Green);
                nodo.setPosition(x, y);
                window.draw(nodo);

                // dibujar número dentro del nodo
                sf::Text texto;
                texto.setFont(font);
                texto.setString(to_string(arr[i]));
                texto.setCharacterSize(16);
                texto.setFillColor(sf::Color::Black);
                texto.setPosition(x + 10, y + 10);
                window.draw(texto);

                // dibujar línea hacia el padre
                if (i != 0) {
                    int padre = (i - 1) / 2;
                    int nivelPadre = 0, idxp = padre + 1;
                    while (idxp >>= 1) nivelPadre++;
                    float gapPadre = 400 / (1 << nivelPadre);
                    float xPadre = 400 + (2 * (padre - (1 << nivelPadre) + 1) - 1) * gapPadre + 20;
                    float yPadre = 50 + nivelPadre * 80 + 20;
                    sf::Vertex linea[] = {
                        sf::Vertex(sf::Vector2f(x + 20, y + 20), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(xPadre, yPadre), sf::Color::Black)
                    };
                    window.draw(linea, 2, sf::Lines);
                }
            }

            window.display(); // mostramos todo
        }
    }
};

int main() {
    Heap heap;
    int opcion;

    cout << "Bienvenido al sistema de pedidos del restaurante.\n";
    heap.insertarPedidos(); // primero ingresamos los pedidos

    do {
        // menú simple
        cout << "\n--- Menu Restaurante ---\n";
        cout << "1. Consultar pedido mas urgente\n";
        cout << "2. Consultar pedido menos urgente\n";
        cout << "3. Atender pedido (extraer mas urgente)\n";
        cout << "4. Mostrar todos los pedidos\n";
        cout << "5. Dibujar heap (visualizacion SFML)\n";
        cout << "6. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            try {
                cout << "Pedido mas urgente: " << heap.mostrarMasUrgente() << endl;
            }
            catch (runtime_error& e) { cout << e.what() << endl; }
            break;
        case 2:
            try {
                cout << "Pedido menos urgente: " << heap.mostrarMenosUrgente() << endl;
            }
            catch (runtime_error& e) { cout << e.what() << endl; }
            break;
        case 3:
            try {
                int atendido = heap.extraerRaiz();
                cout << "Atendiendo pedido: " << atendido << endl;
            }
            catch (runtime_error& e) { cout << e.what() << endl; }
            break;
        case 4:
            heap.mostrar();
            break;
        case 5:
            heap.dibujarHeap(); // abre la ventana gráfica
            break;
        case 6:
            cout << "Saliendo del sistema.\n";
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }

    } while (opcion != 6);

    return 0;
}
