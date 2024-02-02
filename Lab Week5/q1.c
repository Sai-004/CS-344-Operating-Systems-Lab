#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 100000

int main()
{
    key_t key;
    int shmid;
    char *shm;
    char temp[SHM_SIZE];
    FILE *fp_in;

    // Obtain a key
    key = ftok("week05-ML2-input.txt", 'R');
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    // Create a shared memory object
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    // printf("prev_shared mem: %s\n",shm);

    // Read the input file
    fp_in = fopen("week05-ML2-input.txt", "r");
    if (fp_in == NULL)
    {
        perror("fopen");
        exit(1);
    }

    fgets(temp, sizeof(temp), fp_in);
    // Write commentary data into shared memory
    while (fgets(temp, sizeof(temp), fp_in) != NULL)
    {
        // printf("temp: %s", temp);
        shm = strcat(shm, temp);
    }
    printf("shared mem: %s", shm);

    // Detach shared memory object
    shmdt(shm);

    // Free the created shared memory object
    // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    //     perror("shmctl");
    //     exit(1);
    // }

    fclose(fp_in);

    return 0;
}
