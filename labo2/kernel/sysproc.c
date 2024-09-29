#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#define MAX_SEMAPHORES 256

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//abre un semáforo
uint64
sys_sem_open(void){
  int sem, sem_value;

  argint(0, &sem);
  argint(1, &sem_value);

  return (uint64) sem_open(sem, sem_value);
}


// cierra un semáforo 
uint64
sys_sem_close(void){
  int sem;
  
  argint(0, &sem); // obtiene el valor del semáforo

  return (uint64) sem_close(sem);
}

// incrementa su valor. Libera un recurso
uint64
sys_sem_up(void){
  int sem;

  argint(0, &sem);

  return (uint64) sem_up(sem);
}

// decrementa su valor. Solicita un recurso
uint64 
sys_sem_down(void){
  int sem;

  argint(0, &sem);

  return (uint64) sem_down(sem);
}

uint64
sys_search_sem(void){
  int sem_value;

  argint(0, &sem_value);
  
  return (uint64) search_sem(sem_value);
}