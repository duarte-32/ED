#ifndef CARROS_H
#define CARROS_H

typedef struct Veiculo {
    char matricula[10];
    char marca[50];
    char modelo[50];
    int ano;
    int dono; // numContribuinte do dono
    int codVeiculo;
    struct Veiculo* prox;
} Veiculo;

void CarregarVeiculos(const char* arquivo);
void ListarVeiculos();
void pesquisarMatricula();
int proximoCodVeiculo();
void registarVeiculos();
int compararPorMatricula(const void* a, const void* b);
int compararPorMarca(const void* a, const void* b);
int compararPorModelo(const void* a, const void* b);
void listarVeiculosOrdenados(Veiculo* listaVeiculos, int criterio);

#endif