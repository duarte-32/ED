#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas de dados
typedef struct {
    int numContribuinte;
    char nome[100];
    char codPostal[20];
} Dono;

typedef struct {
    char matricula[20];
    char marca[50];
    char modelo[50];
    int ano;
    int dono; // numContribuinte do dono
    int codVeiculo;
} Veiculo;

typedef struct {
    int codSensor;
    char designacao[100];
    char latitude[50];
    char longitude[50];
} Sensor;

typedef struct {
    int codSensor1;
    int codSensor2;
    float distancia;
} Distancia;

typedef struct {
    int idSensor;
    int codVeiculo;
    char data[30];
    int tipoRegisto; // 0=entrada, 1=saída
} Passagem;

// Listas vazias
Dono *donos = NULL;
Veiculo *veiculos = NULL;
Sensor *sensores = NULL;
Distancia *distancias = NULL;
Passagem *passagens = NULL;

int numDonos = 0;
int numVeiculos = 0;
int numSensores = 0;
int numDistancias = 0;
int numPassagens = 0;
