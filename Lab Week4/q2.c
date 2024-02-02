#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>

void sequence(int n)
{
    // printf("child started, n=%d\n",n);
    printf("Sequence for %d: ",n);
    while(n!=1)
    {
        if(n%2==0)
            n=n/2;
        else
            n=3*n+1;
        printf("%d ",n);
    }
    printf("\n\n");
    return;
}

int main()
{
    int n;
    pid_t pid;
    while(1)
    {
        printf("Enter the number: ");
        scanf("%d", &n);
        if(n==1)
        {
            printf("\nTerminating the program...\n");
            exit(0);
        }
        printf("\n");
        sequence(n);
        wait(NULL);
    }
}