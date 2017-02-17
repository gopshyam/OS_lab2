// ucode.c file

char *cmd[]={"getpid", "ps", "chname", "kfork", "switch", "wait", "exit", 0};

#define LEN 64

int show_menu()
{
   printf("***************** Menu *******************\n");
   printf("*  ps  chname  kfork  switch  wait  exit *\n");
   printf("******************************************\n");
}

int find_cmd(char *name)
{
  // return command index
    switch(name[0]) {
        case 'g':
            return 0;
            break;
        case 'p':
            return 1;
            break;
        case 'c':
            return 2;
            break;
        case 'k':
            return 3;
            break;
        case 's':
            return 4;
            break;
        case 'w':
            return 5;
            break;
        case 'e':
            return 6;
            break;
        }
    return -1;
}

int kkfork()
{   
  int child, pid;
  pid = getpid();
  printf("proc %d enter kernel to kfork a child\n", pid); 
  child = kfork()->pid;
  printf("proc %d kforked a child %d\n", pid, child);
}    

int kswitch()
{
    tswitch();
}

int wait()
{
    do_wait();
    return 1;
    /*int child, exitValue;
    printf("proc %d enter Kernel to wait for a child to die\n", getpid());
    //child = syscall(5, &exitValue, 0);
    printf("proc %d back from wait, dead child=%d", getpid(), child);
    if (child>=0)
        printf("exitValue=%d", exitValue);
    printf("\n"); */
} 

int geti()
{
  // return an input integer
    return 1;
}

int exit()
{
   int exitValue;
   //printf("enter an exitValue: ");
   exitValue = geti();
   //printf("exitvalue=%d\n", exitValue);
   //printf("enter kernel to die with exitValue=%d\n", exitValue);
   _exit(exitValue);
}

int _exit(int exitValue)
{
    do_exit(exitValue);
  //return syscall(6,exitValue,0);
}

int invalid(char *name)
{
    printf("Invalid command : %s\n", name);
}
