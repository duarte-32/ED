#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main(){
	setlocale(LC_ALL,"Portuguese");
	int op;
	printf("Bem vindo!\n");
	do{
		printf("\n-------------------------\n");
		printf("| 1.Registar Dono       |\n");
		printf("| 2.Listar Donos        |\n");
		printf("| 3.Registar Veículo    |\n");
		printf("| 4.Listar Veículos     |\n");
		printf("| 5.Registar Passagem   |\n");
		printf("-------------------------\n");
		scanf("%d",&op);
	}while(op !=0);
	printf("Programa fechado. \n");
	return EXIT_SUCCESS;
}
