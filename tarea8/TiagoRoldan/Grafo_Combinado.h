#ifndef GRAFO_COMBINADO_H
#define GRAFO_COMBINADO_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;

class Grafo {
private: //atributos
    unordered_map<string, int> etiqueta_a_indice;
    vector<string> indice_a_etiqueta;
    vector<vector<pair<int, int>>> adyacencia;
    bool ponderado;
    bool direccional;

public:
    Grafo() : ponderado(false), direccional(false) {}//constructor 

    //verificaciones
    void set_direccional(bool d) {
        direccional = d;
    }

    bool es_ponderado() const {
        return ponderado;
    }

    bool es_direccional() const {
        return direccional;
    }

    bool tiene_pesos_negativos() const {
        for (const auto& lista : adyacencia)
            for (const auto& arista : lista)
                if (arista.second < 0) return true;
        return false;
    }
    bool todos_los_pesos_iguales() const {
        bool primer_peso_asignado = false;
        int primer_peso = 0;

        for (const auto& lista : adyacencia) {
            for (const auto& arista : lista) {
                int peso = arista.second;

                if (!primer_peso_asignado) {
                    primer_peso = peso;
                    primer_peso_asignado = true;
                }
                else if (peso != primer_peso) {
                    return false;
                }
            }
        }
        return true;
    }

    bool duplicado(const string& etiqueta) const {
        string lbl = toUpper(etiqueta);
        return etiqueta_a_indice.find(lbl) != etiqueta_a_indice.end();
    }

    bool existeArista(const string& a, const string& b, int peso = -1) const {
        int ia = indice_de(a);
        int ib = indice_de(b);
        if (ia == -1 || ib == -1) return false;
        for (const auto& vw : adyacencia[ia]) {
            if (vw.first == ib && (peso == -1 || vw.second == peso))
                return true;
        }
        return false;
    }

    //funciones generales de grafo
    void vaciar() {
        etiqueta_a_indice.clear();
        indice_a_etiqueta.clear();
        adyacencia.clear();
        ponderado = false;
    }

    string toUpper(const string& s) const {
        string res = s;
        transform(res.begin(), res.end(), res.begin(), ::toupper);
        return res;
    }

    int agregar_nodo(const string& etiqueta) {
        string lbl = toUpper(etiqueta);
        auto it = etiqueta_a_indice.find(lbl);
        if (it != etiqueta_a_indice.end()) return it->second;
        int indice = (int)indice_a_etiqueta.size();
        etiqueta_a_indice[lbl] = indice;
        indice_a_etiqueta.push_back(lbl);
        adyacencia.emplace_back();
        return indice;
    }

    void agregar_arista(const string& a, const string& b, int peso = 0, bool bidireccional = true, bool mostrarMensaje = true) {
        string A = toUpper(a), B = toUpper(b);

        if (!duplicado(A) || !duplicado(B)) {
            if (mostrarMensaje)
                cout << "\t\tError: ambos nodos deben existir\n";
            return;
        }

        int ia = agregar_nodo(A);
        int ib = agregar_nodo(B);


        if (existeArista(A, B, peso)) {
            if (mostrarMensaje)
                cout << "\t\tYa existe la arista\n";
            return;
        }

        if (peso != 0) ponderado = true;
        adyacencia[ia].push_back({ ib, peso });
        if (bidireccional)
            adyacencia[ib].push_back({ ia, peso });

        if (mostrarMensaje)
            cout << "\t\t*Arista agregada\n";
    }

    int indice_de(const string& etiqueta) const {
        string lbl = toUpper(etiqueta);
        auto it = etiqueta_a_indice.find(lbl);
        if (it == etiqueta_a_indice.end()) return -1;
        return it->second;
    }

    const string& nombre_de(int indice) const {
        return indice_a_etiqueta[indice];
    }

    int size() const {
        return (int)indice_a_etiqueta.size();
    }

    const vector<vector<pair<int, int>>>& obtener_adya() const {
        return adyacencia;
    }


    vector<string> obtener_nodos() const {
        return indice_a_etiqueta;
    }

    int cargarArchivo(const string& nombre_archivo = "grafo.txt") {
        ifstream archivo(nombre_archivo);
        if (!archivo.is_open()) return false;

        vaciar();
        string linea;
        bool enNodos = false, enAristas = false;

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            if (linea.find("DIRECTIONAL") == 0) {
                direccional = (linea.substr(12) == "1");
                continue;
            }

            if (linea == "NODES") {
                enNodos = true;
                enAristas = false;
                continue;
            }

            if (linea == "EDGES") {
                enNodos = false;
                enAristas = true;
                continue;
            }

            if (enNodos) {
                agregar_nodo(linea);
            }

            if (enAristas) {
                string nodo1, nodo2;
                int peso = 0;
                stringstream ss(linea);
                ss >> nodo1 >> nodo2;
                if (!(ss >> peso)) peso = 0;

                agregar_arista(nodo1, nodo2, peso, !direccional, false);
            }
        }

        archivo.close();
        return 0;
    }

    bool guardar(const string& nombre_archivo = "grafo.txt") const {
        ofstream archivo(nombre_archivo, ios::trunc);
        if (!archivo) {
            cout << "Error\n";
            return false;
        }

        archivo << "DIRECTIONAL " << (direccional ? "1" : "0") << "\n";
        archivo << "NODES\n";
        for (const auto& n : indice_a_etiqueta)
            archivo << n << "\n";
        archivo << "EDGES\n";

        for (int i = 0; i < (int)adyacencia.size(); ++i) {
            for (auto pr : adyacencia[i]) {
                int v = pr.first;
                int w = pr.second;

                if (!direccional && i < v) { // evitar dup
                    if (ponderado)
                        archivo << indice_a_etiqueta[i] << " " << indice_a_etiqueta[v] << " " << w << "\n";
                    else
                        archivo << indice_a_etiqueta[i] << " " << indice_a_etiqueta[v] << "\n";
                }
                else if (direccional) { //guardar
                    if (ponderado)
                        archivo << indice_a_etiqueta[i] << " " << indice_a_etiqueta[v] << " " << w << "\n";
                    else
                        archivo << indice_a_etiqueta[i] << " " << indice_a_etiqueta[v] << "\n";
                }
            }
        }

        archivo.close();
        return true;
    }

    void borrar_archivo(const string& nombre_archivo = "grafo.txt") {
        ofstream archivo(nombre_archivo, ios::trunc);
        if (archivo) {
            archivo.close();
        }
        else {
            cout << "Error al borrar archivo\n";
        }
    }

    void mostrarArchivo(const string& nombre_archivo = "grafo.txt") const {
        ifstream archivo(nombre_archivo);
        if (!archivo.is_open()) {
            cout << "\t\tNo hay archivo " << nombre_archivo << "\n";
            return;
        }
        cout << "\t\tContenido del grafo:\n";
        cout << "\t\t*NODOS:\n";
        for (const auto& n : indice_a_etiqueta)
            cout << "\t\t\t" << n << "\n";
        cout << "\t\t*ARISTAS:\n";
        int n = (int)adyacencia.size();
        for (int i = 0; i < n; ++i) {
            for (auto pr : adyacencia[i]) {
                int v = pr.first;
                int w = pr.second;


                if (direccional || i < v) {
                    cout << "\t\t\t" << indice_a_etiqueta[i]
                        << (direccional ? " -> " : " - ")
                        << indice_a_etiqueta[v];
                    if (ponderado) cout << " peso: " << w;
                    cout << "\n";
                }
            }
        }
        archivo.close();
    }
};
#endif 