#include "registro.h"
#include <stdlib.h>
#include <string.h>

void escreveCabecalho(FILE *fp, cabecalho *cab) {
    fwrite(&cab->status, sizeof(char), 1, fp);
    fwrite(&cab->topo, sizeof(int), 1, fp);
    fwrite(&cab->proxRRN, sizeof(int), 1, fp);
    fwrite(&cab->nroEstacoes, sizeof(int), 1, fp);
    fwrite(&cab->nroParesEstacao, sizeof(int), 1, fp);
}

void escreveRegistro(FILE *fp, registro *reg) {
    // Escreve registro no formato fixo de 80 bytes:

    fwrite(&reg->removido, sizeof(char), 1, fp);
    fwrite(&reg->proximo, sizeof(int), 1, fp);
    fwrite(&reg->codEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinha, sizeof(int), 1, fp);
    fwrite(&reg->codProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->distProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinhaIntegra, sizeof(int), 1, fp);
    fwrite(&reg->codEstIntegra, sizeof(int), 1, fp);

    fwrite(&reg->tamNomeEstacao, sizeof(int), 1, fp);
    if (reg->tamNomeEstacao > 0) {
        fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, fp);
    }
    fwrite(&reg->tamNomeLinha, sizeof(int), 1, fp);
    if (reg->tamNomeLinha > 0) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, fp);
    }

    // Preenchimento com $ ate completar
    int bytesEscritos = 37 + reg->tamNomeEstacao + reg->tamNomeLinha;
    while (bytesEscritos < 80) {
        fputc('$', fp);
        bytesEscritos++;
    }
}

int leRegistro(FILE *fp, registro *reg) {
    // Faz a leitura na ordem e completa ate 80bytes
    int bytesLidos = 0;

    // Confere se foi removido
    if (fread(&reg->removido, sizeof(char), 1, fp) != 1) {
        return 0;
    }
    bytesLidos += 1;

    fread(&reg->proximo, sizeof(int), 1, fp);
    fread(&reg->codEstacao, sizeof(int), 1, fp);
    fread(&reg->codLinha, sizeof(int), 1, fp);
    fread(&reg->codProxEstacao, sizeof(int), 1, fp);
    fread(&reg->distProxEstacao, sizeof(int), 1, fp);
    fread(&reg->codLinhaIntegra, sizeof(int), 1, fp);
    fread(&reg->codEstIntegra, sizeof(int), 1, fp);
    bytesLidos += 28;

    fread(&reg->tamNomeEstacao, sizeof(int), 1, fp);
    bytesLidos += 4;

    if (reg->tamNomeEstacao > 0) {
        reg->nomeEstacao = malloc(reg->tamNomeEstacao + 1);
        fread(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, fp);
        reg->nomeEstacao[reg->tamNomeEstacao] = '\0';
        bytesLidos += reg->tamNomeEstacao;
    } else {
        reg->nomeEstacao = NULL;
    }

    fread(&reg->tamNomeLinha, sizeof(int), 1, fp);
    bytesLidos += 4;
    if (reg->tamNomeLinha > 0) {
        reg->nomeLinha = malloc(reg->tamNomeLinha + 1);
        fread(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, fp);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
        bytesLidos += reg->tamNomeLinha;
    } else {
        reg->nomeLinha = NULL;
    }

    // Continua até completar 80 bytes
    int lixo = 80 - bytesLidos;
    if (lixo > 0) {
        fseek(fp, lixo, SEEK_CUR);
    }

    return 1;
}

void imprimeRegistro(registro *reg) {
    // Imprime campos na ordem
    // Campos com valor -1 ou tamanho 0 são exibidos como NULO
    if (reg->codEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg->codEstacao);

    if (reg->tamNomeEstacao == 0 || reg->nomeEstacao == NULL)
        printf("NULO ");
    else
        printf("%s ", reg->nomeEstacao);

    if (reg->codLinha == -1)
        printf("NULO ");
    else
        printf("%d ", reg->codLinha);

    if (reg->tamNomeLinha == 0 || reg->nomeLinha == NULL)
        printf("NULO ");
    else
        printf("%s ", reg->nomeLinha);

    if (reg->codProxEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg->codProxEstacao);

    if (reg->distProxEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg->distProxEstacao);

    if (reg->codLinhaIntegra == -1)
        printf("NULO ");
    else
        printf("%d ", reg->codLinhaIntegra);

    if (reg->codEstIntegra == -1)
        printf("NULO\n");
    else
        printf("%d\n", reg->codEstIntegra);
}
