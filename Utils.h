#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace Utils {
    //escreve sequência de bits no arquivo de saída
    void escreverBitsArq(ofstream& outputFile, const string& bits);

    //lê sequência de bits do arquivo de entrada
    void lerBitsArq(ifstream& inputFile, string& bits, size_t numBits);

    //descarregamento dos bits restantes no buffer para o arquivo de saída
    void flushBitsArq(ofstream& outputFile, string& bits);

    //quebra palavras em tokens, manter pontuação e acentos
    vector<string> tokenizar(const string& inputFile);
}

#endif