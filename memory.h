#ifndef MEMORY_H
#define MEMORY_H

#include "queue.h"

//Quadro de páginas
typedef struct PageFrame{
	int virtual_id;
	int read_;     //Se foi lida (0 ou 1)
	int written_;  //Se foi escrita (0 ou 1)
	long long recUsed ; // acessado recentemente
	int refBit; //um bit de referencia (0 ou 1)
}Page;

//Memória
typedef struct VirtualMemory{ 
	Page *p_frames;    //Conjunto de páginas, tem tamanho de "max_frames_num"
	int num_data;      //Quantidade de endereÃ§os utilizados.
	int max_frames_num;
	int page_size;      //Tamanho da página em KB
	int mem_size;       //Tamanho da memória em KB
	int s;             //Quantidade de bits a serem descartados
	int occupied_frames_num;
	long long pageFaults; 
	long long dirtyBits;
	long long readBits;
	long long writtenBits;
}Memory;

//Funções construtoras e destrutoras
int Get_s (int page_size);
Memory* CreateMemory(int page_size, int memory_size);
void DestroyMemory (Memory *mem);

//Algoritmos de substituição
int useAlgorithm(Memory *mem, int size, char *n);
int FIFO(Memory *mem);
int LRU(Memory *mem,int size);
int secondChance(Memory *mem);
unsigned customReplace(Memory *mem);

//Funções para rodar memória
int FrameIndex (Memory *mem, int virtual_id);
void RunMemory (Memory *mem, char *file, char *algo);

#endif