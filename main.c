#include <time.h>
#include "memory.h"

int main(int argc, char *argv[]){

	if(argc < 3){
		return 0;
	}
	
	int page_size = 4;
	int mem_size = 128;
	double time;
	FILE *arq;
	arq = fopen(argv[3],"w");
	if(arq == NULL){
		printf("Arquivo nao encontrado");
		exit(EXIT_FAILURE);
	}
	printf("%s\n",argv[3]);
	while(mem_size <= 16384){
		printf("Tamanho de memoria: %d\n",mem_size);
		Memory* mem = CreateMemory(page_size, mem_size);
		printf("Memória criada\n");
		time = 0;
		clock_t begin = clock();
		RunMemory(mem,argv[2],argv[1]);
		clock_t end = clock();
		time += (double)(end - begin) / CLOCKS_PER_SEC;
		
		printf("Escrevendo no arquivo...\n");
		printf("\n");
		fprintf(arq, "%lld,%lld,%f\n",mem->pageFaults,mem->dirtyBits,time);
		
		DestroyMemory(mem);
		mem_size = mem_size + 128;
		freeQueue();
	}
	fclose(arq);
	printf("Arquivo criado com sucesso.");

	return 0;
}