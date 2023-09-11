#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

int main(int argc,char* argv[])
{
    char *env_n = getenv("n");
    int n;
    if (env_n==NULL)
    {
        //default case
        n = 100;
        pid_t pid;
        pid =fork();
        if(pid<0)
        {
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        else if(pid ==0)
        {
            printf("child started, n=%d\n",n);
            printf("Sequence of numbers: ");
            while(n!=1)
            {
                printf("%d ",n);
                if(n%2==0)
                    n=n/2;
                else
                    n=3*n+1;
            }
            exit(1);
        }
        else
        {
            wait(NULL);
            printf("\nChild Completed.\n");
        }
    }
    else{
        
        char *key = strtok(env_n," ");
        while (key!=NULL)
        {
            // n = atoi(argv[i]);
            n = atoi(key);
            pid_t pid;
            pid = fork();
            if (pid < 0)
            {
                fprintf(stderr,"Fork Failed");
                return 1;
            }
            else if(pid ==0)
            {
                printf("child started, n=%d\n",n);
                printf("Sequence of numbers: ");
                while(n!=1)
                {
                    printf("%d ",n);
                    if(n%2==0)
                        n=n/2;
                    else
                        n=3*n+1;
                }
                exit(1);
            }
            else
            {
                wait(NULL);
                printf("\nChild Completed.\n\n");
            }
            key = strtok(NULL," ");
        }
    }
    return 0;
}

