#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H
#include <string>

using namespace std;

//Para Caracteres
/**
 * @brief Estrutura de dados que representa um nó da árvore de Huffman para Caracteres.
 */
struct NoHuffman {
    char caractere;
    int freq;
    NoHuffman* esq;
    NoHuffman* dir;
    NoHuffman(char c, int f) : caractere(c), freq(f), esq(nullptr), dir(nullptr) {}
};

/**
 * @brief Funcão para comparar dois nós de Huffman com base em suas frequências.
 */
struct compararNos {
    bool operator()(NoHuffman* esq, NoHuffman* dir){
        return esq->freq > dir->freq;
    }
};

//Para Palavras
/**
 * @brief Estrutura de dados que representa um nó da árvore de Huffman para Palavras.
 */
struct NoHuffmanPalavra {
    string palavra;
    int freq;
    NoHuffmanPalavra* esq;
    NoHuffmanPalavra* dir;
    NoHuffmanPalavra(string p, int f) : palavra(p), freq(f), esq(nullptr), dir(nullptr) {}
};

/**
 * @brief Funcão para comparar dois nós de HuffmanPalavra com base em suas frequências.
 */
struct compararNosPalavra {
    bool operator()(NoHuffmanPalavra* esq, NoHuffmanPalavra* dir){
        return esq->freq > dir->freq;
    }
};

#endif
