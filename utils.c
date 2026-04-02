#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

char* trataString(char **stringP) {
    /* Usa a vírgula como delimitador padrão do CSV  */
    char *token = strsep(stringP, ",");

    /* Se o campo estiver vazio (,,) ou for nulo, retorna NULL */
    if (token == NULL || strlen(token) == 0) {
        return NULL;
    }
    return token;
}

void limpaBuffer() {
    int c;
    /* Limpa o rastro do \n deixado pelo scanf no main */
    while ((c = getchar()) != '\n' && c != EOF);
}

void removeQuebraLinha(char *texto) {
    int tam = strlen(texto);
    /* Garante que o \n do final da linha do CSV não entre no dado */
    if (tam > 0 && (texto[tam - 1] == '\n' || texto[tam - 1] == '\r')) {
        texto[tam - 1] = '\0';
    }
}

void scan_quote_string(char *str) {
    char R;
    while((R = getchar()) != EOF && isspace(R));
    if(R == 'N' || R == 'n') { // Trata o NULO
        getchar(); getchar(); getchar();
        strcpy(str, "");
    } else if(R == '\"') {
        if(scanf("%[^\"]", str) != 1) {
            strcpy(str, "");
        }
        getchar();
    } else if(R != EOF){
        str[0] = R;
        scanf("%s", &str[1]);
    } else {
        strcpy(str, "");
    }
}