#ifndef BOYER_MOORE_BANCO_H
#define BOYER_MOORE_BANCO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "agendado.h"
#include "Generaciones.h"
using namespace std;

#define NO_OF_CHARS 256

inline void HeuristicaCMalo(const string& str, int badchar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++) badchar[i] = -1;
    for (int i = 0; i < (int)str.size(); i++) badchar[(int)str[i]] = i;
}

inline void proPrefijos(const string& str, size_t size, vector<int>& prefix) {
    prefix[0] = 0;
    int k = 0;
    for (size_t i = 1; i < size; ++i) {
        while (k > 0 && str[k] != str[i]) k = prefix[k - 1];
        if (str[k] == str[i]) k++;
        prefix[i] = k;
    }
}

inline void heuristicaCBueno(const string& pat, vector<int>& good_suffix) {
    size_t size = pat.size();
    good_suffix.assign(size + 1, size);
    string reversed = pat;
    reverse(reversed.begin(), reversed.end());
    vector<int> prefix_normal(size), prefix_reversed(size);
    proPrefijos(pat, size, prefix_normal);
    proPrefijos(reversed, size, prefix_reversed);
    for (size_t i = 0; i < size; i++) {
        int j = size - prefix_reversed[i];
        int k = i - prefix_reversed[i] + 1;
        if (j <= (int)size && good_suffix[j] > k) good_suffix[j] = k;
    }
}

// ----------------------- BOYER-MOORE FUNCIONES PRINCIPALES -----------------------
inline bool mooreCoincideEnTexto(const string& txt, const string& pat) {
    int m = (int)pat.size();
    int n = (int)txt.size();
    if (m == 0 || n == 0 || m > n) return false;

    int badchar[NO_OF_CHARS];
    vector<int> good_suffix;

    HeuristicaCMalo(pat, badchar);
    heuristicaCBueno(pat, good_suffix);

    int s = 0;
    while (s <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pat[j] == txt[s + j]) j--;
        if (j < 0) return true;
        else {
            int bad_char_shift = j - badchar[(int)txt[s + j]];
            int good_suffix_shift = (j + 1 < (int)good_suffix.size()) ? good_suffix[j + 1] : 1;
            s += max(1, max(bad_char_shift, good_suffix_shift));
        }
    }
    return false;
}

// ----------------------- BUSQUEDA DE MOVIMIENTOS -----------------------
inline void busqueda_mov_BoyerMoore(const string& fechaBuscada, const string& nombreArchivo = "Historial.txt") {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    string linea;
    bool mostrar = false;
    vector<string> movimiento;

    while (getline(archivo, linea)) {
        if (linea.find("Fecha: ") == 0) {
            if (!movimiento.empty() && mostrar) {
                for (const string& l : movimiento) cout << l << endl;
                cout << "------------------------------------" << endl;
            }
            movimiento.clear();
            mostrar = false;

            string fechaHistorial = linea.substr(7);
            movimiento.push_back(linea);

            if (mooreCoincideEnTexto(fechaHistorial, fechaBuscada)) mostrar = true;
        }
        else {
            movimiento.push_back(linea);
        }
    }

    if (!movimiento.empty() && mostrar) {
        for (const string& l : movimiento) cout << l << endl;
        cout << "------------------------------------" << endl;
    }

    archivo.close();
}

// ----------------------- BUSQUEDA DE CONTACTOS -----------------------
inline agendado BuscarAgendado_BM(const string& alias, const string& nombreArchivo = "contactos.txt") {
    ifstream buscar(nombreArchivo);
    agendado datos = { "","","","","",false };

    if (!buscar.is_open()) {
        cout << "Error al abrir " << nombreArchivo << endl;
        return datos;
    }

    string linea;
    while (getline(buscar, linea)) {
        if (linea.find("Alias:") != string::npos) {
            string aliasArchivo = linea.substr(6);
            if (mooreCoincideEnTexto(aliasArchivo, alias)) {
                datos.alias = aliasArchivo;
                getline(buscar, linea); datos.cbu = linea.substr(0);
                getline(buscar, linea); datos.nombre = linea.substr(0);
                getline(buscar, linea); datos.cuit = linea.substr(0);
                getline(buscar, linea); datos.banco = linea.substr(0);
                datos.encontrado = true;
                break;
            }
        }
    }

    if (!datos.encontrado) cout << "Alias " << alias << " no encontrado en " << nombreArchivo << endl;

    buscar.close();
    return datos;
}

#endif