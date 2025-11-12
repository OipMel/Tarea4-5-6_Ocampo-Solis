#ifndef Caesar_Code
#define Caesar_Code
#include <string>
using namespace std;

inline string encrypt(string text, int s)
{
    string result = "";
    for (int i = 0; i < (int)text.length(); i++) {
        unsigned char ch = (unsigned char)text[i];
        if (isdigit(ch)) {
            int base = 48;
            int shifted = ((int)ch - base + s) % 10;
            if (shifted < 0) shifted += 10;
            result += char(shifted + base);
        }
        else if (isupper(ch)) {
            int base = 65;
            int shifted = ((int)ch - base + s) % 26;
            if (shifted < 0) shifted += 26;
            result += char(shifted + base);
        }
        else if (islower(ch)) {
            int base = 97;
            int shifted = ((int)ch - base + s) % 26;
            if (shifted < 0) shifted += 26;
            result += char(shifted + base);
        }
        else
            result += text[i];
    }
    return result;
}

inline string decrypt(string text, int s)
{
    string result = "";
    for (int i = 0; i < (int)text.length(); i++) {
        unsigned char ch = (unsigned char)text[i];
        if (isdigit(ch)) {
            int base = 48;
            int shifted = ((int)ch - base - s) % 10;
            if (shifted < 0) shifted += 10;
            result += char(shifted + base);
        }
        else if (isupper(ch)) {
            int base = 65;
            int shifted = ((int)ch - base - s) % 26;
            if (shifted < 0) shifted += 26;
            result += char(shifted + base);
        }
        else if (islower(ch)) {
            int base = 97;
            int shifted = ((int)ch - base - s) % 26;
            if (shifted < 0) shifted += 26;
            result += char(shifted + base);
        }
        else
            result += text[i];
    }
    return result;
}

#endif // !Caesar_Code