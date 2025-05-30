
#include "structs.h"
#include <locale.h>

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
    }while(opcao != 0);

    return 0;
}

void listarVeiculoSubmenu(){
    int opcao;
    do {
        printf("\n--- Submenu Listar Veículos ---\n");
        printf("1 - Ordenar por Matrícula\n");
        printf("2 - Ordenar por Marca\n");
        printf("3 - Ordenar por Modelo\n");
        printf("4 - Pesquisar por Matricula\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= 3) {
            listarVeiculosOrdenados(veiculos, opcao);
        } else if (opcao == 4){
            pesquisarMatricula();
        } 
        else if (opcao != 0) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
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
		printf("| 7.Memoria Total Ocupada       |\n");
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
            case 7:{
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
