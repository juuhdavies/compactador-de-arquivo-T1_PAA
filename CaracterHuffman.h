#ifndef CARACTER_HUFFMAN_H
#define CARACTER_HUFFMAN_H

#include <string>

using namespace std;

namespace CharHuffman {
    //Executa a compressão por caractere
    void compress(const string& inputFile, const string& outputFile);

    //Executa a descompressão por caractere
    void decompress(const string& inputFile, const string& outputFile);
}

#endif