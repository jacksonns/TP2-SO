#include "memory.h"

//CONSTRUÇÃO E DESCONSTRUÇÃO
int Get_s (int page_size){
	unsigned temp;
	temp = page_size * 1024;
	unsigned s = 0;
	while(temp > 1){
		temp = temp >> 1;
		s++;
	}
	return s;
}

Memory* CreateMemory(int page_size, int memory_size){
	Memory *vir_mem;
	vir_mem = (Memory*) malloc(sizeof(Memory));
	
	//Iniciando alguns valores.
	vir_mem->page_size = page_size;
	vir_mem->mem_size = memory_size;
	vir_mem->max_frames_num = memory_size/page_size;
	vir_mem->occupied_frames_num = 0;
	vir_mem->num_data = 0;
	vir_mem->pageFaults = 0;
	vir_mem->dirtyBits = 0;
	vir_mem->readBits = 0;
	vir_mem->writtenBits = 0;
	vir_mem->s = Get_s(vir_mem->page_size);
	
	//Criando os Page Frames e iniciando todos como vazio.
	vir_mem->p_frames = malloc(vir_mem->max_frames_num * sizeof(Page));
	for (int i = 0; i < vir_mem->max_frames_num; i++){
		vir_mem->p_frames[i].virtual_id = -1;
		vir_mem->p_frames[i].read_ = 0;
		vir_mem->p_frames[i].written_ = 0;
		vir_mem->p_frames[i].recUsed = 0;
		vir_mem->p_frames[i].refBit = 0;
	}
	
	return vir_mem;
}

void DestroyMemory (Memory *mem){
	free(mem->p_frames);
	free(mem);
};


//ALGORITMOS DE SUBSTITUIÇÃO
int FIFO(Memory *mem){
	int element = 0;
	element = frontElement();
	if(mem->p_frames[element].written_ == 1){
		mem->dirtyBits++;
	}
	dequeue();
	enqueue(element);
	return element;
}

int LRU(Memory *mem,int size){
	long long min = 100000000;
	int element = 0;
	for(int i=0;i<size;i++){
		if(mem->p_frames[i].recUsed < min){
			min = mem->p_frames[i].recUsed;
			element = i;
		}
	}
	if(mem->p_frames[element].written_ == 1){
		mem->dirtyBits++;
	}
	return element;
}

int secondChance(Memory *mem){
	int element = 0;
	int firstElement = 0;
	int value = 1;
	struct node *temp;
	temp = getFront();
	while(value){
		if(mem->p_frames[temp->data].refBit == 0){
			element = temp->data;
			value = 0;
		}
		else{
			mem->p_frames[temp->data].refBit = 0;
		    temp = temp->next;
		}
	}
	firstElement = frontElement();
	dequeue();
	enqueue(firstElement);
	if(mem->p_frames[element].written_ == 1){
		mem->dirtyBits++;
	}
	return element;
}

unsigned customReplace(Memory *mem){
	int element = 0;
	element = rand() % mem->max_frames_num;
	if(mem->p_frames[element].written_ == 1){
		mem->dirtyBits++;
	}
	return element;
}

int useAlgorithm(Memory *mem, int size, char *n){
	if(strcmp(n,"lru") == 0){
		return LRU(mem,size);
	}
	else if(strcmp(n,"2a") == 0){
		return secondChance(mem);
	}
	else if(strcmp(n,"fifo") == 0){
		return FIFO(mem);
	}
	else{
		return customReplace(mem);
	}
}


//ALGORITMO DE EXECUÇÃO
//Função que retorna o índice do frame que possui o endereço virtual dado
int FrameIndex (Memory *mem, int virtual_id){
	for (int i = 0; i < mem->occupied_frames_num; i++){
		if (mem->p_frames[i].virtual_id == virtual_id){
			return i;
		}
	}
	return -1;
}

 void RunMemory (Memory *mem, char *file, char *algo){
	 //Processa cada endereço contido no arquivo.
	 unsigned address;
	 char rw;
	 int i = 0;
	 long long used = 0;
	 unsigned page_id;
	 int frame;
	 
	 FILE *arq;
	 arq = fopen(file,"r");
	 if(arq == NULL){
	 	 printf("Arquivo nao encontrado");
		 exit(EXIT_FAILURE);
	 }
	 
	 while(fscanf(arq, "%x %c", &address, &rw) != EOF){
		 
		 page_id = address >> mem->s;
		 frame = FrameIndex(mem, page_id);
		 
		 //printf("Endereço: %x\n", address);
		 
		 if(frame == -1){
			 //Page fault
			 mem->pageFaults++;
			 //Se tem espaço na tabela de páginas, adiciona página
			 if(mem->occupied_frames_num < mem->max_frames_num){
				 enqueue(i);
				 mem->p_frames[i].virtual_id = page_id;
				 mem->p_frames[i].read_ = 1;
				 mem->p_frames[i].recUsed = used;
				 if(rw == 'W'){
					 mem->p_frames[mem->occupied_frames_num].written_ = 1;
					 mem->writtenBits++;
				 }else{
					 mem->readBits++;
				 }
				 mem->occupied_frames_num++;
				 i++;
			 }
			 //Se não tem mais espaço, escolhe uma página para ser substituída 
			 else{
				 //ALGORITMO DE SUBSTITUIÇÃO ENTRA AQUI
				 frame = useAlgorithm(mem,mem->max_frames_num,algo);
				 mem->p_frames[frame].read_ = 0;
				 mem->p_frames[frame].written_ = 0;
				 
				 mem->p_frames[frame].virtual_id = page_id;
				 mem->p_frames[frame].read_ = 1;
				 mem->p_frames[frame].recUsed = used;
			     if(rw == 'W'){
					 mem->p_frames[frame].written_ = 1;
					 mem->writtenBits++;
			     }else{
					 mem->readBits++;
				 }
			 }
		 }else{
			 //Achou página
			 mem->p_frames[frame].read_ = 1;
			 mem->p_frames[frame].recUsed = used;
			 mem->p_frames[frame].refBit = 1;
			 if(rw == 'W'){
				 mem->p_frames[frame].written_ = 1;
				 mem->writtenBits++;
			 }else{
				mem->readBits++;
			 }
		 }
		 used++;
	 } 
	 mem->num_data = used;
	 fclose(arq);
 }