/************* wait.c file ****************/

int ksleep(int event)
{
    // YOUR ksleep function
    printf("Sleeping for event %d\n", event);
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);
    tswitch();
}

int kwakeup(int event)
{   
    PROC *tmp = sleepList;
    PROC *found_proc;
    PROC *prev = 0;

    // YOUR kwakeup function
    printf("Waking up processes waiting for event %d\n", event);
    //Scan sleeplist for all the processes that were waiting for this event, get them out of the queue, change their status, put them in readyQueue
    //Scan sleepList for all the processes waiting for this event
        while(tmp) {
        if (tmp->event == event) {
            printf("Found the event\n");
            found_proc = tmp;
            tmp = tmp->next;
            //found the process, get it out of the queue
            found_proc = removeFromList(&sleepList, found_proc);
            found_proc->status = READY;
            found_proc->next = 0;
            enqueue(readyQueue, found_proc);
        }
        else {
            prev = tmp;
            tmp = tmp->next;
        }
    }
 
}

int kexit(int exitValue)
{
    // YOUR kexit function
    int i;
    PROC *p;
    int g_parent_id = running->ppid;
    for (i=1; i<NPROC; i++) {
        p = &proc[i];
        if (p->ppid == running->pid) {
            p->ppid = g_parent_id;
        }
    }
    running->exitCode = exitValue;
    running->status = ZOMBIE;
    kwakeup(g_parent_id);
    tswitch();
}

int kwait(int *status)
{
    // YOUR kwait function
    int i;
    int num_children = 0;
    int pid = running->pid;
    PROC *p;
    PROC *found_proc;
    for (i=1; i<NPROC; i++) {
        p = &proc[i];
        if (p->ppid == pid) {
            num_children++;
            if (p->status == ZOMBIE)  {
                //ZOMBIE child process found
                found_proc = removeFromList(&readyQueue);
                found_proc->status = FREE;
                printf("Process %d exited with value %d\n", found_proc->pid, found_proc->exitCode);
                put_proc(&freeList, found_proc);
            }
        }
    }
    if (num_children > 0) {
        ksleep(pid);
    }
}
