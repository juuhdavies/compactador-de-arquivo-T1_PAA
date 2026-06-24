#include "NoHuffman.h"
#include "Utils.h"
#include "PalavraHuffman.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

namespace WordHuffman {

    /**
     * @brief Constrói a tabela de frequências das palavras a partir do arquivo de texto.
     * @param inputFile Caminho do arquivo de entrada.
     * @param freq Mapa de dispersão onde será armazenada a contagem (Palavra -> Frequência).
     */
    void montarTabFreq(const string& inputFile, unordered_map<string, int>& freq) {
        // Divide o arquivo de texto em tokens
        vector<string> tokens = Utils::tokenizar(inputFile);

        // Contabiliza a ocorrência de cada token individualmente no mapa
        for (const string& token : tokens) {
            freq[token]++;
        }
    }

    /**
     * @brief Desaloca a árvore de Huffman recursivamente.
     */
    void liberarArv(NoHuffmanPalavra* raiz) {
        if (raiz) {
            liberarArv(raiz->esq);
            liberarArv(raiz->dir);
            delete raiz;
        }
    }

    /**
     * @brief Percorre a árvore binária recursivamente para mapear os caminhos de bits.
     * @param raiz Ponteiro para o nó atual.
     * @param codAtual String binária acumulada no caminho até o nó atual.
     * @param codigos Mapa de saída que associará cada palavra ao seu código binário final.
     */
    void gerarCodHuffman(NoHuffmanPalavra* raiz, string codAtual, unordered_map<string, string>& codigos) {
        if (!raiz) return;

        // Se for um nó folha, armazena o código Huffman para a palavra
        if (!raiz->esq && !raiz->dir) {
            codigos[raiz->palavra] = codAtual;
        }

        // Caminho para a esquerda recebe bit '0', caminho para a direita recebe bit '1'
        gerarCodHuffman(raiz->esq, codAtual + "0", codigos);
        gerarCodHuffman(raiz->dir, codAtual + "1", codigos);
    }

    /**
     * @brief Monta a Árvore Binária de Huffman baseando-se no algoritmo guloso clássico.
     * @param freq Tabela hash contendo a distribuição de frequências das palavras.
     * @return NoHuffmanPalavra* Retorna o ponteiro para o nó raiz da árvore gerada.
     */
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

    /**
     * @brief compressão baseada em palavras.
     * Grava o cabeçalho estruturado e empacota os dados em nível de bits.
     */
    void compress(const string& inputFile, const string& outputFile) {
        // Contagem de tempo de execução da compressão/descompressão
        auto start = chrono::high_resolution_clock::now();
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

        //GRAVAÇÃO DO CABEÇALHO
        size_t qtdPalavras = freq.size();
        output.write(reinterpret_cast<const char*>(&qtdPalavras), sizeof(size_t));

        for (const auto& par : freq) {
            output.write(par.first.c_str(), par.first.size() + 1); // Escreve a palavra e \0
            output.write(reinterpret_cast<const char*>(&par.second), sizeof(int)); // Escreve a frequência
        }

        //GRAVAÇÃO DOS DADOS CODIFICADOS
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

        //Mostra o tempo de execução da compressão/descompressão
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
    }

    /**
     * @brief descompressão por palavras.
     * Reconstrói a árvore de Huffman usando o cabeçalho e regenera o texto original.
     */
    void decompress(const string& inputFile, const string& outputFile) {
        // Contagem de tempo de execução da compressão/descompressão
        auto start = chrono::high_resolution_clock::now();
        
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

        //Mostra o tempo de execução da compressão/descompressão
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
    }
}
