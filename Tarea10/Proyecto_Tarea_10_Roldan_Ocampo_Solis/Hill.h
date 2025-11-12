#ifndef Hill.h
#define Hill
#include <unordered_map>
#include <string>
using namespace std;

inline int comprobar(int x) {
    if (x % 3 == 0)
        return 0;
    int a = x / 3;
    int b = 3 * (a + 1);
    int c = b - x;
    return c;
}

// Helper: mod 26 normalized to [0,25]
static int mod26(int x) {
    x %= 26;
    if (x < 0) x += 26;
    return x;
}

// Hill cipher functions
static int modInverseNumber(int a, int m) {
    int m0 = m, x0 = 0, x1 = 1;
    if (m == 1) return 0;
    a = (a % m + m) % m;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m; a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
        if (m == 0) break;
    }
    if (a != 1) return -1;
    int inv = (x1 % m0 + m0) % m0;
    return inv;
}
static bool inverseMatrix3x3Mod26(const int mat[3][3], int inv[3][3]) {
    int det =
        mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
        mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
        mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    det = mod26(det);
    int detInv = modInverseNumber(det, 26);
    if (detInv == -1) return false;
    int adj[3][3];
    adj[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    adj[0][1] = -(mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]);
    adj[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]);

    adj[1][0] = -(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
    adj[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]);
    adj[1][2] = -(mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]);

    adj[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    adj[2][1] = -(mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]);
    adj[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            inv[i][j] = mod26(adj[i][j] * detInv);
    return true;
}
static const int HILL_KEY[3][3] = {
    {6, 24, 1},
    {13,16,10},
    {20,17,15}
};
static const unordered_map<char, char> charToLetter = {
    {'0','A'}, {'1','B'}, {'2','C'}, {'3','D'}, {'4','E'},
    {'5','F'}, {'6','G'}, {'7','H'}, {'8','I'}, {'9','J'},
    {',','K'}, {'.','L'}
};
static unordered_map<char, char> buildLetterToChar() {
    unordered_map<char, char> rev;
    for (auto& p : charToLetter) rev[p.second] = p.first;
    return rev;
}
static const unordered_map<char, char> letterToChar = buildLetterToChar();

static string hill(const string& money_line) {
    if (money_line.empty()) return money_line;
    string payload = money_line;
    if (payload[0] == '$') payload = payload.substr(1);
    string letters;
    for (char c : payload) {
        auto it = charToLetter.find(c);
        if (it != charToLetter.end()) letters.push_back(it->second);
        else letters.push_back('X');
    }
    int pad = (3 - (letters.size() % 3)) % 3;
    for (int i = 0; i < pad; ++i) letters.push_back('X');

    string encoded;
    for (size_t pos = 0; pos < letters.size(); pos += 3) {
        int p0 = letters[pos + 0] - 'A';
        int p1 = letters[pos + 1] - 'A';
        int p2 = letters[pos + 2] - 'A';
        int c0 = mod26(HILL_KEY[0][0] * p0 + HILL_KEY[0][1] * p1 + HILL_KEY[0][2] * p2);
        int c1 = mod26(HILL_KEY[1][0] * p0 + HILL_KEY[1][1] * p1 + HILL_KEY[1][2] * p2);
        int c2 = mod26(HILL_KEY[2][0] * p0 + HILL_KEY[2][1] * p1 + HILL_KEY[2][2] * p2);
        encoded.push_back(char('A' + c0));
        encoded.push_back(char('A' + c1));
        encoded.push_back(char('A' + c2));
    }
    return string("MHILL:") + encoded;
}
static string money_decode(const string& encoded) {
    const string prefix = "MHILL:";
    if (encoded.rfind(prefix, 0) != 0) return encoded;
    string body = encoded.substr(prefix.size());
    if (body.size() % 3 != 0) return string("$") + encoded;
    int invKey[3][3];
    if (!inverseMatrix3x3Mod26(HILL_KEY, invKey)) return string("$") + encoded;
    string letters;
    for (size_t pos = 0; pos < body.size(); pos += 3) {
        int c0 = body[pos + 0] - 'A';
        int c1 = body[pos + 1] - 'A';
        int c2 = body[pos + 2] - 'A';
        int p0 = mod26(invKey[0][0] * c0 + invKey[0][1] * c1 + invKey[0][2] * c2);
        int p1 = mod26(invKey[1][0] * c0 + invKey[1][1] * c1 + invKey[1][2] * c2);
        int p2 = mod26(invKey[2][0] * c0 + invKey[2][1] * c1 + invKey[2][2] * c2);
        letters.push_back(char('A' + p0));
        letters.push_back(char('A' + p1));
        letters.push_back(char('A' + p2));
    }
    string out;
    for (char L : letters) {
        if (L == 'X') break;
        auto it = letterToChar.find(L);
        if (it != letterToChar.end()) out.push_back(it->second);
        else out.push_back('?');
    }
    return string("$") + out;
}
#endif // !Hill.h
