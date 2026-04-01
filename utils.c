#include "utils.h"
#include <string.h>
#include <stdlib.h>

char* trataString(char **stringP) {
    /* Usa a vírgula como delimitador padrão do CSV  */
    char *token = strsep(stringP, ",");

    /* Se o campo estiver vazio (,,) ou for nulo, retorna NULL [cite: 116, 634] */
    if (token == NULL || strlen(token) == 0) {
        return NULL;
    }
    return token;
}

void limpaBuffer() {
    int c;
    /* Limpa o rastro do \n deixado pelo scanf no main [cite: 406] */
    while ((c = getchar()) != '\n' && c != EOF);
}

void removeQuebraLinha(char *texto) {
    int tam = strlen(texto);
    /* Garante que o \n do final da linha do CSV não entre no dado [cite: 615] */
    if (tam > 0 && (texto[tam - 1] == '\n' || texto[tam - 1] == '\r')) {
        texto[tam - 1] = '\0';
    }
}