#ifndef Affin.h
#define Affin
#include <string>
#include <iostream>
using namespace std;
static const int AFFINE_M = 95; // printable ASCII 32..126 inclusive

static int gcd_int(int x, int y) {
    if (y == 0) return abs(x);
    return gcd_int(y, x % y);
}

static int modInverseAffine(int a, int mval) {
    a = ((a % mval) + mval) % mval;
    for (int x = 1; x < mval; ++x) {
        if ((a * x) % mval == 1) return x;
    }
    return -1;
}

static string encryptAffine(const string& text, int a, int b) {
    string cipher;
    cipher.reserve(text.size());
    for (unsigned char ch : text) {
        if (ch >= 32 && ch <= 126) {
            int x = ch - 32;
            int encrypted = (a * x + b) % AFFINE_M;
            cipher += static_cast<char>(encrypted + 32);
        }
        else {
            cipher += ch;
        }
    }
    return cipher;
}

static string decryptAffine(const string& cipher, int a, int b) {
    string result;
    result.reserve(cipher.size());
    int a_inv = modInverseAffine(a, AFFINE_M);
    if (a_inv == -1) {
        cerr << "Error: a has no modular inverse (a=" << a << ", m=" << AFFINE_M << ")\n";
        return "";
    }
    for (unsigned char ch : cipher) {
        if (ch >= 32 && ch <= 126) {
            int y = ch - 32;
            int decrypted = (a_inv * (((y - b) % AFFINE_M + AFFINE_M) % AFFINE_M)) % AFFINE_M;
            result += static_cast<char>(decrypted + 32);
        }
        else {
            result += ch;
        }
    }
    return result;
}

#endif // !Affin.h
