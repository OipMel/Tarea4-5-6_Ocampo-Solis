#ifndef Huffman_Code
#define Huffman_Code
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
using namespace std;

#define MAX_TREE_HT 256

struct MinHeapNode {
    char data; // One of the input characters
    int freq; // Frequency of the character
    MinHeapNode* left, * right; // Left and right child

    inline MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// to map each character its huffman value
inline map<char, string> codes;

// To store the frequency of character of the input data
inline map<char, int> freq;

// utility function for the priority queue
struct compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

// utility function to print characters along with
// their huffman value
inline void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// utility function to store characters along with
// their huffman value in a hash table
inline void storeCodes(struct MinHeapNode* root, string str)
{
    if (root == NULL)
        return;
    if (root->data != '$')
        codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

// priority queue to store heap tree nodes
extern priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare>
minHeap;

// function to build the Huffman tree and store it in minHeap
inline void HuffmanCodes(int size)
{
    struct MinHeapNode* left, * right, * top;
    for (map<char, int>::iterator v = freq.begin();
        v != freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
    if (minHeap.empty()) return;
    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
        if (minHeap.empty()) break;
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$',
            left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    if (!minHeap.empty())
        storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its frequency in input string
inline void calcFreq(const string& str)
{
    for (size_t i = 0; i < str.size(); i++)
        freq[str[i]]++;
}

inline string decode_file(struct MinHeapNode* root, string s)
{
    string ans = "";
    if (!root) return ans;
    struct MinHeapNode* curr = root;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '0') {
            if (curr->left) curr = curr->left;
            else { curr = root; continue; }
        }
        else {
            if (curr->right) curr = curr->right;
            else { curr = root; continue; }
        }

        // reached leaf node
        if (curr->left == NULL && curr->right == NULL) {
            ans += curr->data;
            curr = root;
        }
    }
    return ans;
}




#endif