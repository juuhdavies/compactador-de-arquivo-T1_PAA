#ifndef PALAVRA_HUFFMAN_H
#define PALAVRA_HUFFMAN_H

#include <string>
#include <unordered_map>
#include "NoHuffman.h"

using namespace std;

namespace WordHuffman {
    //Executa a compressão por palavra
    void compress(const string& inputFile, const string& outputFile);

    //monta a árvore de huffman a partir da tabela de frequência para palavras
    NoHuffmanPalavra* montarArvHuffman(const unordered_map<string, int>& freq);

    //libera memoria da árvore de huffman
    void liberarArv(NoHuffman* raiz);

    //gera cod Huffman para cada palavra da árvore
    void gerarCodHuffman(NoHuffmanPalavra* raiz, string codAtual, unordered_map<string, string>& codigos);

    //Monta tabela de frequência das palavras
    void montarTabFreq(const string& inputFile, unordered_map<string, int>& freq);

    //Executa a descompressão por palavra
    void decompress(const string& inputFile, const string& outputFile);
}

#endif