#include <stdio.h>
#include <stdlib.h>
#include<locale.h> 

#include "memory.h"

int main()
{
	setlocale(LC_ALL,"");
	printf("Iniciando execução...\n");
	printf("Arquivo de entrada:\n");
	printf("Tamanho da memória:\n");
	printf("Tamanho da página:\n");
	printf("Método de substituição:\n");
	
	Memory* mem = CreateMemory(4, 128);
	printf("Memória criada\n");
	RunMemory(mem);
	
	printf("Total de acessos: %d\n", mem->num_data);
	printf("Frames alocados: %d\n",mem->occupied_frames_num);
	printf("Valor de s: %d\n", mem->s);
	
	
	printf("Páginas alocadas:\n");
	for (int i = 0; i < mem->max_frames_num; i++){
		printf("Página %d\n", mem->p_frames[i].virtual_id);
	}
	
	
	DestroyMemory(mem);
	return 0;
}