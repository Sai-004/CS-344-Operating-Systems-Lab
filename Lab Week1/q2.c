#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
    // if(argc!=1)
    // {
    //     printf("n not specified\n");
    //     return 1;
    // }
    FILE *fp;
    fp =fopen("q2_output.txt","w");
    int n=atoi(argv[1]);
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
        fprintf(fp,"Child %d: ",getpid());
        while(n!=1)
        {
            fprintf(fp,"%d ",n);
            if(n%2==0)
                n=n/2;
            else
                n=3*n+1;
        }
    }
    else
    {
        wait(NULL);
        fprintf(fp,"\nFile Position Indicator: %ld\n",ftell(fp));
    }
    return 0;
}

