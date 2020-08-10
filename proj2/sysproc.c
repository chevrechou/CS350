#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "kthread.h"
// #include <stdio.h>


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (proc->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_procdump(void)
{
  procdump();
  return 0;
}


// ### KTHREAD IMPLEMNTATION ###

int sys_kthread_create(void* (*start_func)(), void* stack, int stack_size) {
  // int start;
  // int stack;
  // int size;
  // // if (argint(0, &start) < 0 || (argint(1, &stack) < 0) || (argint(2, & size) < 0) ) {
  // //   return -1;
  // // }

  return kthread_create((void *) start_func, (void *) stack, (int) stack_size);

}

int sys_kthread_mutex_alloc() {
  return kthread_mutex_alloc();
}

int sys_kthread_id(void) {
  return kthread_id();
}

int sys_kthread_join(void) {
  int mutex_id;
  if (argint(0, &mutex_id) < 0) {
    return -1;
  }
  return kthread_id(mutex_id);

}


int sys_kthread_mutex_unlock(void) {
  int mutex_id;
  if (argint(0, &mutex_id) < 0) {
    return -1;
  }
  return kthread_mutex_unlock(mutex_id);
}

int sys_kthread_mutex_lock(void) {

// printf("%s\n", "LEROY");
  int mutex_id;
  if (argint(0, &mutex_id) < 0) {
    return -1;
  }

  return kthread_mutex_lock(mutex_id);
}

int sys_kthread_mutex_dealloc(void) {

  int mutex_id;

  if (argint(0, &mutex_id) < 0) {
    return -1;

  }
  return kthread_mutex_dealloc(mutex_id);
}

int sys_kthread_exit(void) {
  kthread_exit();
  return 0;
}