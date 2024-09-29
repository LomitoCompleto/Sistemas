#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void ping(int sem_ping, int sem_pong, int count){
    for(unsigned int i = 0; i < count; i++){
        sem_down(sem_ping);     // Espera hasta que sea su turno
        printf("ping\n");       // Imprime "ping"
        sem_up(sem_pong);       // Despierta a pong
    }
}

void pong(int sem_pong, int sem_ping, int count){
    for(unsigned int i = 0; i < count; i++){
        sem_down(sem_pong);     // Espera hasta que sea su turno
        printf("\tpong\n");     // Imprime "pong"
        sem_up(sem_ping);       // Despierta a ping
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("ERROR: not enough arguments\n");
        printf("%s\n",argv[0]);
        return (0);
    }

    int length = atoi(argv[1]);

    if (length < 1){
        printf("ERROR: the argument has to be greater than 0\n");
        return 0;
    }  

    // Abrir semáforos
    int sem_ping = search_sem(1);
    int sem_pong = search_sem(0);
    
    sem_open(sem_ping, 1);   // Inicializa semáforo de ping en 1 para que inicie
    sem_open(sem_pong, 0);   // Inicializa semáforo de pong en 0, esperando su turno

    int pid = fork();
    if(pid == 0){
        // Proceso hijo: pong
        pong(sem_pong, sem_ping, length);
        sem_close(sem_pong);  // Cierra el semáforo después de que el proceso hijo termina
        exit(0);
    } else {
        // Proceso padre: ping
        ping(sem_ping, sem_pong, length);
        wait(0);
        sem_close(sem_ping);  // Cierra el semáforo después de que el proceso padre termina
    }

    exit(0);
}