#include "funcionalidades.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int comando;
    char nomeArquivoCsv[100];
    char nomeArquivoBin[100];

    if (scanf("%d", &comando) != 1)
        return 0;

    switch (comando) {
    case 1:
        scanf("%s %s", nomeArquivoCsv, nomeArquivoBin);
        funcionalidade1(nomeArquivoCsv, nomeArquivoBin);
        break;
    case 2:
        scanf("%s", nomeArquivoBin);
        funcionalidade2(nomeArquivoBin);
        break;
    case 3:
        scanf("%s", nomeArquivoBin);
        funcionalidade3(nomeArquivoBin);
        break;
    case 4: {
        int n;
        scanf("%s %d", nomeArquivoBin, &n);
        funcionalidade4(nomeArquivoBin, n);
        break;
    }
    default:
        break;
    }

    return 0;
}