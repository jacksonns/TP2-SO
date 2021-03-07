#include <stdio.h>
#include <stdlib.h>

//#define MAX_DATA 1000001   

/*
typedef struct DataAccess{
	unsigned address;
	char rw;
}DataAccess;
*/
struct node{
	int data;
	struct node *next;
};

struct node *front = -1;
struct node *back = -1;
int j = 0;
void enqueue(int frame){
	struct node *newNode;
	newNode = (struct node *)malloc(sizeof(struct node));
	newNode->data = frame;
	newNode->next = 0;
	if(back == -1){
		front=back=newNode;
		back->next = front;
	}
	else{
		back->next = newNode;
		back = newNode;
		back->next = front;
	}
}

void dequeue(){
	struct node *temp;
	temp = front;
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else if(front == back){
		front = back = -1;
		free(temp);
	}
	else{
		front = front->next;
		back->next = front;
		free(temp);
	}
}

int frontElement(){
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else{
		return front->data;
	}
}

void display(){
	struct node *temp;
	temp = front;
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else{
		while (temp->next != front){
			dequeue();
		}
		dequeue();
	}
}

typedef struct PageFrame{
	int virtual_id;
	int read_;     //Se foi lida (0 ou 1)
	int written_;  //Se foi escrita (0 ou 1)
	long long recUsed ; // acessado recentemente
	int refBit; //um bit de referencia (0 ou 1)
}Page;

typedef struct VirtualMemory{
	//DataAccess *data_access;  
	Page *p_frames;    //Conjunto de páginas, tem tamanho de "max_frames_num"
	//int num_data;      //Quantidade de endereços utilizados.
	int max_frames_num;
	int page_size;      //Tamanho da página em KB
	int mem_size;       //Tamanho da memória em KB
	int s;             //Quantidade de bits a serem descartados
	int occupied_frames_num;
}Memory;

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
	vir_mem->s = Get_s(vir_mem->page_size);
	
	//Coloca dados do arquivo em data_access
	
	/*
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
	*/
	
	//Criando os Page Frames e iniciando todos como vazio.
	vir_mem->p_frames = malloc(vir_mem->max_frames_num * sizeof(Page));
	for (int i = 0; i < vir_mem->max_frames_num; i++){
		vir_mem->p_frames[i].virtual_id = -1;
		vir_mem->p_frames[i].read_ = 0;
		vir_mem->p_frames[i].written_ = 0;
		vir_mem->p_frames[i].recUsed = 0;
	}
	
	return vir_mem;
}

//Função que retorna o índice do frame que possui o endereço virtual dado
int FrameIndex (Memory *mem, int virtual_id){
	for (int i = 0; i < mem->occupied_frames_num; i++){
		if (mem->p_frames[i].virtual_id == virtual_id){
			return i;
		}
	}
	return -1;
}

int FIFO(){
	int element = 0;
	element = frontElement();
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
	return element;
}

int secondChance(Memory *mem){
	int element = 0;
	int firstElement = 0;
	struct node *temp;
	temp = front;
	while(temp->next != NULL){
		if(mem->p_frames[temp->data].refBit == 0){
			element = temp->data;
			break;
		}
		else{
			mem->p_frames[temp->data].refBit = 0;
		}
	}
	firstElement = frontElement();
	dequeue();
	enqueue(firstElement);
	return element;
}

unsigned customReplace(Memory *mem){
	int frame = 0;
	frame = rand() % mem->max_frames_num;
	return frame;
}

 void RunMemory (Memory *mem){
	 //Processa cada endereço contido no arquivo.
	 unsigned address;
	 char rw;
	 int i = 0;
	 long long used = 0;
	 int page_id, frame;
	 
	 FILE *arq;
	 arq = fopen("compilador.log","r");
	 if(arq == NULL){
	 	 printf("Arquivo nao encontrado");
		 exit(EXIT_FAILURE);
	 }
	 while(fscanf(arq, "%x %c", &address, &rw) == 2){
		 page_id = address >> mem->s;
		 frame = FrameIndex(mem, page_id);
		 printf("%d\n",page_id);
		 if(frame == -1){
			 //Page fault
			 //Se tem espaço na tabela de páginas, adiciona página
			 if(mem->occupied_frames_num < mem->max_frames_num){
				 enqueue(i);
				 mem->p_frames[i].virtual_id = page_id;
				 mem->p_frames[i].read_ = 1;
				 mem->p_frames[i].recUsed = used;
				 mem->p_frames[i].refBit = 1;
				 if(rw == 'W'){
					 mem->p_frames[i].written_ = 1;
				}
				mem->occupied_frames_num++;
				i++;
			 }
			 
			 //Se não tem mais espaço, escolhe uma página para ser substituída 
			 else{
				 //ALGORITMO DE SUBSTITUIÇÃO ENTRA AQUI
				frame = secondChance(mem);
				 //Escolhendo aleatoriamente:
				 mem->p_frames[frame].read_ = 0;
				 mem->p_frames[frame].written_ = 0;
				 
				 mem->p_frames[frame].virtual_id = page_id;
				 mem->p_frames[frame].read_ = 1;
				 mem->p_frames[frame].recUsed = used;
				 mem->p_frames[frame].refBit = 1;
			     if(rw == 'W'){
					 mem->p_frames[frame].written_ = 1;
			    } 
			 }
		 }else{
			 //Achou página
			 mem->p_frames[frame].read_ = 1;
			 mem->p_frames[frame].recUsed = used;
			 mem->p_frames[frame].refBit = 1;
			 if(rw == 'W'){
				 mem->p_frames[frame].written_ = 1;
			 }
		 }
		used++;
	 }
	 fclose(arq);
 }

int main()
{
	//printf("Iniciando execução\n");
	
	Memory* mem = CreateMemory(4, 128);
	
	printf("Número de frames: %d\n", mem->max_frames_num);
	printf("Valor de s: %d\n", mem->s);
	/*
	for (int i = 0; i < mem->num_data; i++){
		printf("%p %c \n", (void *) mem->data_access[i].address, mem->data_access[i].rw);
	}
	*/
	
	
	RunMemory(mem);
	
	/*
	printf("Frames alocados: %d\n",mem->occupied_frames_num);
	printf("Valor de s: %d\n", mem->s);
	printf("Páginas alocadas:\n");
	for (int i = 0; i < mem->max_frames_num; i++){
		printf("Página %d Lida? %d \n", mem->p_frames[i].virtual_id, mem->p_frames[i].read_);
	}
	
	free(mem->p_frames);
	free(mem);
	*/
    display();
	return 0;
}