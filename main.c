#include "funcionalidades.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int comando;
    char nomeArquivoCsv[100];
    char nomeArquivoBin[100];

    // Lê o identificador da funcionalidade [cite: 133, 651]
    if (scanf("%d", &comando) != 1)
        return 0;

    switch (comando) {
    case 1:
        // Lê: arquivoEntrada.csv arquivoSaida.bin [cite: 148, 666]
        scanf("%s %s", nomeArquivoCsv, nomeArquivoBin);
        funcionalidade1(nomeArquivoCsv, nomeArquivoBin);
        break;
    // As próximas funcionalidades entrarão aqui
    default:
        break;
    }

    return 0;
}