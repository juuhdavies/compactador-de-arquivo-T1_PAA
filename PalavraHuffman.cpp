#include "NoHuffman.h"
#include "Utils.h"
#include "PalavraHuffman.h"
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;

namespace WordHuffman {
    void montarTabFreq(const string& inputFile, unordered_map<string, int>& freq) {
        vector<string> tokens = Utils::tokenizar(inputFile);
        for (const string& token : tokens) {
            freq[token]++;
        }
    }

    void liberarArv(NoHuffmanPalavra* raiz) {
        if (raiz) {
            liberarArv(raiz->esq);
            liberarArv(raiz->dir);
            delete raiz;
        }
    }

    void gerarCodHuffman(NoHuffmanPalavra* raiz, string codAtual, unordered_map<string, string>& codigos) {
        if (!raiz) return;

        // Se for um nó folha, armazena o código Huffman para a palavra
        if (!raiz->esq && !raiz->dir) {
            codigos[raiz->palavra] = codAtual;
        }

        gerarCodHuffman(raiz->esq, codAtual + "0", codigos);
        gerarCodHuffman(raiz->dir, codAtual + "1", codigos);
    }

    NoHuffmanPalavra* montarArvHuffman(const unordered_map<string, int>& freq) {
        priority_queue<NoHuffmanPalavra*, vector<NoHuffmanPalavra*>, compararNosPalavra> fila;
        for (const auto& par : freq) { // Para cada palavra e sua frequência na tabela de frequência, cria um nó folha e o adiciona à fila de prioridade
            fila.push(new NoHuffmanPalavra(par.first, par.second));
        }
        if (fila.empty()) return nullptr; // Se a fila estiver vazia, retorna null

        while(fila.size() > 1) { // Enquanto houver mais de um nó na fila, remove os dois nós com menor frequência
            NoHuffmanPalavra* esq = fila.top(); fila.pop();
            NoHuffmanPalavra* dir = fila.top(); fila.pop();
            NoHuffmanPalavra* novoNo = new NoHuffmanPalavra("", esq->freq + dir->freq);// Cria um novo nó interno com frequência igual à soma das frequências dos dois nós removidos
            novoNo->esq = esq;
            novoNo->dir = dir;
            fila.push(novoNo);// Adiciona o novo nó de volta à fila
        }

        return fila.top(); // O nó restante na fila é a raiz da árvore de Huffman
    }

    void compress(const string& inputFile, const string& outputFile) {
        unordered_map<string, int> freq;
        montarTabFreq(inputFile, freq);

        NoHuffmanPalavra* raiz = montarArvHuffman(freq);
        if (raiz == nullptr) {
            cerr << "Erro: O arquivo de entrada esta vazio ou corrompido." << endl;
            return;
        }

        unordered_map<string, string> codigos;
        gerarCodHuffman(raiz, "", codigos);

        ofstream output(outputFile, ios::binary);

        size_t qtdPalavras = freq.size();
        output.write(reinterpret_cast<const char*>(&qtdPalavras), sizeof(size_t));

        for (const auto& par : freq) {
            output.write(par.first.c_str(), par.first.size() + 1); // Escreve a palavra e \0
            output.write(reinterpret_cast<const char*>(&par.second), sizeof(int)); // Escreve a frequência
        }


        ifstream input(inputFile);
        string word;
        unsigned char buffer = 0;
        int bitsPreenchidos = 0;

        while (input >> word) {
            const string& codigo = codigos[word];
            Utils::escreverBitsArq(output, codigo, buffer, bitsPreenchidos);
        }
        Utils::flushBitsArq(output, buffer, bitsPreenchidos);
        liberarArv(raiz);
        input.close();
        output.close();
        cout << "Compactacao concluida!" << endl;
    }

    void decompress(const string& inputFile, const string& outputFile) {
        ifstream input(inputFile, ios::binary);
        ofstream output(outputFile, ios::binary);

        unordered_map<string, int> freq;
        size_t qtdPalavras;
        input.read(reinterpret_cast<char*>(&qtdPalavras), sizeof(size_t)); // Lê a quantidade de palavras na tabela de frequência

        long long totalPalavras = 0;

        for (size_t i = 0; i < qtdPalavras; ++i) {
            string palavra;
            getline(input, palavra, '\0'); // Lê a palavra até o caractere nulo
            
            int frequencia;
            input.read(reinterpret_cast<char*>(&frequencia), sizeof(int)); // Lê a frequência
            
            freq[palavra] = frequencia;
            totalPalavras += frequencia;
        }



        NoHuffmanPalavra* raiz = montarArvHuffman(freq);
        if (raiz == nullptr) {
            cerr << "Erro: O arquivo de entrada esta vazio ou corrompido." << endl;
            return;
        }

        unordered_map<string, string> codigos;
        gerarCodHuffman(raiz, "", codigos);

        string bits;
        Utils::lerBitsArq(input, bits); // Lê os bits codificados do arquivo de entrada
        long long totalDecodificados = 0;


        NoHuffmanPalavra* atual = raiz;
        for (char bit : bits) {
            if (bit == '0') {
                atual = atual->esq; // Vai para a subárvore esquerda
            } else {
                atual = atual->dir; // Vai para a subárvore direita
            }

            if (!atual->esq && !atual->dir) { // Se chegar em um nó folha, escreve a palavra correspondente no arquivo de saída
                output << atual->palavra << " ";
                atual = raiz; // Volta para a raiz da árvore
                totalDecodificados++;

                if (totalDecodificados == totalPalavras) {
                    break; // Todos as palavras foram decodificadas
                }
            }
        }

        liberarArv(raiz); // Libera memória da árvore de Huffman

        input.close();
        output.close();
        cout << "Descompactacao concluida!" << endl;
    }
}
