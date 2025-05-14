#include "structs.h"
#include <locale.h>

// Listas vazias globais
extern Dono *donos;
extern Veiculo *veiculos;
extern Sensor *sensores;
extern Distancia *distancias;
extern Passagem *passagens;



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
		printf("| 1.Registar Dono       |\n");
		printf("| 2.Listar Donos        |\n");
		printf("| 3.Registar Veiculo    |\n");
		printf("| 4.Listar Veiculos     |\n");
		printf("| 5.Registar Passagem   |\n");
		printf("| 0.Sair do Programa    |\n");
		printf("-------------------------\n");
		scanf("%d",&op);
		switch(op){
			case 1:
				// RegistarDono();
				break;
			case 2:
				ListarDonos();
				break;
			case 3:
				// RegistarVeiculo();
				break;
			case 4:
				ListarVeiculos();
				break;
			case 5:
				// RegisarPassagem();
				break;
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
