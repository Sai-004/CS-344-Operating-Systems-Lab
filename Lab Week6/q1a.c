#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

struct my_msgbuf
{
    long mtype;
    int mrow;
    int mcol;
};

int main()
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;
    FILE *fp;

    // obtaining key
    if ((key = ftok("ML2-input.txt", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    printf("key: %d\n", key);

    // creating a message queue
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    // reading the file
    fp = fopen("ML2-input.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }

    // printf("Enter (row_number,column_number), ^D to quit:\n");
    buf.mtype = 1; //20x50 is limit.
    for (int i = 1; i <= 256; i++)
    {
        for (int j = 1; j <= 256; j++)
        {
            buf.mrow = i;
            buf.mcol = j;
            printf("(%d,%d)\t", i, j);
            if (msgsnd(msqid, (struct msgbuf *)&buf, sizeof(buf), 0) == -1)
                perror("msgsnd");
        }
        printf("\trow %d done\n",i);
    }

    printf("\nDone\n");
    sleep(1);
    // remove message queue
    if (msgctl(msqid, IPC_RMID, NULL) == -1)
    {
        printf("cleared\n");
        perror("msgctl");
        exit(1);
    }

    fclose(fp);

    return 0;
}
