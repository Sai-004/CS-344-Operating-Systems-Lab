#include <stdio.h>
#include<sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int n;
    for(int i=0;i<3;i++)
    {
    pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(pid==0)
    {
        if(i==0)
        {
            char *env_vars[] = {};
            execl("./210101070-q1b","./210101070-q1b",NULL);
        }
        else if(i==1)
        {
           
            char *env_vars[] ={"n=5",NULL};
            execle("./210101070-q1b","10",NULL,env_vars);
        }
        else if(i==2)
        {
           
            char *args[] = {NULL};
            char *env_vars[] = {"n=19 20 30 40 50 60 70 80 90 100",NULL};
            execve("./210101070-q1b", args, env_vars);
        }
    }
    wait(NULL);
    }
    return 1;
}

