#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <string_view>
#include <windows.h>
#include <sstream>
#include <queue>
#include <vector>
#include <random>
#include "Huffman.h"
#include "Cesar.h"
#include "Hill.h"
#include "Affin.h"
#include "Generaciones.h"
#include "Encriptar_Usuario.h"
#include "Encriptar_Otros.h"
#include "Cliente.h"
#include "Vigenere.h"
#include "kmp.h"
#include "lzw.h"
#include "playfair.h"
#include "agendado.h"
#include "Booyer_Moore.h"
using namespace std;

priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

// --- Funciones auxiliares para búsqueda y decodificación de contactos ---

// Codifica y comprime el nombre del contacto igual que en Encriptar_Comprimir_Otros
inline string codificarContacto(const string& nombre) {
    // Cifrado César (desplazamiento 4) y compresión LZW
    string cesar = encrypt(nombre, 4);
    vector<int> lzw = LZW_compress(cesar);
    // Convertimos el vector a string separado por espacios (igual que en contactos.txt)
    stringstream ss;
    if (!lzw.empty()) {
        ss << lzw[0];
        for (size_t i = 1; i < lzw.size(); ++i) ss << ' ' << lzw[i];
    }
    return ss.str();
}

// Dada una cadena con enteros separados por espacios (forma en que se guarda el LZW),
// convierte a vector<int>
inline vector<int> parseCompressedInts(const string& s) {
    vector<int> res;
    stringstream ss(s);
    int num;
    while (ss >> num) res.push_back(num);
    return res;
}

// Busca el contacto en el archivo y muestra sus datos decodificados
inline void buscarYMostrarContacto(const string& nombreContacto, const string& claveVigenere) {
    ifstream archivo("contactos.txt", ios::in);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir contactos.txt" << endl;
        return;
    }

    // Leer todas las líneas en un vector (conservar orden)
    vector<string> lines;
    string line;
    while (getline(archivo, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    archivo.close();

    if (lines.empty()) {
        cout << "contactos.txt vacío." << endl;
        return;
    }

    // Si la primera línea es la flag PROCESSED, saltarla para procesar los bloques reales
    size_t start = 0;
    if (lines[0] == "PROCESSED") start = 1;

    // Preparar patrón (mismo formato que en Encriptar_Comprimir_Otros)
    string patron = codificarContacto(nombreContacto);

    bool encontrado = false;
    // Iterar por bloques de 3 líneas
    for (size_t idx = start; idx + 0 < lines.size(); idx += 3) {
        string userLine = (idx < lines.size()) ? lines[idx] : "";
        string bankLine = (idx + 1 < lines.size()) ? lines[idx + 1] : "";
        string ownerLine = (idx + 2 < lines.size()) ? lines[idx + 2] : "";

        // Buscar usando Boyer-Moore: comparar la línea de usuario (números separados por espacios)
        if (mooreCoincideEnTexto(userLine, patron)) {
            encontrado = true;

            // Descomprimir cada campo si existe
            string username = "(vacío)", bank = "(vacío)", owner = "(vacío)";

            if (!userLine.empty()) {
                vector<int> comp = parseCompressedInts(userLine);
                if (!comp.empty()) {
                    string dec = LZW_decompress(comp);
                    if (dec != "(LZW error)") {
                        // Aquí se aplica la decodificación César tras la descompresión
                        username = decrypt(dec, 4);
                    }
                    else username = "(error LZW)";
                }
                else {
                    username = "(línea usuario vacía)";
                }
            }

            if (!bankLine.empty()) {
                vector<int> comp = parseCompressedInts(bankLine);
                if (!comp.empty()) {
                    string dec = LZW_decompress(comp);
                    if (dec != "(LZW error)") bank = descifrarVigenere(dec, claveVigenere);
                    else bank = "(error LZW)";
                }
                else {
                    bank = "(línea banco vacía)";
                }
            }

            if (!ownerLine.empty()) {
                vector<int> comp = parseCompressedInts(ownerLine);
                if (!comp.empty()) {
                    string dec = LZW_decompress(comp);
                    if (dec != "(LZW error)") {
                        PlayfairCipher playfair("BANCO");
                        owner = playfair.decrypt(dec);
                    }
                    else owner = "(error LZW)";
                }
                else {
                    owner = "(línea dueño vacía)";
                }
            }

            cout << "\n--- Datos del contacto ---" << endl;
            cout << "Usuario: " << username << endl;
            cout << "Banco: " << bank << endl;
            cout << "Dueño: " << owner << endl;
            break;
        }
        // si no hay suficientes líneas para el siguiente bloque, salir
        if (idx + 3 > lines.size()) break;
    }

    if (!encontrado) {
        cout << "Contacto no encontrado." << endl;
    }
}

// Busca el patrón en cada línea del archivo y muestra la línea completa donde se encuentra
bool fuerzaBrutaBusquedaEnArchivo(const string& nombreArchivo, const string& patron) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para lectura." << endl;
        return false;
    }

    string linea;
    int numLinea = 1;
    bool encontrado = false;
    while (getline(archivo, linea)) {
        linea = trim_cr(linea);
        int n = (int)linea.size();
        int m = (int)patron.size();
        if (m == 0 || n < m) { numLinea++; continue; }
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            while (j < m && linea[i + j] == patron[j]) j++;
            if (j == m) {
                encontrado = true;
            }
        }
        numLinea++;
    }
    if (!encontrado) {
        cout << "Patrón no encontrado en el archivo." << endl;
        archivo.close();
        return false;
    }
    archivo.close();
    return true;
}

int main() {
    // First, process the file and rewrite it with generated alias/CVU, encoded and compressed
    Encriptar_Comprimir();
    Encriptar_Comprimir_Otros();

    string usuario, contrasena, to_Encode = "", buscado = "", Usuario_Clave;
    cout << "Ingrese su nombre de usuario: ";
    cin >> usuario;
    cout << "Ingrese su contrase�a: ";
    cin >> contrasena;

    if (contrasena.size() != 4) {
        cout << "La contrase�a no coincide con el formato requerido (4 caracteres)." << endl;
    }

    // Build the same encoding for the lookup as we used when writing user lines:
    Usuario_Clave = usuario + "," + contrasena;
    freq.clear();
    codes.clear();
    while (!minHeap.empty()) minHeap.pop();
    to_Encode = encrypt(Usuario_Clave, 4);
    calcFreq(to_Encode);
    HuffmanCodes((int)to_Encode.length());

    for (auto i : to_Encode)
        buscado += codes[i];

    if (fuerzaBrutaBusquedaEnArchivo("Usuario.txt", buscado)) {
        ifstream infile("Usuario.txt");
        if (infile.is_open()) {
            string line;
            bool found = false;
            // find the exact line that matches 'buscado'
            while (getline(infile, line)) {
                line = trim_cr(line);
                if (line == buscado) {
                    found = true;
                    break;
                }
            }

            if (found) {
                system("cls");
                string moneyLine, aliasLine, cvuLine, ownerLine;

                if (!getline(infile, moneyLine)) moneyLine = "";
                moneyLine = trim_cr(moneyLine);

                if (!getline(infile, aliasLine)) aliasLine = "";
                aliasLine = trim_cr(aliasLine);

                if (!getline(infile, cvuLine)) cvuLine = "";
                cvuLine = trim_cr(cvuLine);

                if (!getline(infile, ownerLine)) ownerLine = "";
                ownerLine = trim_cr(ownerLine);

                // Decode money
                string decodedMoney = "";
                if (!moneyLine.empty()) decodedMoney = money_decode(moneyLine);
                else decodedMoney = "$0";
                cout << "Saldo: " << decodedMoney << endl;

                // Helper lambda to decode HUFF lines: parse header, read bitstring directly, rebuild Huffman, decode, then decrypt
                auto decodeHuffLine = [](const string& line)->string {
                    const string prefix = "HUFF:";
                    if (line.rfind(prefix, 0) != 0) return line; // return raw if not HUFF
                    size_t after = prefix.size();
                    size_t colonPos = line.find(':', after); // colon after header length
                    if (colonPos == string::npos) return string("(HUFF malformed)");
                    string nstr = line.substr(after, colonPos - after);
                    int N = atoi(nstr.c_str());
                    size_t headerStart = colonPos + 1;
                    if ((int)line.size() < (int)headerStart + N) return string("(HUFF header length mismatch)");
                    string header = line.substr(headerStart, N);
                    // the remainder after header is the raw bitstring (no Base64)
                    string bitstr = line.substr(headerStart + N);
                    // parse header
                    map<char, int> fmap;
                    int affA = -1, affB = -1;
                    int pad = parseFreqHeader(header, fmap, affA, affB);
                    if (pad < 0) return string("(HUFF header parse error)");
                    // rebuild Huffman tree
                    freq.clear(); codes.clear();
                    while (!minHeap.empty()) minHeap.pop();
                    for (auto& p : fmap) freq[p.first] = p.second;
                    HuffmanCodes(0);
                    if (minHeap.empty()) return string("(HUFF tree error)");
                    string decodedEncrypted = decode_file(minHeap.top(), bitstr);
                    // decodedEncrypted is the encrypted text, now decrypt with either affine (if present) or fall back to Caesar(4)
                    string plain;
                    if (affA != -1) {
                        plain = decryptAffine(decodedEncrypted, affA, affB);
                    }
                    else {
                        plain = decrypt(decodedEncrypted, 4);
                    }
                    // cleanup
                    freq.clear(); codes.clear();
                    while (!minHeap.empty()) minHeap.pop();
                    return plain;
                    };

                // Decode alias
                if (!aliasLine.empty()) {
                    string aliasDecoded = decodeHuffLine(aliasLine);
                    cout << "Alias: " << aliasDecoded << endl;
                }
                else {
                    cout << "Alias: (no disponible)" << endl;
                }

                // Decode CVU
                if (!cvuLine.empty()) {
                    string cvuDecoded = decodeHuffLine(cvuLine);
                    cout << "CVU: " << cvuDecoded << endl;
                }
                else {
                    cout << "CVU: (no disponible)" << endl;
                }

                // Decode owner
                if (!ownerLine.empty()) {
                    string ownerDecoded = decodeHuffLine(ownerLine);
                    cout << "Dueño: " << ownerDecoded << endl;
                }
                else {
                    cout << "Dueño: (no disponible)" << endl;
                }

                string contacto;
                cout << "\n\n  Ingrese un contacto del cual desee ver los datos: ";
                cin >> contacto;
                buscarYMostrarContacto(contacto, "BANCO");
            }
            else {
                cout << "Error: la línea de usuario fue reportada por la búsqueda, pero no se localizó en el archivo." << endl;
            }

            infile.close();
        }
        else {
            cout << "No se pudo abrir el archivo claves.txt para leer el saldo." << endl;
        }

    }
    else {
        cout << "Acceso denegado." << endl;
    }

    return 0;
}