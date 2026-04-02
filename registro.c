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
    // 1. Campos fixos iniciais (removido + 7 inteiros = 29 bytes)
    fwrite(&reg->removido, sizeof(char), 1, fp);
    fwrite(&reg->proximo, sizeof(int), 1, fp);
    fwrite(&reg->codEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinha, sizeof(int), 1, fp);
    fwrite(&reg->codProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->distProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinhaIntegra, sizeof(int), 1, fp);
    fwrite(&reg->codEstIntegra, sizeof(int), 1, fp);

    // 2. Campos de tamanho (8 bytes) - DEVE VIR ANTES DAS STRINGS [cite: 100, 105]
    fwrite(&reg->tamNomeEstacao, sizeof(int), 1, fp);
    fwrite(&reg->tamNomeLinha, sizeof(int), 1, fp);

    // 3. Campos variáveis (Strings)
    if (reg->tamNomeEstacao > 0) {
        fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, fp);
    }
    if (reg->tamNomeLinha > 0) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, fp);
    }

    // 4. Preenchimento com lixo '$' até completar 80 bytes [cite: 99, 118, 119]
    int bytes_escritos = 37 + reg->tamNomeEstacao + reg->tamNomeLinha;
    while (bytes_escritos < 80) {
        fputc('$', fp);
        bytes_escritos++;
    }
}

int leRegistro(FILE *fp, registro *reg) {
    int bytesLidos = 0;
    
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

    // Lê os tamanhos primeiro [cite: 105]
    fread(&reg->tamNomeEstacao, sizeof(int), 1, fp);
    fread(&reg->tamNomeLinha, sizeof(int), 1, fp);
    bytesLidos += 8;

    if (reg->tamNomeEstacao > 0) {
        reg->nomeEstacao = malloc(reg->tamNomeEstacao + 1);
        fread(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, fp);
        reg->nomeEstacao[reg->tamNomeEstacao] = '\0';
        bytesLidos += reg->tamNomeEstacao;
    } else {
        reg->nomeEstacao = NULL;
    }

    if (reg->tamNomeLinha > 0) {
        reg->nomeLinha = malloc(reg->tamNomeLinha + 1);
        fread(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, fp);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
        bytesLidos += reg->tamNomeLinha;
    } else {
        reg->nomeLinha = NULL;
    }

    // Pula o lixo até o fim do registro de 80 bytes [cite: 99, 119]
    int lixo = 80 - bytesLidos;
    if (lixo > 0) {
        fseek(fp, lixo, SEEK_CUR);
    }
    
    return 1;
}