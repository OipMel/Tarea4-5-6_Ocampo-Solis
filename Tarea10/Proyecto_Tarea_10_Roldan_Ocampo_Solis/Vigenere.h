#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>
using namespace std;

// Función auxiliar para hacer mayúsculas (solo letras)
inline string limpiarTexto(const string& texto) {
    string limpio;
    for (char c : texto) {
        if (isalpha((unsigned char)c))
            limpio += toupper(c);
    }
    return limpio;
}

// Cifrado Vigenère
inline string cifrarVigenere(const string& texto, const string& clave) {
    string limpio = limpiarTexto(texto);
    string claveLimpia = limpiarTexto(clave);
    string resultado;
    int j = 0;
    for (int i = 0; i < limpio.size(); i++) {
        char c = ((limpio[i] - 'A') + (claveLimpia[j % claveLimpia.size()] - 'A')) % 26 + 'A';
        resultado += c;
        j++;
    }
    return resultado;
}

// Descifrado Vigenère
inline string descifrarVigenere(const string& texto, const string& clave) {
    string limpio = limpiarTexto(texto);
    string claveLimpia = limpiarTexto(clave);
    string resultado;
    int j = 0;
    for (int i = 0; i < limpio.size(); i++) {
        char c = ((limpio[i] - claveLimpia[j % claveLimpia.size()] + 26) % 26) + 'A';
        resultado += c;
        j++;
    }
    return resultado;
}

#endif
