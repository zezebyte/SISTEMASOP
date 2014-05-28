// SOP - António Pinto / ESTGF / 2006
//
// Biblioteca de funcções de manipulação
// de semáforos em C Unix.
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
       /* union semun is defined by including <sys/sem.h> */
       #else
       /* according to X/OPEN we have to define it ourselves */
       union semun {
             int val;                  /* value for SETVAL */
             struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
             unsigned short *array;    /* array for GETALL, SETALL */
                                       /* Linux specific part: */
             struct seminfo *__buf;    /* buffer for IPC_INFO */
       };
#endif

int cria_semaforo (key_t chave, int n_recursos) {
int s;
union semun seu;

	if ((s=semget(chave,1,0777|IPC_CREAT|IPC_EXCL)) == -1) {
		if ((s=semget(chave,1,0777|IPC_CREAT)) == -1) {
			perror("Erro na criaçao do semáforo ");
			exit(-1);
			}
		}
	else 	{
		seu.val=n_recursos;
		semctl(s,0,SETVAL,seu);
		}
return s;
}

int ocupa_recursos (int identificador, int n_recursos) {
struct sembuf down;
down.sem_num=0;
down.sem_op=-n_recursos;
down.sem_flg=0;

return semop(identificador,&down,1);
}

int liberta_recursos (int identificador, int n_recursos) {
struct sembuf up;
up.sem_num=0;
up.sem_op=n_recursos;
up.sem_flg=0;

return semop(identificador,&up,1);
}

int remove_semaforo (int identificador) {
return semctl(identificador,0,IPC_RMID,NULL);
}
