#ifndef PRIM_H
#define PRIM_H

#include "Grafo_Combinado.h"
#include <vector>
#include <queue>
#include <tuple>
#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include "SFML.h"
using namespace std;

// Estructura de arista 
struct AristaMST {
    int origen, destino, peso;
};

pair<int, vector<AristaMST>> MST(const Grafo& grafo, int inicio = 0) {
    int tamanio_grafo = grafo.size();
    auto adyacente = grafo.obtener_adya();

    vector<bool> visitado(tamanio_grafo, false);
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> cola;
    vector<AristaMST> mst;
    int total = 0;

    cola.emplace(0, inicio, -1); // peso, nodoactual, nodopadre

    while (!cola.empty() && mst.size() < (size_t)(tamanio_grafo - 1)) {
        auto [peso, u, padre] = cola.top();
        cola.pop();

        if (visitado[u]) continue;
        visitado[u] = true;

        if (padre != -1) {
            mst.push_back({ padre, u, peso });
            total += peso;
        }

        for (const auto& [v, w] : adyacente[u]) {
            if (!visitado[v]) {
                cola.emplace(w, v, u);
            }
        }
    }

    if (mst.size() != (size_t)(tamanio_grafo - 1)) {
        return { -1, {} }; // grafo no conexo, no anda
    }

    return { total, mst };
}

void ejecutar_prim(const Grafo& grafo) {
    grafo.mostrarArchivo();

    cout << "\t\t-----------------------------------------------------------------\n";

    if (grafo.es_direccional()) {
        cout << "\t\t*Prim no aplica, el grafo es direccional\n";
        return;
    }
    if (!grafo.es_ponderado()) {
        cout << "\t\t*Prim requiere pesos en las aristas\n";
        return;
    }

    string nodo_inicio;
    cout << "\t\t-Ingrese nodo de inicio: ";
    cin >> nodo_inicio;

    if (!grafo.duplicado(nodo_inicio)) {
        cout << "\t\t*El nodo no existe\n";
        return;
    }

    int tipoHeapConsola = 0;
    cout << "\t\tOrden de creacion del heap:\n";
    cout << "\t\t  1) De menor a mayor (min-heap)\n";
    cout << "\t\t  2) De mayor a menor (max-heap)\n";
    cout << "\t\t  Seleccione 1 o 2: ";
    cin >> tipoHeapConsola;
    Heap::TipoHeap tipoHeap = (tipoHeapConsola == 2) ? Heap::MAXIMO : Heap::MINIMO;

    int inicio = grafo.indice_de(nodo_inicio);
    auto [peso_total, mst] = MST(grafo, inicio);

    if (peso_total == -1) {
        cout << "\n\t\t*No se pudo construir el arbol de expansion minimo (el grafo no es conexo)\n";
        return;
    }

    int cantidad_aristas = 0;

    cout << "\n\t\t-----------------------------------------------------------------\n";
    cout << "\t\t* Arbol de expansion minima:\n";
    for (const auto& a : mst) {
        cout << "\t\t " << grafo.nombre_de(a.origen)
            << " - " << grafo.nombre_de(a.destino)
            << " (peso " << a.peso << ")\n";
        cantidad_aristas++;
    }

	cout << "\n\t\t-----------------------------------------------------------------\n";
	cout << "\t\t* Peso total de la conexion mas optima: " << peso_total << "\n";
	cout << "\t\t-----------------------------------------------------------------\n";

	//Vector de los pesos de las aristas del arbol
    int* vector_tam = new int[mst.size()];
    for (size_t i = 0; i < mst.size(); ++i) {
        vector_tam[i] = mst[i].peso;
    }
    window(vector_tam, cantidad_aristas, tipoHeap);

    delete[] vector_tam;
}
#endif 
