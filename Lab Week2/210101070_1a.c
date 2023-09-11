#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
    int n;
    pid_t pid;
    if (argc < 2)
    {
        //default case
        n = 100;
        pid =fork();
        if(pid<0)
        {
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        //child
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
            exit(0);
        }
        //parent
        else
        {
            wait(NULL);
            printf("\nChild Completed.\n");
        }
        return 0;
    }
    else{
        for (int i = 1; i < argc;i++)
        {
            n = atoi(argv[i]);
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
                exit(0);
            }
            else
            {
                wait(NULL);
                printf("\nChild Completed.\n\n");
            }
        }
    }
    return 0;
}

