#ifndef Encripcion_Otros
#define Encripcion_Otros
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "lzw.h"
#include "Cesar.h"
#include "playfair.h"
#include "Vigenere.h"
#include "Generaciones.h"
using namespace std;

inline string joinInts(const vector<int>& v) {
    stringstream ss;
    bool first = true;
    for (int x : v) {
        if (!first) ss << ' ';
        ss << x;
        first = false;
    }
    return ss.str();
}

inline void Encriptar_Comprimir_Otros() {
    // Abrir archivo original
    ifstream lee("contactos.txt", ios::in);
    if (!lee.is_open()) {
        cout << "Error abriendo contactos.txt" << endl;
        return;
    }

    // Leer todas las líneas
    vector<string> lines;
    string line;
    while (getline(lee, line)) {
        // conservar tal cual (pero eliminar \r si existe)
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    lee.close();

    // Si el archivo ya fue procesado (flag en primera línea), no volvemos a procesar
    if (!lines.empty() && lines[0] == "PROCESSED") return;

    // Preparar archivo auxiliar con flag PROCESSED en la primera línea
    ofstream aux("auxiliar.txt", ios::out);
    if (!aux.is_open()) {
        cout << "Error abriendo auxiliar.txt" << endl;
        return;
    }
    aux << "PROCESSED" << "\n";

    // Procesar en bloques de 3: Username, Bank, Owner
    for (size_t idx = 0; idx < lines.size(); idx += 3) {
        string usernameLine = (idx < lines.size()) ? lines[idx] : "";
        string bankLine = (idx + 1 < lines.size()) ? lines[idx + 1] : "";
        string ownerLine = (idx + 2 < lines.size()) ? lines[idx + 2] : "";

        // Usuario: César (desplazamiento 4) + LZW (escribimos la secuencia de enteros en su propia línea)
        if (!usernameLine.empty()) {
            string user_enc = encrypt(usernameLine, 4);
            vector<int> user_comp = LZW_compress(user_enc);
            aux << joinInts(user_comp) << "\n";
        }
        else {
            aux << "\n";
        }

        // Banco: Vigenère + LZW
        if (!bankLine.empty()) {
            string bank_enc = cifrarVigenere(bankLine, "BANCO");
            vector<int> bank_comp = LZW_compress(bank_enc);
            aux << joinInts(bank_comp) << "\n";
        }
        else {
            aux << "\n";
        }

        // Dueño: Playfair + LZW
        if (!ownerLine.empty()) {
            PlayfairCipher playfair("BANCO");
            string owner_enc = playfair.encrypt(ownerLine);
            vector<int> owner_comp = LZW_compress(owner_enc);
            aux << joinInts(owner_comp) << "\n";
        }
        else {
            aux << "\n";
        }
    }

    aux.close();

    // Reemplazar el original por el procesado
    remove("contactos.txt");
    rename("auxiliar.txt", "contactos.txt");
}

#endif // !Encripcion_Otros