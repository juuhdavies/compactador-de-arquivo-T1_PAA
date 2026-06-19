#include "Utils.h"

using namespace std;

namespace Utils {
    // Implementação das funções declaradas em Utils.h

    //Ler os bits do arquivo de entrada
    void lerBitsArq(ifstream& inputFile, string& bits) {
        char byte;
        while (inputFile.get(byte)) {
            // Lê cada um dos 8 bits do byte lido
            for (int i = 7; i >= 0; i--) {
                //verificar se o i-ésimo bit é 1 ou 0
                if ((byte >> i) & 1) {
                    bits += '1';
                } else {
                    bits += '0';
                }
            }
        }
    }

    //Escrever os bits do arquivo de saída
    void escreverBitsArq(ofstream& outputFile, const string& bits, unsigned char& buffer, int& bitsPreenchidos) {
        for (char bit : bits) {
            buffer <<= 1; // Desloca o buffer para a esquerda
            if (bit == '1') {
                buffer |= 1; // Define o bit mais à direita se for '1'
            }
            bitsPreenchidos++;

            if (bitsPreenchidos == 8) { // Se o buffer estiver cheio, escreve no arquivo
                outputFile.put(buffer);
                buffer = 0;
                bitsPreenchidos = 0;
            }
        }
    }

    void flushBitsArq(ofstream& outputFile, unsigned char& buffer, int& bitsPreenchidos) {
        if (bitsPreenchidos > 0) {
            buffer <<= (8 - bitsPreenchidos);
            outputFile.put(static_cast<char>(buffer));
            buffer = 0;
            bitsPreenchidos = 0;
        }
    }

    //Tokenizar o arquivo de entrada, para compressão por palavra
    vector<string> tokenizar(const string& inputFile) {
        vector<string> tokens;
        ifstream file(inputFile);
        string word;

        while (file >> word) {
            tokens.push_back(word);
        }

        return tokens;
    }


}