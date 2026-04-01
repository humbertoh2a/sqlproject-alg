#include "funcionalidades.h"
#include "registro.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    fgets(linha, sizeof(linha), arquivoCsv);

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
        if (campo != NULL) {
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
            if (existe == 0) {
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
        if (campo != NULL) {
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
            if (existePar == 0) {
                paresUnicosE1[cab.nroParesEstacao] = reg.codEstacao;
                paresUnicosE2[cab.nroParesEstacao] = reg.codProxEstacao;
                cab.nroParesEstacao++;
            }
        }

        escreveRegistro(arquivoBin, &reg);
        cab.proxRRN++;

        if (reg.nomeEstacao != NULL) free(reg.nomeEstacao);
        if (reg.nomeLinha != NULL) free(reg.nomeLinha);
    }

    cab.status = '1';
    fseek(arquivoBin, 0, SEEK_SET);
    escreveCabecalho(arquivoBin, &cab);

    fclose(arquivoCsv);
    fclose(arquivoBin);

    for (int i = 0; i < cab.nroEstacoes; i++) {
        free(nomesUnicos[i]);
    }

    binarioNaTela(nomeBin);
}