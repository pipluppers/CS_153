#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{

        int status;                     //      Status
        argint(0, &status);             //      Update status to the value on top of the user stack
        exit(status);                   //      exit(new status)


//exit(0);
  return 0;  // not reached
}

int
sys_wait(void)
{

        int* status;                    //      Pointer to status
        //      Cast the integer value of status into a char** because argptr takes char**
        argptr(0, (char**)&status, sizeof(*status));
        return wait(status);            //      Return the actual status of wait


  //return wait();                              <--- Original
}

/*      Add to sysproc.c
 *      Add to syscall.c
 *      [SYS_waitpid]   sys_waitpid,
 *      Add to syscall.h #define 22
 */

int
sys_waitpid(void)
{

        //      Fetch the pid
        int pid;
        argint(0, &pid);

        //      Fetch the status pointer
        int* status;
        argptr(0, (char**)&status, sizeof(*status));    // maybe sizeof(int)
//      Options can be anything. Not used
        return waitpid(pid, status, 0);
}


int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
