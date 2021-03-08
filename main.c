#include <locale.h>

#include "memory.h"

int main(int argc, char *argv[]){
	if(argc < 4){
		return 0;
	}
	setlocale(LC_ALL,"");
	printf("Iniciando execução...\n");
	printf("Arquivo de entrada: %s\n", argv[2]);
	printf("Tamanho da memória: %s KB\n",argv[4] );
	printf("Tamanho da página: %s KB\n", argv[3]);
	printf("Método de substituição: %s\n", argv[1]);

	Memory* mem = CreateMemory(atoi(argv[3]), atoi(argv[4]));
	printf("Memória criada\n");
	RunMemory(mem,argv[2],argv[1]);
	
	printf("Total de acessos: %d\n", mem->num_data);
	printf("Frames alocados: %d\n",mem->occupied_frames_num);
	printf("Valor de s: %d\n", mem->s);
	printf("Page faults: %lld\n", mem->pageFaults);
	printf("Dirty pages: %lld\n", mem->dirtyBits);
	printf("Memoria lidas: %lld\n", mem->readBits);
	printf("Memoria escrita: %lld\n",mem->writtenBits);
	
	
	printf("Páginas alocadas:\n");
	for (int i = 0; i < mem->max_frames_num; i++){
		printf("Página %d\n", mem->p_frames[i].virtual_id);
	}
	
	
	DestroyMemory(mem);
	freeQueue();
	return 0;
}