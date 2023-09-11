#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int forking(pid_t pid,FILE *fp,int n,int i)
{
    pid =fork();
    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(pid ==0)
    {
        printf("child %d started, n=%d\n",i+1,n);
        fprintf(fp,"Child %d: ",getpid());
        while(n!=1)
        {
            fprintf(fp,"%d ",n);
            if(n%2==0)
                n=n/2;
            else
                n=3*n+1;
        }
        fprintf(fp,"\n");
        exit(0);
    }
    return 0;
}

int main(int argc,char* argv[])
{
    // if(argc!=1)
    // {
    //     printf("n not specified\n");
    //     return 1;
    // }
    FILE *fp;
    fp =fopen("q3_output.txt","a");
    int n=atoi(argv[1]);
    pid_t pid;int i;
    for(i=0;i<50;i++)
    {
        forking(pid,fp,(i+1)*n,i);
        wait(NULL);
    }
    printf("Children Completed. File Position Indicator: %ld\n",ftell(fp));    

    return 0;
}

