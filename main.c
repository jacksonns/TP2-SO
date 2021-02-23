#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA 1000001  //Quantidade máxima de endereços 

typedef struct DataAccess{
	unsigned address;
	char rw;
}DataAccess;

typedef struct PageFrame{
	int virtual_id;
	int read;     //Se foi lida (0 ou 1)
	int written;  //Se foi escrita (0 ou 1)
}Page;

typedef struct VirtualMemory{
	DataAccess *data_access;
	Page *p_frames;    //Conjunto de páginas, tem tamanho de "max_frame_num"
	int num_data;      //Quantidade de endereços utilizados.
	int max_frames_num;
	int page_size;
	int mem_size;
	int occupied_frames_num;
}Memory;

Memory* CreateMemory(int page_size, int memory_size){
	Memory *vir_mem;
	vir_mem = (Memory*) malloc(sizeof(Memory));
	
	//Iniciando alguns valores.
	vir_mem->page_size = page_size;
	vir_mem->mem_size = memory_size;
	vir_mem->max_frames_num = memory_size/page_size;
	vir_mem->occupied_frames_num = 0;
	
	//Coloca dados do arquivo em data_access
	FILE *arq;
	arq = fopen("compilador.log","r");
	if(arq == NULL){
		printf("Arquivo nao encontrado");
		exit(EXIT_FAILURE);
	}
	
	vir_mem->data_access = malloc(MAX_DATA * sizeof(DataAccess));
	int access_number = 0;
	unsigned address;
	char rw;
	while(fscanf(arq, "%x %c", &address, &rw) == 2){
		if (access_number >= MAX_DATA){
			exit(EXIT_FAILURE);
		}
		vir_mem->data_access[access_number].address = address;
		vir_mem->data_access[access_number].rw = rw;
		access_number++;
	}
	vir_mem->num_data = access_number;
	fclose(arq);
	
	//Criando os Page Frames e iniciando todos como vazio.
	vir_mem->p_frames = malloc(vir_mem->max_frames_num * sizeof(DataAccess));
	for (int i = 0; i < vir_mem->max_frames_num; i++){
		vir_mem->p_frames[i].virtual_id = -1;
		vir_mem->p_frames[i].read = 0;
		vir_mem->p_frames[i].written = 0;
	}
	
	return vir_mem;
}

int main()
{
	Memory* mem = CreateMemory(4, 128);
	
	for (int i = 0; i < 20; i++){
		printf("%p %c \n", (void *) mem->data_access[i].address, mem->data_access[i].rw);
	}
	free(mem->data_access);
	free(mem->p_frames);
	free(mem);
	return 0;
}