#include<locale.h> 
#include "memory.h"

int main()
{
	setlocale(LC_ALL,"");
	printf("Iniciando execu��o...\n");
	printf("Arquivo de entrada:\n");
	printf("Tamanho da mem�ria:\n");
	printf("Tamanho da p�gina:\n");
	printf("M�todo de substitui��o:\n");
	
	Memory* mem = CreateMemory(4, 128);
	printf("Mem�ria criada\n");
	RunMemory(mem);
	
	printf("Total de acessos: %d\n", mem->num_data);
	printf("Frames alocados: %d\n",mem->occupied_frames_num);
	printf("Valor de s: %d\n", mem->s);
	
	
	printf("P�ginas alocadas:\n");
	for (int i = 0; i < mem->max_frames_num; i++){
		printf("P�gina %d\n", mem->p_frames[i].virtual_id);
	}
	
	
	DestroyMemory(mem);
	return 0;
}