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

typedef struct{
    int codVeiculo;
    float kmTotal;
}RankingVeiculo;

typedef struct {
    char marca[50];
    float kmTotal;
} RankingMarca;

typedef struct {
    int codVeiculo;
    int numInfracoes;
} InfracaoVeiculo;

typedef struct {
    int codVeiculo;
    float totalKm;
    float totalHoras;
} VelocidadeMedia;

typedef struct {
    char marca[50];
    float totalKm;
    float totalHoras;
} MarcaVelocidade;

typedef struct {
    int dono;
    float totalKm;
    float totalHoras;
} DonoVelocidade;

typedef struct {
    char marca[50];
    int total;
} MarcaContador;

typedef struct{
    time_t tempoBase;
    int milissegundos;
}TempoComMs;

typedef struct{
    char subnome[50];
    int contador;
}SubnomeContador;

size_t calcularMemoriaTotal(Dono* donos, Veiculo* veiculos, Sensor* sensores, Distancia* distancias, Passagem* passagens);
void listarDonoSubmenu();
void listarVeiculoSubmenu();
void subMenuAnaliseCirculacao();

void CarregarDonos(const char* arquivo);
void ListarDonos();
void registarDono();
void listarCondutoresOrdemAlfa(Dono* listaDonos);
void listarCondutoresPorContribuinte(Dono* listaDonos);
void exportarParaCSV(Dono* donos, Veiculo* veiculos, Sensor* sensores, Passagem* passagens);
void exportarParaXML(Dono* donos, Veiculo* veiculos);
void subnomeMaisComum(Dono* donos);

void CarregarVeiculos(const char* arquivo);
void ListarVeiculos();
int proximoCodVeiculo();
void registarVeiculos();
void listarVeiculosPorMatriculaPeriodo(Passagem* passagens, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void listarVeiculosOrdenados(Veiculo* listaVeiculos, int criterio);
Veiculo* obterVeiculo(Veiculo* lista, int cod);
void carroMaisRapido(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos);

void CarregarSensores(const char* arquivo);
void CarregarDistancias(const char* arquivo);
float obterDistanciaEntreSensores(Distancia* distancias, int s1, int s2);

void CarregarPassagens(const char* arquivo);
void registarPassagem();
RankingVeiculo* adicionarOuAtualizarRanking(RankingVeiculo* ranking, int* tamanho, int* capacidade, int codVeiculo, float km);
void rankingCirculacao(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
RankingMarca* adicionarOuAtualizarMarca(RankingMarca* lista, int* tamanho, int* capacidade, const char* marca, float km);
void rankingPorMarca(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void listarInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
InfracaoVeiculo* adicionarOuContarInfracao(InfracaoVeiculo* lista, int* tamanho, int* capacidade, int codVeiculo);
void rankingInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim);
void calcularVelocidadesMedias(Passagem* passagens, Distancia* distancias, Veiculo* veiculos);



void limparBuffer();
int compararNomes (const void* a, const void* b);
int compararContribuintes(const void* a, const void* b);
int compararPorMatricula(const void* a, const void* b);
int compararPorMarca(const void* a, const void* b);
int compararPorModelo(const void* a, const void* b);
TempoComMs stringParaTempoComMs(const char* data);
time_t stringParaTempo(const char* data);
#endif
