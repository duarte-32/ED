#ifndef PASSAGENS_H
#define PASSAGENS_H

#include <time.h>
#include "donos.h"
#include "carros.h"
#include "distancias.h"

typedef struct Passagem {
    int idSensor;
    int codVeiculo;
    char data[30];
    int tipoRegisto; // 0=entrada, 1=saída
    struct Passagem* prox;
} Passagem;

void CarregarPassagens(const char* arquivo);
void registarPassagem();
time_t stringParaTempo(const char* data);
void rankingCirculacao(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void rankingPorMarca(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void listarInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void rankingInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void calcularVelocidadesMedias(Passagem* passagens, Distancia* distancias, Veiculo* veiculos);
void marcaMaisRapida(Passagem* passagens, Distancia* distancias, Veiculo* veiculos);
void donoMaisRapido(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos);
void mediaPorCodigoPostal(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos, const char* codigoPostal);
void marcaMaisComum(Veiculo* veiculos);
void exportarParaCSV(Dono* donos, Veiculo* veiculos, Sensor* sensores, Passagem* passagens);


#endif
