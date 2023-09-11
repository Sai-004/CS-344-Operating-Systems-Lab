#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
    // int n=atoi(argv[1]);
    pid_t pid;
    for (int i = 0; i < 3;i++)
    {
        pid = fork();
        if(pid<0)
        {
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        else if(pid ==0)
        {
            char *env_vars[] = {NULL};
            printf("\nChild %d.\n", i + 1);
            if (i == 0)
            {
                execle("./210101070-q1a", "./210101070-q1a", NULL, env_vars);
            }
            else if(i==1)
            {
                execl("./210101070-q1a", "./210101070-q1a", "10", NULL);
            }
            else if(i==2)
            {
                char *argvs[] = {"./210101070-q1a", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100", NULL};
                execve("./210101070-q1a", argvs, env_vars);
            }
        }
        else
        {
            wait(NULL);
            // printf("\nChild Completed.\n");
        }
    }
    return 0;
}