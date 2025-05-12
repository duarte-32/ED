#include "structs.h"

void CarregarVeiculos(const char* arquivo){
  FILE* f = fopen("carros.txt","r");
  if(!f)return;
  fclose(f);
}
void ListarVeiculos();
