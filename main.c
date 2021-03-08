#include <time.h>
#include "memory.h"

int main(int argc, char *argv[]){
	if(argc < 5){
		return 0;
	}
	
	printf("Iniciando execução...\n");
	printf("Arquivo de entrada: %s\n", argv[2]);
	printf("Tamanho da memória: %s KB\n",argv[4] );
	printf("Tamanho da página: %s KB\n", argv[3]);
	printf("Método de substituição: %s\n", argv[1]);

	Memory* mem = CreateMemory(atoi(argv[3]), atoi(argv[4]));
	
	double time = 0.0;
	clock_t begin = clock();
	RunMemory(mem,argv[2],argv[1]);
	clock_t end = clock();
	time += (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("Total de acessos: %d\n", mem->num_data);
	printf("Page faults: %lld\n", mem->pageFaults);
	printf("Dirty pages: %lld\n", mem->dirtyBits);
	printf("Memoria lida: %lld\n", mem->readBits);
	printf("Memoria escrita: %lld\n",mem->writtenBits);
	printf("Tempo de execucao: %f segundos\n",time);
	
	printf("Tabela de páginas final:\n");
	printf("\tÍndice\tPágina\tdirty bit\n");
	for (int i = 0; i < mem->max_frames_num; i++){
		printf("\t%d\t%d\t%d\n", i, mem->p_frames[i].virtual_id, mem->p_frames[i].written_);
	}
	
	
	DestroyMemory(mem);
	freeQueue();
	return 0;
}