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
  int max_value;
} semaphore;

semaphore sem_t[MAX_SEMAPHORES];

// inicializa los semáforos
void sem_init(void){

  for(unsigned int i = 0; i < MAX_SEMAPHORES; i++){
    sem_t[i].value = -1;
  }
}

//abre un semáforo

int sem_open(int sem, int sem_value){

  if (sem >= MAX_SEMAPHORES || sem < 0 || sem_t[sem].value >=0 || sem_value <= 0) {
      printf("ERROR: invalid semaphore ID.\n");
      return 0;
  }
  
  initlock(&sem_t[sem].lock, "Semaphore");
  acquire(&(sem_t[sem].lock));
  
  sem_t[sem].value = sem_value;
  sem_t[sem].max_value = sem_value;
  
  release(&sem_t[sem].lock);

  return 1;
}


// cierra un semáforo 
int sem_close(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES){
    return 0; // error si el valor no es válido
  }

  acquire(&sem_t[sem].lock); // adquiere el semáforo

  sem_t[sem].value = 0; // cierra el semáforo

  wakeup(&sem_t[sem]); // despierta los procesos que estén esperando

  release(&sem_t[sem].lock); // libera el lock

  return 1;
}

int sem_up(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES|| sem_t[sem].value >= sem_t[sem].max_value ){
    return 0; 
  }

  acquire(&sem_t[sem].lock); // acceso exclusivo al semáforo

  if(sem_t[sem].value ==0){
    wakeup(&sem_t[sem]); // despierta los procesos que estén esperando luego de haber liberado un recurso
  }
  sem_t[sem].value++; // incrementa el valor del semáforo

  
  release(&sem_t[sem].lock);

  return 1;

}
int sem_down(int sem){

  if(sem < 0 || sem >= MAX_SEMAPHORES){
    return 0; 
  }

  acquire(&sem_t[sem].lock); // acceso exclusivo al semáforo
  
  if(sem_t[sem].value == 0){
    sleep(&sem_t[sem], &sem_t[sem].lock); // el proceso entra en estado de suspensión si no hay recursos disponibles
  } 
  if(sem_t[sem].value > 0){
    sem_t[sem].value--; // decrementa el valor del semáforo
  }
  release(&sem_t[sem].lock);

  return 1;
  
}

int
get_sem(int value)
{
    int sem = 0;

    while (sem < MAX_SEMAPHORES && !sem_open(sem, value)) {
        ++sem;
    }

    return (sem == MAX_SEMAPHORES) ? -1 : sem; //tengo que ver como modificarlo
}
