#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void ping(int sem_father, int sem_son){
    sem_down(sem_father);  // espera a que el semáforo esté desbloqueado
    printf("ping\n");
    sem_up(sem_son);    // libera el semáforo para que "pong" pueda ser ejecutado
}

void pong(int sem_son, int sem_father){
    sem_down(sem_son); 
    printf("pong\n");
    sem_up(sem_father); 
}


int main(int argc, char *argv[]){
    if(argc != 2){
        printf("ERROR: not enough arguments\n");
        printf("%s\n",argv[0]);
        exit (1);
    }

    
    if (atoi(argv[1])< 1){
        printf("ERROR: the argument has to be greater than 0\n");
        return 0;
    }    
    
    int pong_sem = get_sem(0);   // hijo
    int ping_sem = get_sem(1);   // padre

    
    int pid = fork();

    if (pid == -1){
        return(0);
    }else if(pid == 0){
        for(int i = 0; i<atoi(argv[1]); i++){
            pong(pong_sem, ping_sem);
        }
        exit(0); //???
    }else{
        for(int i = 0; i<atoi(argv[1]); i++){
          ping(ping_sem, pong_sem);
        }
    }
       
    wait(0);
    sem_close(pong_sem);
    sem_close(ping_sem);
        
    exit(0);
}


