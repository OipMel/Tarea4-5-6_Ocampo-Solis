#ifndef Encripcion
#define Encripcion
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <string_view>
#include <sstream>
#include <queue>
#include <vector>
#include <random>
#include "Huffman.h"
#include "Cesar.h"
#include "Hill.h"
#include "Affin.h"
#include "Generaciones.h"

using namespace std;

inline void Encriptar_Comprimir() {
    // Abrir archivo original
    ifstream lee("Usuario.txt", ios::in);
    if (!lee.is_open()) {
        cout << "Error opening Usuario.txt" << endl;
        return;
    }

    // Leer primera línea: si es la flag "PROCESSED" no procesamos
    string firstLine;
    if (!getline(lee, firstLine)) { // archivo vacío
        lee.close();
        return;
    }
    firstLine = trim_cr(firstLine);
    if (firstLine == "PROCESSED") {

        lee.close();
        return; // ya procesado, salir
    }

    // No estaba procesado: reconstruir vector de líneas empezando por la primera leída
    vector<string> lines;
    lines.push_back(firstLine);
    string line;
    while (getline(lee, line)) {
        lines.push_back(trim_cr(line));
    }
    lee.close();

    ofstream aux("auxiliar.txt", ios::out);
    if (!aux.is_open()) {
        cout << "Error opening auxiliar.txt" << endl;
        return;
    }

    // Escribimos la flag como primera línea del archivo resultante
    aux << "PROCESSED" << "\n";

    random_device rd;
    mt19937 gen(rd());
    // Use affine for alias, cvu and owner. Choose a fixed 'a' coprime with 95 and random b.
    const int affineA = 11; // 11 is coprime with 95
    uniform_int_distribution<int> bdist(0, AFFINE_M - 1);

    // Process input in blocks of 4: username, passcode, money, owner
    for (size_t idx = 0; idx < lines.size(); idx += 4) {
        string usernameLine = (idx < lines.size()) ? lines[idx] : "";
        string passcodeLine = (idx + 1 < lines.size()) ? lines[idx + 1] : "";
        string moneyLine = (idx + 2 < lines.size()) ? lines[idx + 2] : "";
        string ownerLine = (idx + 3 < lines.size()) ? lines[idx + 3] : "";

        // Build user+pass
        string userAndPass;
        if (!usernameLine.empty() && !passcodeLine.empty()) {
            if (usernameLine.find(',') != string::npos && passcodeLine.empty()) userAndPass = usernameLine;
            else userAndPass = usernameLine + "," + passcodeLine;
        }
        else if (!usernameLine.empty()) userAndPass = usernameLine;
        else userAndPass = "";

        // USERNAME: keep same behavior so search works: encrypt then build Huffman bitstring and write raw bitstring (no header)
        if (!userAndPass.empty()) {
            string to_Encode = encrypt(userAndPass, 4);
            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
            calcFreq(to_Encode);
            HuffmanCodes((int)to_Encode.length());

            string Usuario_ClaveAux;
            for (auto ch : to_Encode) Usuario_ClaveAux += codes[ch];
            aux << Usuario_ClaveAux << "\n";
        }
        else {
            aux << "\n";
        }

        if (!moneyLine.empty()) {
            string money_for_hill = moneyLine;
            if (money_for_hill[0] != '$') money_for_hill = "$" + money_for_hill;
            string encodedMoney = hill(money_for_hill);
            aux << encodedMoney << "\n";
        }
        else {
            aux << "\n";
        }

        string aliasPlain = generateAliasFromSet(gen);
        string cvuPlain = generateCvu22(gen);

        {
            int affB = bdist(gen);
            string encrypted = encryptAffine(aliasPlain, affineA, affB);
            // build freq/codes for this encrypted string
            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
            calcFreq(encrypted);
            HuffmanCodes((int)encrypted.length());

            // build bitstring (Huffman)
            string bitstr;
            for (unsigned char ch : encrypted) bitstr += codes[ch];

            // header = serialized freq + pad + affine keys; include header length
            map<char, int> fmap;
            for (auto& p : freq) fmap[p.first] = p.second;
            int pad = 0; // no packing -> pad = 0
            string header = serializeFreqHeader(fmap, pad, affineA, affB);

            // store header + raw bitstring (no Base64 / no packing)
            aux << "HUFF:" << header.size() << ":" << header << bitstr << "\n";

            // clear huffman globals
            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
        }

        // For CVU: same pipeline
        {
            int affB = bdist(gen);
            string encrypted = encryptAffine(cvuPlain, affineA, affB);
            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
            calcFreq(encrypted);
            HuffmanCodes((int)encrypted.length());

            string bitstr;
            for (unsigned char ch : encrypted) bitstr += codes[ch];

            map<char, int> fmap;
            for (auto& p : freq) fmap[p.first] = p.second;
            int pad = 0;
            string header = serializeFreqHeader(fmap, pad, affineA, affB);
            aux << "HUFF:" << header.size() << ":" << header << bitstr << "\n";

            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
        }

        // For OWNER: read ownerLine, encrypt then Huffman compress similarly
        if (!ownerLine.empty()) {
            int affB = bdist(gen);
            string encrypted = encryptAffine(ownerLine, affineA, affB);
            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
            calcFreq(encrypted);
            HuffmanCodes((int)encrypted.length());

            string bitstr;
            for (unsigned char ch : encrypted) bitstr += codes[ch];

            map<char, int> fmap;
            for (auto& p : freq) fmap[p.first] = p.second;
            int pad = 0;
            string header = serializeFreqHeader(fmap, pad, affineA, affB);
            aux << "HUFF:" << header.size() << ":" << header << bitstr << "\n";

            freq.clear(); codes.clear();
            while (!minHeap.empty()) minHeap.pop();
        }
        else {
            aux << "\n";
        }
    }

    aux.close();

    // Replace original file with processed file
    remove("Usuario.txt");
    rename("auxiliar.txt", "Usuario.txt");
}
#endif // !Encripcion