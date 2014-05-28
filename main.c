//trabalho pratico de so

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


int main(){
	char str[80];
	int pisos, i, *pedidos;
	pid_t *pidv;
	void *memoria_partilhada = (void *) 0;
	int *pisoactual; //guarda o piso aonde se encontra
	int shmid;
	//int valor;
	shmid = shmget(1000, sizeof(int), 0666|IPC_CREAT); //cria a memoria partilhada
	if (shmid == -1) {
		printf("Erro !!!\n");
		exit(-1);
		} 
	memoria_partilhada = shmat(shmid, (void *)0,0); //liga-se a memoria partilhada
	if (memoria_partilhada == (void *) -1) {
		printf("Erro ao fazer shmat\n");
		exit(-1);
		}
	
	pisoactual=(int *) memoria_partilhada;
	(*pisoactual)=0;
	printf("Indique quantos pisos vai ter o predio: ");
	fgets(str, sizeof(str), stdin); //le a quantidade de pisos que vai ter o predio
	sscanf(str, "%d", &pisos);
	pidv=malloc(sizeof(int)*pisos); //vetor de pids dos processos filhos
	pedidos=malloc(sizeof(int)*pisos); //vetor que guarda o pedidos
	


	
	for(i=0; i<pisos; i++){ //Gera tantos processos filhos, quantos o numero de pisos do predio
		if((pidv[i]=fork())==0){
			sprintf(str, "%d", i);
			execlp("xterm", "xterm", "-e", "./piso", str, NULL);
			exit(0);
		}

	}
	
	sleep(2);

for(i=0; i<pisos; i++){
	wait(NULL);
}

//teste ao vetor de pedidos
memoria_partilhada=shmat(shmid,(void *)0,0);
pedidos=(int *)memoria_partilhada;
printf("Teste pedidos\n");

for(i=0; i<5; i++){
	printf("Piso %d - Destino %d\n", i, pedidos[i]);
}
	

return 0;
}
