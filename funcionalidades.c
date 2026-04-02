#include "funcionalidades.h"
#include "registro.h"
#include "utils.h"
#include "fornecidas.h"
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

    BinarioNaTela(nomeBin);
}

void imprimeRegistro(registro *reg) {
    // A ordem de exibição é exata: codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstIntegra [cite: 191]
    
    if (reg->codEstacao == -1) printf("NULO ");
    else printf("%d ", reg->codEstacao);

    if (reg->tamNomeEstacao == 0 || reg->nomeEstacao == NULL) printf("NULO ");
    else printf("%s ", reg->nomeEstacao);

    if (reg->codLinha == -1) printf("NULO ");
    else printf("%d ", reg->codLinha);

    if (reg->tamNomeLinha == 0 || reg->nomeLinha == NULL) printf("NULO ");
    else printf("%s ", reg->nomeLinha);

    if (reg->codProxEstacao == -1) printf("NULO ");
    else printf("%d ", reg->codProxEstacao);

    if (reg->distProxEstacao == -1) printf("NULO ");
    else printf("%d ", reg->distProxEstacao);

    if (reg->codLinhaIntegra == -1) printf("NULO ");
    else printf("%d ", reg->codLinhaIntegra);

    if (reg->codEstIntegra == -1) printf("NULO\n");
    else printf("%d\n", reg->codEstIntegra);
}

void funcionalidade2(char *nomeBin) {
    FILE *fp = fopen(nomeBin, "rb");
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.\n"); // [cite: 194]
        return;
    }

    char status;
    fread(&status, sizeof(char), 1, fp);
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n"); // Verifica consistência [cite: 49]
        fclose(fp);
        return;
    }
    
    fseek(fp, 17, SEEK_SET); // Pula o cabeçalho (17 bytes) [cite: 62]

    registro reg;
    int encontrou = 0;
    
    while (leRegistro(fp, &reg)) {
        if (reg.removido == '0') { // Ignora removidos logicamente [cite: 181]
            imprimeRegistro(&reg);
            encontrou = 1;
        }
        if (reg.nomeEstacao != NULL) free(reg.nomeEstacao);
        if (reg.nomeLinha != NULL) free(reg.nomeLinha);
    }

    if (!encontrou) {
        printf("Registro inexistente.\n"); // [cite: 193]
    }

    fclose(fp);
}

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

    int n; // quantidade de vezes que a busca deve acontecer [cite: 231]
    if(scanf("%d", &n) != 1) {
        fclose(fp);
        return;
    }

    for (int i = 0; i < n; i++) {
        int m; // quantidade de pares nome/valor [cite: 232]
        scanf("%d", &m);
        
        char campos[m][50];
        char valores[m][100];
        
        for (int j = 0; j < m; j++) {
            scanf("%s", campos[j]);
            scan_quote_string(valores[j]); // Função para ler strings com aspas ou NULO [cite: 213]
        }

        fseek(fp, 17, SEEK_SET); // Volta pro começo dos registros pra cada busca
        registro reg;
        int encontrou_algum = 0;

        while (leRegistro(fp, &reg)) {
            if (reg.removido == '0') {
                int match = 1;
                for (int j = 0; j < m; j++) {
                    if (strcmp(campos[j], "codEstacao") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.codEstacao != val) match = 0;
                    } else if (strcmp(campos[j], "codLinha") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.codLinha != val) match = 0;
                    } else if (strcmp(campos[j], "codProxEstacao") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.codProxEstacao != val) match = 0;
                    } else if (strcmp(campos[j], "distProxEstacao") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.distProxEstacao != val) match = 0;
                    } else if (strcmp(campos[j], "codLinhaIntegra") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.codLinhaIntegra != val) match = 0;
                    } else if (strcmp(campos[j], "codEstIntegra") == 0) {
                        int val = (strcmp(valores[j], "NULO") == 0) ? -1 : atoi(valores[j]);
                        if (reg.codEstIntegra != val) match = 0;
                    } else if (strcmp(campos[j], "nomeEstacao") == 0) {
                        if (strcmp(valores[j], "NULO") == 0) {
                            if (reg.tamNomeEstacao != 0) match = 0;
                        } else {
                            if (reg.nomeEstacao == NULL || strcmp(reg.nomeEstacao, valores[j]) != 0) match = 0;
                        }
                    } else if (strcmp(campos[j], "nomeLinha") == 0) {
                        if (strcmp(valores[j], "NULO") == 0) {
                            if (reg.tamNomeLinha != 0) match = 0;
                        } else {
                            if (reg.nomeLinha == NULL || strcmp(reg.nomeLinha, valores[j]) != 0) match = 0;
                        }
                    }
                }
                
                if (match) {
                    imprimeRegistro(&reg);
                    encontrou_algum = 1;
                }
            }
            if (reg.nomeEstacao != NULL) free(reg.nomeEstacao);
            if (reg.nomeLinha != NULL) free(reg.nomeLinha);
        }
        
        if (!encontrou_algum) {
            printf("Registro inexistente.\n"); // [cite: 242]
        }
    }
    fclose(fp);
}