extern PROC *readyQueue, *freeList;

/****************** utility.c file ***********************/

int put_proc(PROC **list, PROC *p)   // return a PROC to freeList
{
    PROC *tmp = *list;
    //If list is empty
    if (!tmp) {
        tmp = p;
        p->next = 0;
        *list = tmp;
        return 1;
    }

    while(tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = p;
    p->next = 0;
    return 1;
}

// enter a PROC into queue by priority
int enqueue(PROC **queue, PROC *p)
{
    PROC *tmp = *queue;
    int new_proc_priority;
    //If the queue is empty
    if (!tmp) {
        tmp = p;
        p->next = 0;
        *queue = tmp;
        return 1;
    }
    
    //Queue goes from higher to lower
    //Find the last process with a higher priority
    new_proc_priority = p->priority;
    while(tmp->next->priority >= new_proc_priority) {
        tmp = tmp->next;
    }
    p->next = tmp->next;
    tmp->next = p;
}

// remove and return first PROC in queue 
PROC *dequeue(PROC **queue)
{
    PROC *first = *queue;
    PROC *tmp = first;
    if (!first) {
        return 0;
    }
    first = first->next;
    *queue = first;
    tmp->next = 0;
    return tmp;
}

PROC *get_proc(PROC **list)  // allocate a FREE proc from freeList
{
    PROC *p = dequeue(list);
    return p;
}


// print queue and print list functions 
int printList(char *name, PROC *p)
{
    int pfret = printf("%s", name);
    PROC *tmp = p; 
    while(tmp) {
        printf("%d ->", tmp->pid);
        tmp = tmp->next;
    }
    printf("NULL\n");
    return 1;
}

int printQueue(char *name, PROC *p)
{
    int pfret = printf("%s  ", name);
    PROC *tmp = p; 
    while(tmp) {
        printf("%d[ %d] ->", tmp->pid, tmp->ppid);
        tmp = tmp->next;
    }
    printf("NULL\n");
    return 1;

}

int printSleepList(char *name, PROC *p)
{
    int pfret = printf("%s  ", name);
    PROC *tmp = p; 
    while(tmp) {
        printf("%d[ %d] ->", tmp->pid, tmp->event);
        tmp = tmp->next;
    }
    printf("NULL\n");
    return 1;

}

PROC *removeFromList(PROC **list, PROC *p)
{
    PROC *found_proc;
    PROC *tmp = *list;
    PROC *prev = 0;
    while(tmp != p && tmp) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp == 0) {
        return 0;
    }
    
    //If the first element has to be returned, the start of the list must also be moved to match this
    if (prev == 0) {
        found_proc = tmp;
        tmp = tmp->next;
        *list = tmp;
        found_proc->next = 0;
        return found_proc;
    }
    //Else, the normal case
    found_proc = tmp;
    tmp = tmp->next;
    prev->next = tmp;
    found_proc->next = 0;
    return found_proc;
}
