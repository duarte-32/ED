#ifndef DONOS_H
#define DONOS_H

typedef struct Dono {
    int numContribuinte;
    char nome[100];
    char codPostal[20];
    struct Dono* prox;
} Dono;


void listarDonoSubmenu();
void listarVeiculoSubmenu();
void CarregarDonos(const char* arquivo);
void ListarDonos();
void registarDono();
int compararNomes (const void* a, const void* b);
void listarCondutoresOrdemAlfa(Dono* listaDonos);
int compararContribuintes(const void* a, const void* b);
void listarCondutoresPorContribuinte(Dono* listaDonos);
void exportarParaXML(Dono* donos, Veiculo* veiculos);
void exportarParaCSV(Dono* donos, Veiculo* veiculos, Sensor* sensores, Passagem* passagens);

#endif
