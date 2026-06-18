#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

using namespace std;


struct NoHuffman {
    char caractere;
    int freq;
    NoHuffman* esq;
    NoHuffman* dir;
    NoHuffman(char c, int f) : caractere(c), freq(f), esq(nullptr), dir(nullptr) {}
};

struct compararNos {
    bool operator()(NoHuffman* esq, NoHuffman* dir){
        return esq->freq > dir->freq;
    }
};

#endif
