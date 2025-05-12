#include "structs.h"
#include <locale.h>

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

int main(){
	int op;
	setlocale(LC_ALL, "Portuguese");
	do{
		printf("******VIA VERMELHA******");
		printf("\n-------------------------\n");
		printf("| 1.Registar Dono       |\n");
		printf("| 2.Listar Donos        |\n");
		printf("| 3.Registar Veiculo    |\n");
		printf("| 4.Listar Veiculos     |\n");
		printf("| 5.Registar Passagem   |\n");
		printf("-------------------------\n");
		scanf("%d",&op);
		switch(op){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				printf("Opção inválida!\n");
				break;
		}
	}while(op !=0);
	printf("Programa fechado. \n");
	return 0;
}
