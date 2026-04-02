#include "funcionalidades.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int comando;
    char nomeArquivoCsv[100];
    char nomeArquivoBin[100];

    // Lê o identificador da funcionalidade
    if (scanf("%d", &comando) != 1)
        return 0;

    switch (comando) {
    case 1:
        // Lê: arquivoEntrada.csv arquivoSaida.bin
        scanf("%s %s", nomeArquivoCsv, nomeArquivoBin);
        funcionalidade1(nomeArquivoCsv, nomeArquivoBin);
        break;
    case 2:
        // Lê: arquivoEntrada.bin
        scanf("%s", nomeArquivoBin);
        funcionalidade2(nomeArquivoBin);
        break;
    case 3:
        // Lê: arquivoEntrada.bin
        scanf("%s", nomeArquivoBin);
        funcionalidade3(nomeArquivoBin);
        break;
    default:
        break;
    }

    return 0;
}