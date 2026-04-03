#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>

// Arquivo destinado a declarar as structs a serem usados no trabalho

typedef struct {
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacao;
} cabecalho;

typedef struct {
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char *nomeEstacao;
    int tamNomeLinha;
    char *nomeLinha;
} registro;

// Declaração de funcoes a serem utilizadas:

void escreveCabecalho(FILE *fp, cabecalho *cab);
void escreveRegistro(FILE *fp, registro *reg);
int leRegistro(FILE *fp, registro *reg);
void imprimeRegistro(registro *reg);

#endif