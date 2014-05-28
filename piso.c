#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	void *memoria_partilhada = (void *) 0;
	int *aponta;
	int shmid;
	int piso; 
	pid_t pid;
	printf("Modulo do piso\n");
	
	shmid = shmget(1000, sizeof(int)*1, 0666|IPC_CREAT);
	if (shmid == -1) {
		printf("Erro !!!\n");
		exit(-1);
		} 
		
	memoria_partilhada = shmat(shmid, (void *)0,0);
	if (memoria_partilhada == (void *) -1) {
		printf("Erro ao fazer shmat\n");
		exit(-1);
		}
		
	aponta = (int *) memoria_partilhada;
	sscanf(argv[1], "%d", &piso);

	while(1){
		printf("Este é o piso %d\n", piso);
		printf("Prima ENTER para chamar Elevador\n");
		getchar();
		printf("Chamou o elevador\n");
		execlp("xterm", "xterm", "-e", "./menu", argv[1], NULL); //chama o menu do elevador
		kill(getppid(), SIGUSR1);
		
		system("clear");
	}
	
	
	//tem de enviar um sinal ao pai
	//depois o pai(elevador manda um sinal a pedir qual o piso quer ir
	

	return 0;
}
