#include "structs.h"
#include <locale.h>
#include <stdio.h>

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
    while(s != NULL){
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
}

void subMenuAnaliseCirculacao(){
    char dataInicio[30], dataFim[30];
    int opcao;

    limparBuffer(); //Limpar \n pendente do scnaf anterior

    printf("\n--- Submenu Análise de Circulação --\n");


     do {
        printf("\n--- Análises Disponíveis ---\n");
        printf("| 1.Listar veículos por matrícula (por período)     |\n");
        printf("| 2.Ranking de circulação por veículo (total km)    |\n");
        printf("| 3.Ranking por marca (total km)                    |\n");
        printf("| 4.Veículos com velocidade média > 120 km/h        |\n");
        printf("| 5.Ranking de infrações por veículo                |\n");
        printf("| 0.Voltar                                          |\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:

                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);

                listarVeiculosPorMatriculaPeriodo(passagens, veiculos, dataInicio, dataFim);
                break;
            case 2:
                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);
                rankingCirculacao(veiculos, passagens, distancias,
                      dataInicio, dataFim);
                break;
            case 3:
                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);
                rankingPorMarca(veiculos, passagens, distancias,
                dataInicio, dataFim);
                break;
            case 4:
                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);
                listarInfracoesVelocidade(passagens,  veiculos,  distancias,
                              dataInicio,  dataFim);
                break;
            case 5:
                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);
                rankingInfracoesPorVeiculo( passagens,  distancias,  veiculos,
                dataInicio, dataFim);
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
    char dataInicio[30], dataFim[30];

    do {
        printf("\n===== Submenu Análises Avançadas =====\n");
        printf("|   1.Listar velocidades médias por veículo\n");
        printf("|   2.Qual a marca mais rápida              |\n");
        printf("|   3.Qual o carro mais rápido               |\n");
        printf("|   4.Velocidade média por código postal    |\n");
        printf("|   5.Marca mais comum                      |\n");
        printf("|   6.Exportar base de dados para CSV       |\n");
        printf("|   7.Exportar base de dados para XML       |\n");
        printf("|   8.Subnome mais comum dos donos          |\n");
        printf("|   0.Voltar ao menu principal              |\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("Digite a data de inicio (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataInicio);
                printf("Digite a data de fim (DD-MM-AAAA HH:MM:SS.mmm): ");
                scanf(" %[^\n]", dataFim);
                listarVelocidadesMedias(passagens, distancias,  veiculos,
                            dataInicio, dataFim);
                break;
            case 2:
                marcaMaisRapida(veiculos, passagens, distancias,dataInicio,dataFim);
                break;
            case 3:
                carroMaisRapido(passagens, distancias, veiculos, donos);
                break;
            case 4:
                printf("Introduza o código postal: ");
                fgets(codPostal, sizeof(codPostal), stdin);
                codPostal[strcspn(codPostal, "\n")] = '\0';
                velocidadeMediaPorCodigoPostal(donos,veiculos,passagens,distancias,codPostal,dataInicio,dataFim);
                break;
            case 5:
                marcaMaisComum(veiculos);
                break;
            case 6:
                exportarParaCSV(donos, veiculos, sensores, passagens);
                break;
            case 7:
                exportarParaXML(donos, veiculos);
                break;
            case 8:
                subnomeMaisComum(donos);
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
		printf("| 8.Análises Avançadas          |\n");
		printf("| 9.Memoria Total Ocupada       |\n");
		printf("| 0.Sair do Programa            |\n");
		printf("\n----------------------------\n");
		scanf("%d",&op);
		limparBuffer();

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
                printf("Memória total ocupada pelas estruturas de dados: %zu bytes = %0.2lf MB\n", memoriaTotal,(float)memoriaTotal/1000000);
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
