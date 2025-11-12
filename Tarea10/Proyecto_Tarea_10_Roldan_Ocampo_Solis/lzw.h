#ifndef LZW_H
#define LZW_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

// Comprime una cadena usando LZW
inline vector<int> LZW_compress(const string& input) {
    unordered_map<string, int> dict;
    for (int i = 0; i < 256; i++) {
        dict[string(1, char(i))] = i;
    }

    string w;
    vector<int> result;
    int code = 256;

    for (char c : input) {
        string wc = w + c;
        if (dict.count(wc)) {
            w = wc;
        }
        else {
            result.push_back(dict[w]);
            dict[wc] = code++;
            w = string(1, c);
        }
    }

    if (!w.empty()) result.push_back(dict[w]);
    return result;
}

// Descomprime una secuencia de códigos LZW
inline string LZW_decompress(const vector<int>& compressed) {
    unordered_map<int, string> dict;
    for (int i = 0; i < 256; i++) {
        dict[i] = string(1, char(i));
    }

    string w = dict[compressed[0]];
    string result = w;
    int code = 256;

    for (size_t i = 1; i < compressed.size(); i++) {
        int k = compressed[i];
        string entry;

        if (dict.count(k)) {
            entry = dict[k];
        }
        else if (k == code) {
            entry = w + w[0];
        }
        else {
            return "(LZW error)";
        }

        result += entry;
        dict[code++] = w + entry[0];
        w = entry;
    }

    return result;
}

// Mostrar el vector comprimido
inline void mostrarLZW(const vector<int>& comp) {
    cout << "LZW comprimido: ";
    for (int c : comp) cout << c << ' ';
    cout << endl;
}

#endif // LZW_H
