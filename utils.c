#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *trataString(char **stringP) {
    // Usa vírgula como delimitador e devolve o token.
    // Retorna NULL para campos vazios
    char *token = strsep(stringP, ",");
    if (token == NULL || strlen(token) == 0) {
        return NULL;
    }
    return token;
}

void removeQuebraLinha(char *texto) {
    // Remove caracteres de quebra de linha \r' e '\n'
    texto[strcspn(texto, "\r\n")] = '\0';
}