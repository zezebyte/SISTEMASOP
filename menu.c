#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int piso, dest, shmid;
	char str[80];
	void *memoria_partilhada = (void *) 0;
	int *pedidos; //vetor que vai guardar os pedidos
	
	shmid = shmget(1000, sizeof(int), 0666|IPC_CREAT); //cria a memoria partilhada ou utiliza
	if (shmid == -1) {
		printf("Erro !!!\n");
		exit(-1);
		}
		 
	memoria_partilhada = shmat(shmid, (void *)0,0); //liga-se a memoria partilhada
	if (memoria_partilhada == (void *) -1) {
		printf("Erro ao fazer shmat\n");
		exit(-1);
		}
	
	
	sscanf(argv[1], "%d", &piso);
	printf("Menu no Piso %d\n", piso);
	do{
		printf("Indique o piso que pretende ir: ");
		fgets(str, sizeof(str), stdin);
		sscanf(str, "%d", &dest);
		if(dest<0){
			printf("Piso invalido\n");
		}else{
			printf("Pedido registado, aguarde pelo elevador\n");
		}
	}while(dest<0); //!!tambem temos que o piso que introduz esta dentro dos pisos existentes
	pedidos=(int *) memoria_partilhada;
	pedidos[piso]=dest;
	getchar();
	//ira guardar a informacao para um vetor de pedidos, atravez da memoria partilhada
	
	return 0;
}
