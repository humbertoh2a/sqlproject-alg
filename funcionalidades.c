#include "funcionalidades.h"
#include "fornecidas.h"
#include "registro.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Verifica se um registro atende a todos os pares campo/valor fornecidos.
// Implementa lógica AND entre os filtros e retorna 1 (sim) ou 0 (nao).
int registroAtendeFiltros(registro *reg, char campos[][50], char valores[][100], int m) {
    for (int j = 0; j < m; j++) {
        if (strcmp(campos[j], "codEstacao") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->codEstacao != val)
                return 0;
        } else if (strcmp(campos[j], "codLinha") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->codLinha != val)
                return 0;
        } else if (strcmp(campos[j], "codProxEstacao") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->codProxEstacao != val)
                return 0;
        } else if (strcmp(campos[j], "distProxEstacao") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->distProxEstacao != val)
                return 0;
        } else if (strcmp(campos[j], "codLinhaIntegra") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->codLinhaIntegra != val)
                return 0;
        } else if (strcmp(campos[j], "codEstIntegra") == 0) {
            int val = (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
            if (reg->codEstIntegra != val)
                return 0;
        } else if (strcmp(campos[j], "nomeEstacao") == 0) {
            if (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) {
                if (reg->tamNomeEstacao != 0)
                    return 0;
            } else {
                if (reg->nomeEstacao == NULL || strcmp(reg->nomeEstacao, valores[j]) != 0)
                    return 0;
            }
        } else if (strcmp(campos[j], "nomeLinha") == 0) {
            if (valores[j][0] == '\0' || strcmp(valores[j], "NULO") == 0) {
                if (reg->tamNomeLinha != 0)
                    return 0;
            } else {
                if (reg->nomeLinha == NULL || strcmp(reg->nomeLinha, valores[j]) != 0)
                    return 0;
            }
        }
    }
    return 1;
}

// FUNCIONALIDADE 1
// Converte CSV em arquivo binário seguindo o formato especificado.
void funcionalidade1(char *nomeCsv, char *nomeBin) {
    FILE *arquivoCsv = fopen(nomeCsv, "r");
    if (arquivoCsv == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *arquivoBin = fopen(nomeBin, "wb");
    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoCsv);
        return;
    }

    cabecalho cab;
    cab.status = '0';
    cab.topo = -1;
    cab.proxRRN = 0;
    cab.nroEstacoes = 0;
    cab.nroParesEstacao = 0;

    escreveCabecalho(arquivoBin, &cab);

    char linha[200];
    fgets(linha, sizeof(linha), arquivoCsv); // pula cabecalho CSV

    char *nomesUnicos[1000];
    int paresUnicosE1[1000];
    int paresUnicosE2[1000];

    while (fgets(linha, sizeof(linha), arquivoCsv) != NULL) {
        removeQuebraLinha(linha);
        char *ponteiroLinha = linha;

        registro reg;
        reg.removido = '0';
        reg.proximo = -1;

        char *campo = trataString(&ponteiroLinha);
        reg.codEstacao = (campo != NULL) ? atoi(campo) : -1;

        campo = trataString(&ponteiroLinha);
        if (campo != NULL && strlen(campo) > 0) {
            reg.tamNomeEstacao = strlen(campo);
            reg.nomeEstacao = malloc(reg.tamNomeEstacao + 1);
            strcpy(reg.nomeEstacao, campo);

            int existe = 0;
            for (int i = 0; i < cab.nroEstacoes; i++) {
                if (strcmp(nomesUnicos[i], reg.nomeEstacao) == 0) {
                    existe = 1;
                    break;
                }
            }
            if (!existe) {
                nomesUnicos[cab.nroEstacoes] = malloc(reg.tamNomeEstacao + 1);
                strcpy(nomesUnicos[cab.nroEstacoes], reg.nomeEstacao);
                cab.nroEstacoes++;
            }
        } else {
            reg.tamNomeEstacao = 0;
            reg.nomeEstacao = NULL;
        }

        campo = trataString(&ponteiroLinha);
        reg.codLinha = (campo != NULL) ? atoi(campo) : -1;

        campo = trataString(&ponteiroLinha);
        if (campo != NULL && strlen(campo) > 0) {
            reg.tamNomeLinha = strlen(campo);
            reg.nomeLinha = malloc(reg.tamNomeLinha + 1);
            strcpy(reg.nomeLinha, campo);
        } else {
            reg.tamNomeLinha = 0;
            reg.nomeLinha = NULL;
        }

        campo = trataString(&ponteiroLinha);
        reg.codProxEstacao = (campo != NULL) ? atoi(campo) : -1;

        campo = trataString(&ponteiroLinha);
        reg.distProxEstacao = (campo != NULL) ? atoi(campo) : -1;

        campo = trataString(&ponteiroLinha);
        reg.codLinhaIntegra = (campo != NULL) ? atoi(campo) : -1;

        campo = trataString(&ponteiroLinha);
        reg.codEstIntegra = (campo != NULL) ? atoi(campo) : -1;

        if (reg.codEstacao != -1 && reg.codProxEstacao != -1) {
            int existePar = 0;
            for (int i = 0; i < cab.nroParesEstacao; i++) {
                if (paresUnicosE1[i] == reg.codEstacao && paresUnicosE2[i] == reg.codProxEstacao) {
                    existePar = 1;
                    break;
                }
            }
            if (!existePar) {
                paresUnicosE1[cab.nroParesEstacao] = reg.codEstacao;
                paresUnicosE2[cab.nroParesEstacao] = reg.codProxEstacao;
                cab.nroParesEstacao++;
            }
        }

        escreveRegistro(arquivoBin, &reg);
        cab.proxRRN++;

        if (reg.nomeEstacao != NULL)
            free(reg.nomeEstacao);
        if (reg.nomeLinha != NULL)
            free(reg.nomeLinha);
    }

    cab.status = '1';
    fseek(arquivoBin, 0, SEEK_SET);
    escreveCabecalho(arquivoBin, &cab);

    fclose(arquivoCsv);
    fclose(arquivoBin);

    for (int i = 0; i < cab.nroEstacoes; i++) {
        free(nomesUnicos[i]);
    }

    BinarioNaTela(nomeBin);
}

// FUNCIONALIDADE 2
// Lista todos os registros ativos (nao removidos).
void funcionalidade2(char *nomeBin) {
    FILE *fp = fopen(nomeBin, "rb");
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char status;
    fread(&status, sizeof(char), 1, fp);
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    fseek(fp, 17, SEEK_SET);

    registro reg;
    int encontrou = 0;

    while (leRegistro(fp, &reg)) {
        if (reg.removido == '0') {
            imprimeRegistro(&reg);
            encontrou = 1;
        }
        if (reg.nomeEstacao != NULL)
            free(reg.nomeEstacao);
        if (reg.nomeLinha != NULL)
            free(reg.nomeLinha);
    }

    if (!encontrou) {
        printf("Registro inexistente.\n");
    }

    fclose(fp);
}

// FUNCIONALIDADE 3
// Executa buscas por pares campo/valor (AND entre pares) e imprime resultados.
void funcionalidade3(char *nomeBin) {
    FILE *fp = fopen(nomeBin, "rb");
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char status;
    fread(&status, sizeof(char), 1, fp);
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    int n;
    if (scanf("%d", &n) != 1) {
        fclose(fp);
        return;
    }

    for (int i = 0; i < n; i++) {
        int m;
        scanf("%d", &m);

        char campos[m][50];
        char valores[m][100];

        for (int j = 0; j < m; j++) {
            scanf("%s", campos[j]);
            if (strcmp(campos[j], "nomeEstacao") == 0 || strcmp(campos[j], "nomeLinha") == 0) {
                ScanQuoteString(valores[j]);
            } else {
                scanf("%s", valores[j]);
            }
        }

        fseek(fp, 17, SEEK_SET);

        registro reg;
        int encontrou_algum = 0;

        while (leRegistro(fp, &reg)) {
            if (reg.removido == '0') {
                if (registroAtendeFiltros(&reg, campos, valores, m)) {
                    imprimeRegistro(&reg);
                    encontrou_algum = 1;
                }
            }
            if (reg.nomeEstacao != NULL)
                free(reg.nomeEstacao);
            if (reg.nomeLinha != NULL)
                free(reg.nomeLinha);
        }

        if (!encontrou_algum) {
            printf("Registro inexistente.\n");
        }
    }

    fclose(fp);
}

// FUNCIONALIDADE 4
// Recuperacao direta de um registro atraves de seu RRN (Acesso com fseek)
void funcionalidade4(char *nomeBin, int rrn) {
    FILE *fp = fopen(nomeBin, "rb"); // Aberto apenas para leitura
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char status;
    if (fread(&status, sizeof(char), 1, fp) != 1 || status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    // Calcula o byte offset pulando o cabecalho
    long offset = 17L + (long)rrn * 80L;
    fseek(fp, offset, SEEK_SET);

    registro reg;

    if (leRegistro(fp, &reg)) {
        if (reg.removido == '0') {
            imprimeRegistro(&reg);
        } else {
            printf("Registro inexistente.\n");
        }
        
        if (reg.nomeEstacao != NULL)
            free(reg.nomeEstacao);
        if (reg.nomeLinha != NULL)
            free(reg.nomeLinha);
    } else {
        printf("Registro inexistente.\n");
    }

    fclose(fp);
}