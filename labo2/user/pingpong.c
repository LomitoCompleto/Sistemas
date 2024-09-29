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
    printf("\tpong\n");
    sem_up(sem_father); 
}


int main(int argc, char *argv[]){
    if(argc != 2){
        printf("ERROR: not enough arguments\n");
        printf("%s\n",argv[0]);
        return (0);
    }

    
    if (atoi(argv[1])< 1){
        printf("ERROR: the argument has to be greater than 0\n");
        return 0;
    }    
    
    
    int sem_son = search_sem(0);   // hijo
    int sem_father = search_sem(1);   // padre

    sem_up(sem_father);
    sem_down(sem_son);


    int pid = fork();

    if (pid == -1){
        return(0);
    }else if(pid == 0){
        for(unsigned int i = 0; i<atoi(argv[1]); i++){
            pong(sem_son, sem_father);
            
        }
        return(0);
    }else{
        for(unsigned int i = 0; i<atoi(argv[1]); i++){
            ping(sem_father, sem_son);
           
            wait(0);
        }
    }
    
    sem_close(sem_son);
    sem_close(sem_father);
        
    return(0);
}

