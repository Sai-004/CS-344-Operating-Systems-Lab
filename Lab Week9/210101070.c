#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>

union semun
{
    int val;              /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    short *array;         /* used for GETALL and SETALL */
};

void sem_wait(int semid, int sem_num)
{
    struct sembuf sb = {0, -1, 0};
    sb.sem_num = sem_num;
    sb.sem_op = -1;
    if (semop(semid, &sb, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

void sem_signal(int semid, int sem_num)
{
    struct sembuf sb = {0, -1, 0};
    sb.sem_num = sem_num;
    sb.sem_op = 1; /* free resource */
    if (semop(semid, &sb, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

void grab_forks(int semid, int philosopher_id, int round)
{
    if (philosopher_id % 2 == 0)
    {
        sem_wait(semid, (philosopher_id + 1 + round) % 5);
        sem_wait(semid, (philosopher_id + round) % 5);
    }
    else
    {
        sem_wait(semid, (philosopher_id + round) % 5);
        sem_wait(semid, (philosopher_id + 1 + round) % 5);
    }
}

void release_forks(int semid, int philosopher_id, int round)
{
    sem_signal(semid, (philosopher_id + round) % 5);
    sem_signal(semid, (philosopher_id + 1 + round) % 5);
}

struct Table
{
    char name[50];
    int pids[10];
};

int main()
{
    key_t key;
    // semaphores
    struct sembuf sb = {0, -1, 0};
    int semid;
    if ((key = ftok("q1.c", 'S')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((semid = semget(key, 5, IPC_CREAT | 0666)) == -1)
    {
        perror("semget");
        exit(1);
    }
    if (semctl(semid, 0, SETVAL, 1) < 0)
    {
        perror("semctl");
        exit(1);
    }
    if (semctl(semid, 1, SETVAL, 1) < 0)
    {
        perror("semctl");
        exit(1);
    }
    if (semctl(semid, 2, SETVAL, 1) < 0)
    {
        perror("semctl");
        exit(1);
    }
    if (semctl(semid, 3, SETVAL, 1) < 0)
    {
        perror("semctl");
        exit(1);
    }
    if (semctl(semid, 4, SETVAL, 1) < 0)
    {
        perror("semctl");
        exit(1);
    }

    // Shared memory
    key_t shm_key = ftok("q1.c", 'M');
    int shm_id = shmget(shm_key, sizeof(struct Table) * 5, IPC_CREAT | 0666);
    struct Table *tables = shmat(shm_id, NULL, 0);

    // Initialize each table
    for (int i = 0; i < 5; i++)
    {
        snprintf(tables[i].name, 50, "Table%d", i + 1);
        for (int j = 0; j < 10; j++)
        {
            tables[i].pids[j] = 0;
        }
    }

    // Create 5 child processes (philosophers)
    for (int i = 0; i < 5; i++)
    {
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            int philosopher_id = i;
            for (int round = 0; round < 5; round++)
            {
                sleep(2);
                grab_forks(semid, philosopher_id, round);
                printf("Transaction %d holding the semaphore\n", getpid());
                int left_fork = (philosopher_id + round) % 5;
                int right_fork = (philosopher_id + 1 + round) % 5;
                printf("Transaction %d: operating on Table %d - Table %d\n", getpid(), left_fork + 1, right_fork + 1);
                for (int j = 0; j < 10; j++)
                {
                    if (tables[left_fork].pids[j] == 0)
                    {
                        tables[left_fork].pids[j] = getpid();
                        break;
                    }
                }
                for (int j = 0; j < 10; j++)
                {
                    if (tables[right_fork].pids[j] == 0)
                    {
                        tables[right_fork].pids[j] = getpid();
                        break;
                    }
                }
                sleep(10); // Sleep for 10 seconds
                release_forks(semid, philosopher_id, round);
            }
            exit(0);
        }
    }
    
    // Wait for all child processes to complete
    for (int i = 0; i < 5; i++)
    {
        wait(NULL);
    }

    // Print the contents of every table
    for (int i = 0; i < 5; i++)
    {
        printf("Table %d:\n", i + 1);
        for (int j = 0; j < 10; j++)
        {
            printf("pid[%d] = %d\t", j, tables[i].pids[j]);
        }
        printf("\n");
    }

    return 0;
}