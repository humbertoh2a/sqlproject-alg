#include "registro.h"
#include <stdlib.h>

void escreveCabecalho(FILE *fp, cabecalho *cab) {
    // Escrita individual de cada campo
    fwrite(&cab->status, sizeof(char), 1, fp);
    fwrite(&cab->topo, sizeof(int), 1, fp);
    fwrite(&cab->proxRRN, sizeof(int), 1, fp);
    fwrite(&cab->nroEstacoes, sizeof(int), 1, fp);
    fwrite(&cab->nroParesEstacao, sizeof(int), 1, fp);
}

void escreveRegistro(FILE *fp, registro *reg) {
    // Escrita individual de cada campo
    fwrite(&reg->removido, sizeof(char), 1, fp);
    fwrite(&reg->proximo, sizeof(int), 1, fp);
    fwrite(&reg->codEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinha, sizeof(int), 1, fp);
    fwrite(&reg->codProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->distProxEstacao, sizeof(int), 1, fp);
    fwrite(&reg->codLinhaIntegra, sizeof(int), 1, fp);
    fwrite(&reg->codEstIntegra, sizeof(int), 1, fp);

    // Escreve campos variáveis

    fwrite(&reg->tamNomeEstacao, sizeof(int), 1, fp);
    if (reg->tamNomeEstacao > 0) {
        fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, fp);
    }

    fwrite(&reg->tamNomeLinha, sizeof(int), 1, fp);
    if (reg->tamNomeLinha > 0) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, fp);
    }

    int bytes_escritos = 37 + reg->tamNomeEstacao + reg->tamNomeLinha;

    // esse while é feito para evitar o lixo apos nomelinha.
    while (bytes_escritos < 80) {
        fputc('$', fp);
        bytes_escritos++;
    }
}