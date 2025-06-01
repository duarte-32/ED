#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "donos.h"
#include "carros.h"
#include "sensores.h"
#include "distancias.h"
#include "passagens.h"


// Listas vazias globais
extern Dono *donos;
extern Veiculo *veiculos;
extern Sensor *sensores;
extern Distancia *distancias;
extern Passagem *passagens;

size_t calcularMemoriaTotal(Dono* donos, Veiculo* veiculos, Sensor* sensores, Distancia* distancias, Passagem* passagens){
    size_t total = 0;

    Dono* d = donos;
    while(d != NULL){
        total += sizeof(Dono);
        d = d->prox;
    }

    Veiculo* v = veiculos;
    while(v != NULL){
        total += sizeof(Veiculo);
        v = v->prox;
    }

    Sensor* s = sensores;
    while(d != NULL){
         total += sizeof(Sensor);
        s = s->prox;
    }

    // Distancias
    Distancia* dist = distancias;
    while (dist != NULL) {
        total += sizeof(Distancia);
        dist = dist->prox;
    }

    // Passagens
    Passagem* p = passagens;
    while (p != NULL) {
        total += sizeof(Passagem);
        p = p->prox;
    }

    return total;
}

void listarDonoSubmenu(){
    int opcao;

    do{
        printf("\n--- Submenu Listar Dono ---\n");
        printf("| 1. Listar Donos                       |\n");
        printf("| 2. Listar por ordem Alfabética        |\n");
        printf("| 3. Listar por número de contribuinte  |\n");
        printf("| 0. Voltar ao menu principal           |\n");
        printf("Escolha uma opção:\n");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                ListarDonos();
                break;

            case 2:
                listarCondutoresOrdemAlfa(donos);
                break;
            case 3:
                listarCondutoresPorContribuinte(donos);
                break;
            case 0:
                printf("Voltar ao menu principal.\n");
                break;
            default:
                printf("Opção inválida!\n");

        }
    }
    while(opcao != 0);

    return;
}

void listarVeiculoSubmenu(){
    int opcao;
    do {
        printf("\n--- Submenu Listar Veículos ---\n");
        printf("|   1.Ordenar por Matrícula     |\n");
        printf("|   2.Ordenar por Marca         |\n");
        printf("|   3.Ordenar por Modelo        |\n");
        printf("|   0.Voltar                    |\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= 3) {
            listarVeiculosOrdenados(veiculos, opcao);
        } else if (opcao != 0) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}

void subMenuAnaliseCirculacao(){
    char dataInicio[30], dataFim[30];
    int opcao;

    getchar(); //Limpar \n pendente do scnaf anterior

    printf("\n--- Submenu Análise de Circulação --\n");
    printf("Introduza o período a analisar: \n");

    printf("Data início (dd-mm-yyyy hh:mm:ss: ");
    fgets(dataInicio, sizeof(dataInicio), stdin);
    dataInicio[strcspn(dataInicio, "\n")] = '\0';

    printf("Data fim (dd-mm-yyyy hh:mm:ss: ");
    fgets(dataFim, sizeof(dataFim), stdin);
    dataFim[strcspn(dataFim, "\n")] = '\0';

     do {
        printf("\n--- Análises Disponíveis ---\n");
        printf("| 1.Ranking de circulação por veículo (total km)    |\n");
        printf("| 2.Ranking por marca (total km)                    |\n");
        printf("| 3.Veículos com velocidade média > 120 km/h        |\n");
        printf("| 4.Ranking de infrações por veículo                |\n");
        printf("| 0.Voltar                                          |\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                rankingCirculacao(passagens, distancias, veiculos, dataInicio, dataFim);
                break;
            case 2:
                rankingPorMarca(passagens, distancias, veiculos, dataInicio, dataFim);
                break;
            case 3:
                listarInfracoes(passagens, distancias, veiculos, dataInicio, dataFim);
                break;
            case 4:
                rankingInfracoes(passagens, distancias, veiculos, dataInicio, dataFim);
                break;
            case 0:
                printf("A voltar ao menu principal...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void submenuAnalisesAvancadas() {
    int opcao;
    char codPostal[20];

    do {
        printf("\n===== Submenu Análises Avançadas =====\n");
        printf("12 - Listar velocidades médias por veículo\n");
        printf("13 - Qual a marca mais rápida\n");
        printf("14 - Qual o dono mais rápido\n");
        printf("15 - Velocidade média por código postal\n");
        printf("16 - Marca mais comum\n");
        printf("17 - Exportar base de dados para CSV\n");
        printf("18 - Exportar base de dados para XML\n");
        printf("0  - Voltar ao menu principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpar \n

        switch (opcao) {
            case 12:
                calcularVelocidadesMedias(passagens, distancias, veiculos);
                break;
            case 13:
                marcaMaisRapida(passagens, distancias, veiculos);
                break;
            case 14:
                donoMaisRapido(passagens, distancias, veiculos, donos);
                break;
            case 15:
                printf("Introduza o código postal: ");
                fgets(codPostal, sizeof(codPostal), stdin);
                codPostal[strcspn(codPostal, "\n")] = '\0';
                mediaPorCodigoPostal(passagens, distancias, veiculos, donos, codPostal);
                break;
            case 16:
                marcaMaisComum(veiculos);
                break;
            case 17:
                exportarParaCSV(donos, veiculos, sensores, passagens);
                break;
            case 18:
                exportarParaXML(donos, veiculos);
                break;
            case 0:
                printf("A voltar ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}


int main(){
	int op;
	setlocale(LC_ALL, "Portuguese");

	CarregarDonos("donos.txt");
	CarregarVeiculos("carros.txt");
	CarregarSensores("sensores.txt");
	CarregarDistancias("distancias.txt");
	CarregarPassagens("passagem.txt");
	do{
		printf("******VIA VERMELHA******");
		printf("\n-------------------------\n");
		printf("| 1.Registar Dono               |\n");
		printf("| 2.Listar Donos                |\n");
		printf("| 3.Registar Veiculo            |\n");
		printf("| 4.Listar Veiculos             |\n");
		printf("| 5.Listar veículos ordenados   |\n");
		printf("| 6.Registar Passagem           |\n");
		printf("| 7.Análise de Circulação       |\n");
		printf("| 8. Análises Avançadas         |\n");
		printf("| 9.Memoria Total Ocupada       |\n");
		printf("| 0.Sair do Programa            |\n");
		printf("----------------------------\n");
		scanf("%d",&op);
		switch(op){
			case 1:
				registarDono();
				break;
			case 2:
				listarDonoSubmenu();
				break;
			case 3:
				registarVeiculos();
				break;
			case 4:
                ListarVeiculos();
                break;
            case 5:
				listarVeiculoSubmenu();
				break;
			case 6:
				registarPassagem();
				break;
            case 7:
                subMenuAnaliseCirculacao();
                break;
            case 8:
                submenuAnalisesAvancadas();
                break;
            case 9:{
                size_t memoriaTotal = calcularMemoriaTotal(donos, veiculos, sensores, distancias, passagens);
                printf("Memória total ocupada pelas estruturas de dados: %zu bytes\n", memoriaTotal);
                break;
            }
			case 0:
				break;
			default:
				printf("Opcao invalida!\n");
				break;
		}
	}while(op !=0);

	printf("Programa fechado. \n");
	return 0;
}

