#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"  // Include the header file where struct proc is defined
#include <stdbool.h>

#define MAX_SEMAPHORES 10


typedef struct semaphore{
  int value;              // representa el estado del semáforo
  struct spinlock lock;   // asegura que solo un proceso modifique el valor del semáforo a la vez
} semaphore;

semaphore sem_t[MAX_SEMAPHORES];

// inicializa los semáforos
void sem_init(void){
  for(unsigned int i = 0; i < MAX_SEMAPHORES; i++){
    sem_t[i].value = -1;
    initlock(&sem_t[i].lock, "Semaphore");
  }
  
}


//abre un semáforo


int sem_open(int sem, int sem_value) {
  if (sem < 0 || sem >= MAX_SEMAPHORES ||  sem_t[sem].value != -1 || sem_value < 0) {
    return 0;                                   //El semaforo es invalido, retornamos cero que es error
  }

  acquire(&(sem_t[sem].lock));                              //Bloqueamos 
  sem_t[sem].value = sem_value;                               //Le asigamos el valor 
  release(&(sem_t[sem].lock));                              //Desbloqueamos

  return 1;                                       //Retornamos uno que es exito
}

// cierra un semáforo 
int sem_close(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES){
    return 0; // error si el valor no es válido
  }

  acquire(&sem_t[sem].lock); // adquiere el semáforo

  sem_t[sem].value = -1; // cierra el semáforo

  release(&sem_t[sem].lock); // libera el lock

  return 1;
}

int sem_up(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES || sem_t[sem].value == -1){
    return 0; 
  }

  acquire(&sem_t[sem].lock); // acceso exclusivo al semáforo

  if(sem_t[sem].value == 0){
    wakeup(&sem_t[sem]); // despierta los procesos que estén esperando luego de haber liberado un recurso
  }

  sem_t[sem].value++; // incrementa el valor del semáforo ??????????????????????????????????????????????????

  
  release(&sem_t[sem].lock);

  return 1;

}
int sem_down(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES || sem_t[sem].value == -1){
    return 0; 
  }

  acquire(&sem_t[sem].lock); // acceso exclusivo al semáforo
  
  if(sem_t[sem].value == 0){
    sleep(&sem_t[sem], &sem_t[sem].lock); // el proceso entra en estado de suspensión si no hay recursos disponibles
  } 
 
  sem_t[sem].value--; // decrementa el valor del semáforo

  release(&sem_t[sem].lock);

  return 1;
  
}

int search_sem(int sem_value) {
  
    acquire(&sem_t[sem_value].lock);  // Bloquea el acceso al arreglo sem_t[] para evitar condiciones de carrera

    int i = 0;
   while(i < MAX_SEMAPHORES && sem_open(i, sem_value) == 0) { 
    i++;
  }

    release(&sem_t[sem_value].lock);  // Desbloquea el acceso después de la búsqueda

    if (i == MAX_SEMAPHORES) {
        return -1;  // Si no se encuentra un semáforo disponible
    }

    return i;  // Devuelve el índice del primer semáforo disponible
}
