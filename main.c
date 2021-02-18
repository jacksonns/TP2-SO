#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *arq;
	arq = fopen("compilador.log","r");
	if(arq == NULL){
		printf("Arquivo nao encontrado");
	}
	
	unsigned address;
	char rw;
	fscanf(arq, "%x %c", &address, &rw);
	printf("%p %c", (void *) address, rw);
	
	fclose(arq);
	

	return 0;
}