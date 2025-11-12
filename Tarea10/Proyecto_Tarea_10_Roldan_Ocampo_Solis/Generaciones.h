#ifndef Generaciones.h
#define Generaciones
#include <string>
#include <random>
using namespace std;

static string generateAliasFromSet(mt19937& gen) {
    static const vector<string> words = {
        "earth","penguin","ice","juice","corn","royale","pen","helicopter","bacon","salt"
    };
    vector<string> pool = words;
    shuffle(pool.begin(), pool.end(), gen);
    // join first 3 by underscores to keep single token
    string alias = pool[0] + "_" + pool[1] + "_" + pool[2];
    return alias;
}

// Generate a 22-digit CVU (string of digits 0-9)
static string generateCvu22(mt19937& gen) {
    uniform_int_distribution<int> digitDist(0, 9);
    string cvu;
    cvu.reserve(22);
    for (int i = 0; i < 22; ++i) cvu.push_back(char('0' + digitDist(gen)));
    return cvu;
}

inline string trim_cr(const string& s) {
    if (!s.empty() && s.back() == '\r') return s.substr(0, s.size() - 1);
    return s;
}

static int parseFreqHeader(const string& header, map<char, int>& outFmap, int& outAffA, int& outAffB) {
    outFmap.clear();
    outAffA = -1;
    outAffB = -1;
    int pad = -1;
    size_t pos = 0;
    while (pos < header.size()) {
        size_t semi = header.find(';', pos);
        if (semi == string::npos) semi = header.size();
        string token = header.substr(pos, semi - pos);
        pos = semi + 1;
        if (token.empty()) continue;
        if (token.rfind("pad=", 0) == 0) {
            pad = atoi(token.substr(4).c_str());
            continue;
        }
        if (token.rfind("aff=", 0) == 0) {
            string rest = token.substr(4);
            size_t comma = rest.find(',');
            if (comma == string::npos) return -1;
            outAffA = atoi(rest.substr(0, comma).c_str());
            outAffB = atoi(rest.substr(comma + 1).c_str());
            continue;
        }
        // token like "<ascii>,<count>"
        size_t comma = token.find(',');
        if (comma == string::npos) return -1;
        int ascii = atoi(token.substr(0, comma).c_str());
        int cnt = atoi(token.substr(comma + 1).c_str());
        outFmap[(char)ascii] = cnt;
    }
    return pad;
}

static string serializeFreqHeader(const map<char, int>& fmap, int pad, int affA = -1, int affB = -1) {
    stringstream ss;
    ss << "pad=" << pad << ";";
    if (affA != -1 && affB != -1) {
        ss << "aff=" << affA << "," << affB << ";";
    }
    for (auto& p : fmap) {
        ss << (int)(unsigned char)p.first << "," << p.second << ";";
    }
    return ss.str();
}

#endif // !Generaciones.h
