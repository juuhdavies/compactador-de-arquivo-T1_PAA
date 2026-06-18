#ifndef PALAVRA_HUFFMAN_H
#define PALAVRA_HUFFMAN_H

#include <string>

using namespace std;

namespace WordHuffman {
    //Executa a compressão por palavra
    void compress(const string& inputFile, const string& outputFile);

    //Executa a descompressão por palavra
    void decompress(const string& inputFile, const string& outputFile);
}

#endif