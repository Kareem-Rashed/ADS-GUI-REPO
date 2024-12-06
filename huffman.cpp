// huffman.cpp

#include "Huffman.h"
#include <cmath>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cctype>

using namespace std;

Huffman::Huffman(string file): fileName(file){}

Huffman::~Huffman()
{

}

void Huffman::Frequencies(string file)
{
    ifstream infile(file);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file."<< endl;
    }

    unordered_map<char, int> freqmap;
    int total = 0;
    char ch;
    while (infile.get(ch))
    {

        if (freqmap.find(ch) == freqmap.end())
        {
            freqmap.insert(make_pair(ch, 1));
        }
        else
        {
            freqmap[ch]++;
        }
        total++;

    }
    infile.close();

    /*
    for (auto& it : freqmap) {
        if (std::isprint(it.first)) {
            std::cout << "'" << it.first << "' : " << it.second << '\n';
        } else {
            std::cout << "Character: " << ch << " ASCII: " << static_cast<int>(it.first) << '\n'
                      << " : " << it.second << '\n';
        }
    }
    cout<<"total: "<<total<<endl;
    */


    for (auto& it : freqmap)
    {
        pq.insert(it.first, it.second);
    }


}

void Huffman::buildHuffmanTree()
{

    while(pq.size>1)
    {
        Node<char> *FirstNode = pq.dequeue();
        Node<char> *SecondNode = pq.dequeue();


        Node<char> *ParentNode = new Node<char>( -999, FirstNode->priori + SecondNode->priori);
        ParentNode->right = SecondNode;
        ParentNode->left = FirstNode;
        pq.insert(ParentNode);
    }
    root = pq.dequeue();

}

void Huffman::CreateEncodings(Node<char>* temproot, string enc)
{
    if((temproot->left == nullptr)&&(temproot->right == nullptr))
    {
        vector<bool> newVector;

        for(int i=0; i<enc.size(); i++)
        {
            if(enc[i] == '1')
            {
                newVector.push_back(1);
            }
            else
            {
                newVector.push_back(0);
            }
        }
        Encodings.insert(make_pair(temproot->data, newVector));
        return;
    }
    else
    {
        CreateEncodings(temproot->left, enc + '0');
        CreateEncodings(temproot->right, enc + '1');
    }
}

void Huffman::EncodeTreeNormal(string file)
{
    ofstream outfile(file);

    Node<char>* temproot = pq.root;
    int size = pq.size;
    string sizeString = to_string(size);
    outfile<<sizeString;
    int charnum = 0;

    while(temproot!=NULL)
    {
        char ch = temproot->data;
        int freq = temproot->priori;
        outfile<<ch;
        outfile<<to_string(freq);
        charnum++;
        temproot = temproot->next;

    }
    outfile<<'\n';
    outfile.close();

}
void Huffman::EncodeTreeCaeser(string file)
{
    ofstream outfile(file);
    int randcypher = rand() % 4 + 1;

    outfile<<to_string(randcypher);

    Node<char>* temproot = pq.root;

    while(temproot!=NULL)
    {
        char ch = temproot->data;
        int freq = temproot->priori;
        char newch = (ch - randcypher+256)%256;
        outfile<<newch;
        outfile<<to_string(freq);
        temproot = temproot->next;

    }
    outfile<<'\n';
    outfile.close();

}

void Huffman::ReadTreeNormal(string file)
{
    chars = 0;
    int pqsize;
    string pqsizestring = "";
    ifstream infile1(file);
    int numchar = 0;
    char pad;

    while(isdigit(infile1.peek()))
    {
        pqsizestring += infile1.get();
    }
    pqsize = stoi(pqsizestring);

    while(infile1.peek()!= EOF)
    {
        pad = infile1.get();
    }
    padding = pad - '0';
    infile1.close();

    ifstream infile(file);

    for(int i=0; i<pqsizestring.size(); i++)
    {
        infile.get();
        chars++;
    }


    while(pq.size<pqsize)
    {
        char ch = infile.get();
        chars++;
        string num = "";
        while(isdigit(infile.peek()))
        {
            num+=infile.get();
            chars++;
        }
        if(num!="")
        {
            long intnum = stol(num);

            pq.insert(ch, intnum);
            numchar++;
        }
        else
        {
            break;
        }

    }



}

void Huffman::ReadTreeCaeser(string file)
{
    chars = 0;

    // First, read the entire file to extract padding
    ifstream infile1(file);

    char pad;
    while (infile1.peek() != EOF)
    {
        pad = infile1.get();
    }
    padding = pad - '0';

    infile1.close();

    // Open the file again to process the content
    ifstream infile(file);

    // Read the cipher key
    char cypher = infile.get();
    chars++;

    int cypherkey = cypher - '0';

    while (infile.peek() != '\n')
    {
        chars++;
        char wrongch = infile.get();

        // Reverse the Caesar cipher transformation
        char ch = (wrongch + cypherkey) % 256;

        // Ensure the result stays within the ASCII range
        if (ch < 0) ch += 256;

        string num = "";
        while (isdigit(infile.peek()))
        {
            chars++;
            num += infile.get();
        }

        if (num != "")
        {
            int intnum = stoi(num);
            pq.insert(ch, intnum);
        }
        else
        {
            break;
        }
    }
}

void Huffman::Compress(string type, string OutputFileName) {
    Frequencies(fileName);
    if(type == "Caesar")
    {
        EncodeTreeCaeser(OutputFileName);
    }
    else if(type == "Normal")
    {
        EncodeTreeNormal(OutputFileName);
    }
    else
    {
        cerr<<"You need to choose either caeser or normal encryption"<<endl;
    }

    buildHuffmanTree();

    CreateEncodings(root, "");


    ifstream infile(fileName, ios::binary);
    ofstream outfile(OutputFileName, ios::app);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open input file '" << fileName << "'." << endl;
        return;
    }

    unsigned char buffer = 0;
    int bitCount = 0;

    char ch;
    while (infile.get(ch)) {
        vector<bool> encoding = Encodings[ch];

        for (bool bit : encoding) {
            buffer = buffer * 2 + bit;
            bitCount++;

            if (bitCount == 8) {
                outfile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    //the rest
    if (bitCount > 0) {
        padding = 8-bitCount;
        buffer = buffer << (8 - bitCount);
        outfile.put(buffer);
    }
    outfile<<to_string(padding);


    infile.close();
    outfile.close();
}
void Huffman::Decompress(string type, string OutputFileName) {
    if(type == "Caesar")
    {
        ReadTreeCaeser(fileName);
    }
    else if(type == "Normal")
    {
        ReadTreeNormal(fileName);
    }
    else
    {
        cerr<<"You need to choose either caeser or normal encryption"<<endl;
    }
    buildHuffmanTree();

    ifstream infile(fileName, ios::binary);
    ofstream outfile(OutputFileName);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open compressed file." << endl;
        return;
    }

    for(int i=0; i<chars+1; i++)
    {
        infile.get();
    }


    char ch;
    unsigned char buffer;

    Node<char>* currentNode = root;

    while (infile.get(reinterpret_cast<char&>(buffer))) {
        for (int i = 7; i >= 0; i--) {

            bool bit = (buffer >> i) & 1;  //getting the ith bit
            if(bit)
            {
                currentNode = currentNode->right;
            }
            else
            {
                currentNode = currentNode->left;
            }
            if ((currentNode->left == NULL)&& (currentNode->right == NULL)) {

                bool lastchar = false;
                char ch = infile.get();
                if(infile.peek()==EOF)
                {
                    lastchar = true;
                }
                streampos currentPosition = infile.tellg();
                infile.seekg(currentPosition -  static_cast<streamoff>(1));
                if (lastchar && i < padding)
                {

                    return;
                }
                else
                {
                    outfile.put(currentNode->data);
                    currentNode = root;
                }
            }
        }
    }

    infile.close();
    outfile.close();
}
