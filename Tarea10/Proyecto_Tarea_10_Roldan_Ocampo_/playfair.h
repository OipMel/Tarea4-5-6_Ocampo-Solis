#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class PlayfairCipher {
private:
    vector<vector<char>> matrix;
    string key;

    void generateMatrix() {
        string tempKey = key;
        tempKey.erase(remove(tempKey.begin(), tempKey.end(), ' '), tempKey.end());

        string cleanKey;
        for (char c : tempKey) {
            c = toupper(c);
            if (c != 'Ñ' && isalpha(c)) cleanKey += c;
        }

        string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        base.erase(remove(base.begin(), base.end(), 'Ñ'), base.end());

        string used;
        for (char c : cleanKey + base) {
            if (used.find(c) == string::npos) used += c;
        }

        matrix.resize(5, vector<char>(5));
        int idx = 0;
        for (int row = 0; row < 5; ++row)
            for (int col = 0; col < 5; ++col)
                matrix[row][col] = used[idx++];
    }

    pair<int, int> findPosition(char c) {
        for (int row = 0; row < 5; ++row)
            for (int col = 0; col < 5; ++col)
                if (matrix[row][col] == c)
                    return { row, col };
        return { -1, -1 };
    }

    string prepareText(string text) {
        string result;
        for (char c : text) {
            c = toupper(c);
            if (isalpha(c) && c != 'Ñ') result += c;
        }

        string prepared;
        for (size_t i = 0; i < result.length(); i += 2) {
            char first = result[i];
            char second = (i + 1 < result.length()) ? result[i + 1] : 'X';
            if (first == second) {
                prepared += first;
                prepared += 'X';
                i--;
            }
            else {
                prepared += first;
                prepared += second;
            }
        }

        return prepared;
    }

public:
    PlayfairCipher(string k) : key(k) {
        generateMatrix();
    }

    string encrypt(string text) {
        string prepared = prepareText(text);
        string encrypted;

        for (size_t i = 0; i < prepared.length(); i += 2) {
            char a = prepared[i];
            char b = prepared[i + 1];
            auto [row1, col1] = findPosition(a);
            auto [row2, col2] = findPosition(b);

            if (row1 == row2) {
                encrypted += matrix[row1][(col1 + 1) % 5];
                encrypted += matrix[row2][(col2 + 1) % 5];
            }
            else if (col1 == col2) {
                encrypted += matrix[(row1 + 1) % 5][col1];
                encrypted += matrix[(row2 + 1) % 5][col2];
            }
            else {
                encrypted += matrix[row1][col2];
                encrypted += matrix[row2][col1];
            }
        }

        return encrypted;
    }

    string decrypt(const string& text) {
        string clean;
        for (char c : text) {
            c = toupper(c);
            if (isalpha(c) && c != 'Ñ') clean += c;
        }

        if (clean.length() % 2 != 0) clean += 'X';
        string decrypted;

        for (size_t i = 0; i < clean.length(); i += 2) {
            char a = clean[i];
            char b = clean[i + 1];
            auto [row1, col1] = findPosition(a);
            auto [row2, col2] = findPosition(b);

            if (row1 == row2) {
                decrypted += matrix[row1][(col1 + 4) % 5];
                decrypted += matrix[row2][(col2 + 4) % 5];
            }
            else if (col1 == col2) {
                decrypted += matrix[(row1 + 4) % 5][col1];
                decrypted += matrix[(row2 + 4) % 5][col2];
            }
            else {
                decrypted += matrix[row1][col2];
                decrypted += matrix[row2][col1];
            }
        }

        return decrypted;
    }

    void printMatrix() {
        for (const auto& row : matrix) {
            for (char c : row) cout << c << ' ';
            cout << endl;
        }
    }
};

#endif // PLAYFAIR_H
