#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas de dados
typedef struct Dono {
    int numContribuinte;
    char nome[100];
    char codPostal[20];
    struct Dono* prox;
} Dono;

typedef struct Veiculo {
    char matricula[10];
    char marca[50];
    char modelo[50];
    int ano;
    int dono; // numContribuinte do dono
    int codVeiculo;
    struct Veiculo* prox;
} Veiculo;

typedef struct Sensor {
    int codSensor;
    char designacao[100];
    char latitude[50];
    char longitude[50];
    struct Sensor* prox;
} Sensor;

typedef struct Distancia{
    int codSensor1;
    int codSensor2;
    float distancia;
    struct Distancia* prox;
} Distancia;

typedef struct Passagem {
    int idSensor;
    int codVeiculo;
    char data[30];
    int tipoRegisto; // 0=entrada, 1=saída
    struct Passagem* prox;
} Passagem;


size_t calcularMemoriaTotal(Dono* donos, Veiculo* veiculos, Sensor* sensores, Distancia* distancias, Passagem* passagens);
void listarDonoSubmenu();
void listarVeiculoSubmenu();

void CarregarDonos(const char* arquivo);
void ListarDonos();
void registarDono();
int compararNomes (const void* a, const void* b);
void listarCondutoresOrdemAlfa(Dono* listaDonos);
int compararContribuintes(const void* a, const void* b);
void listarCondutoresPorContribuinte(Dono* listaDonos);
void CarregarVeiculos(const char* arquivo);
void ListarVeiculos();
int proximoCodVeiculo();
void registarVeiculos();
int compararPorMatricula(const void* a, const void* b);
int compararPorMarca(const void* a, const void* b);
int compararPorModelo(const void* a, const void* b);
void listarVeiculosOrdenados(Veiculo* listaVeiculos, int criterio);
void CarregarSensores(const char* arquivo);
void CarregarDistancias(const char* arquivo);
void CarregarPassagens(const char* arquivo);
void registarPassagem();

#endif
