#define NPROC    9
#define SSIZE 1024

/******* PROC status ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

typedef struct proc{
    struct proc *next;
    int    *ksp;
    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    int    kstack[SSIZE];      // per proc stack area
    char   name[32];
    int    inUmode;
}PROC;

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;
extern int color;

#include "queue.c"
#include "wait.c"
#include "kernel.c"
#include "u1.c"

char *pname[]={"Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter", "Saturn", "Uranus", "Neptune" };

int init()
{
    PROC *p;
    int i;

    color = 0x0C;
    printf("init ....");

    for (i=0; i<NPROC; i++){   // initialize all procs
           p = &proc[i];
           p->pid = i;
           p->status = FREE;
           p->priority = 0;     
           p->inUmode = 0;
           p->next = &proc[i+1];
           strcpy(proc[i].name, pname[i]);
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue = sleepList = 0;

    /**** create P0 as running ******/
    p = get_proc(&freeList);
    printList("FreeList   ", freeList);

    p->status = RUNNING;
    running = p;
    nproc++;
    printf("done\n");
} 

int scheduler()
{
  if (running->status == RUNNING){
      running->status = READY;
      enqueue(&readyQueue, running);
  }
  running = dequeue(&readyQueue);
  running->status = RUNNING;
  color = running->pid + 0x0A;
  //rflag = 0;

}

main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1){
      printf("P0 running\n");
      if (nproc==2 && proc[1].status != READY)
	  printf("no runable process, system halts\n");
      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();   // P0 switch to run P1

   }
    printf("BACK TO MAIN \n\n\n");
}
