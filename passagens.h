#ifndef PASSAGENS_H
#define PASSAGENS_H

typedef struct Passagem {
    int idSensor;
    int codVeiculo;
    char data[30];
    int tipoRegisto; // 0=entrada, 1=saída
    struct Passagem* prox;
} Passagem;

void CarregarPassagens(const char* arquivo);
void registarPassagem();

#endif