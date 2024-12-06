// huffman.h
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "PriorityQueue.h"
#include "Node.h"

using namespace std;

class Huffman {
private:
    string fileName;
    int padding;
    int chars;
    Node<char>* root;
    PriorityQueue<char> pq;
    unordered_map<char, vector<bool>> Encodings;

public:
    Huffman(string file);
    ~Huffman();
    void Frequencies(string file);
    void buildHuffmanTree();
    void CreateEncodings(Node<char>* temproot, string enc);
    void Compress(string type, string outputFileName);
    void Decompress(string type, string outputFileName);
    void EncodeTreeNormal(string file);
    void EncodeTreeCaeser(string file);
    void ReadTreeNormal(string file);
    void ReadTreeCaeser(string file);
    // Added method declarations for no encryption
    void EncodeTreeNone(string file);
    void ReadTreeNone(string file);
};

#endif // HUFFMAN_H
