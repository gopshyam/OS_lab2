/****************** kernel.c file *************************/
int body();

PROC *kfork(char *filename)
{
      PROC *p = get_proc(&freeList);
      int i, p_seg;
      if (p==0){
         printf("no more PROC, kfork() failed\n");
         return 0;
      }
      p_seg = (p->pid + 1) * 1000;
      printf("PROC %d forked a child at segment %d\n", p->pid, p_seg);
      p->status = READY;
      p->priority = 1;          // priority = 1 for all proc except P0
      p->ppid = running->pid;   // parent = running
      p->parent = running;
      
      if (filename) {
                    printf("loading %s to segment %d .... done\n", filename, p_seg);
      }

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
  printf("proc %d resumes in segment %d\n", running->pid, (running->pid + 1) * 1000);
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

int getpid()
{
   return running->pid;
}

int chname()
{
    char s[32];
    printf("input new name : ");
    gets(s);
    kchname(s);
}

int ps()
{
    int i;
    int s;
    char status[32];
    printf("=======================================================\n");
    printf("name \t status \t pid \t ppid\n");
    printf("-------------------------------------------------------\n");
    
    for (i = 0; i < NPROC; i++) {
        s = proc[i].status;
        switch(s) {
            case FREE:
                strcpy(status, "FREE");
                break;
            case READY:
                strcpy(status, "READY");
                break;
            case RUNNING:
                strcpy(status, "RUNNING");
                break;
            case STOPPED:
                strcpy(status, "STOPPED");
                break;
            case SLEEP:
                strcpy(status, "SLEEP");
                break;
            case ZOMBIE:
                strcpy(status, "ZOMBIE");
                break;
        }
        printf("%s \t %s \t %d \t %d\n", proc[i].name, status, proc[i].pid, proc[i].ppid);
    }
}

int kchname(char *name)
{
    strcpy(running->name, name);
    return 1;
}

int getcs() {
    int id, cs;
    id = running->pid;
    cs = (id + 1) * 0x1000;
    return cs;
}

int body()
{
  char c;

  if (running->inUmode) {
        goUmode();
    }

  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
    printList("freelist  ", freeList);
    printQueue("readyQueue", readyQueue);
    printSleepList("sleepList ", sleepList);
    printf("-----------------------------------------\n");

    printf("proc %d[%d] running: parent=%d\n",
	   running->pid, running->priority, running->ppid);

    printf("enter a char [s|f|q|w|t|c|u] : ");
    c = getc(); printf("%c\n", c);
    switch(c){
       case 's' : do_tswitch();   break;
       case 'f' : do_kfork();     break;
       case 'q' : do_exit();      break;  
       case 'w' : do_wait();      break;
       case 't' : do_sleep();     break;
       case 'c' : do_wakeup();    break;
       case 'u' : goUmode();      break;
    }
  }
}

