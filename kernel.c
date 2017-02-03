/****************** kernel.c file *************************/
int body();

PROC *kfork()
{
      PROC *p = get_proc(&freeList);
      int i;
      if (p==0){
         printf("no more PROC, kfork() failed\n");
         return 0;
      }
      printf("PROC %d forked\n", p->pid);
      p->status = READY;
      p->priority = 1;          // priority = 1 for all proc except P0
      p->ppid = running->pid;   // parent = running
      p->parent = running;

      p->kstack[SSIZE-1] = (int)body;
      for (i=2; i<10; i++)
	    p->kstack[SSIZE-i]= 0 ;   // Registers?
      p->ksp = &(p->kstack[SSIZE-9]);
      enqueue(&readyQueue, p);  // enter p into readyQueue by priority
      nproc++;
      return p;                 // return child PROC pointer
}         

int do_sleep()
{
  int sleepValue;
  if (running->pid == 0){
      printf("Can't put P0 to sleep!%c\n",007);
      return -1;
  }
  printf("enter a value to sleep for (0-9) : ");
  sleepValue = (getc()&0x7F) - '0'; 
  printf("%d\n", sleepValue);
  ksleep(sleepValue);
}

int do_wakeup() 
{
  int wakeupValue;
  printf("enter a event to trigger (0-9) : ");
  wakeupValue = (getc()&0x7F) - '0'; 
  printf("%d\n", wakeupValue);
  kwakeup(wakeupValue);
}

int do_tswitch()
{
  printf("proc %d tswitch()\n", running->pid);
  tswitch();
  printf("proc %d resumes\n", running->pid);
}

int do_kfork()
{
  PROC *p;
  printf("proc %d kfork a child\n", running->pid);
  p = kfork();
  if (p==0)
    printf("kfork failed\n");
  else
    printf("child pid = %d\n", p->pid);
}

int do_exit()
{
  int exitValue;
  if (running->pid == 1 && nproc > 2){
      printf("other procs still exist, P1 can't die yet !%c\n",007);
      return -1;
  }
  printf("enter an exitValue (0-9) : ");
  exitValue = (getc()&0x7F) - '0'; 
  printf("%d\n", exitValue);
  kexit(exitValue);
}

int do_wait()
{
  int child, status;
  child = kwait(&status);
  if (child<0){
    printf("proc %d wait error : no child\n", running->pid);
    return -1;
  }
  printf("proc %d found a ZOMBIE child %d exitValue=%d\n", 
	   running->pid, child, status);
  return child;
}

int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
    printList("freelist  ", freeList);
    printQueue("readyQueue", readyQueue);
    printSleepList("sleepList ", sleepList);
    printf("-----------------------------------------\n");

    printf("proc %d[%d] running: parent=%d\n",
	   running->pid, running->priority, running->ppid);

    printf("enter a char [s|f|q|w|t|c] : ");
    c = getc(); printf("%c\n", c);
    switch(c){
       case 's' : do_tswitch();   break;
       case 'f' : do_kfork();     break;
       case 'q' : do_exit();      break;  
       case 'w' : do_wait();      break;
       case 't' : do_sleep();     break;
       case 'c' : do_wakeup();    break;
    }
  }
}

