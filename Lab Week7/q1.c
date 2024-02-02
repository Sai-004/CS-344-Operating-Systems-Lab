#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 25
#define NUM_REQUESTS 100

void sem_wait(int sem_id, int sem_num) {
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

void sem_signal(int sem_id, int sem_num) {
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

int main() {
    key_t key1, key2;
    int shmid;
    int semid;
   
    key1 = ftok("q1.c", 'R');
    
    shmid = shmget(key1, SHM_SIZE, IPC_CREAT | 0644);
    
    key2 = ftok("q2.c", 'R');
    
    semid = semget(key2, 2, IPC_CREAT | 0644);
    
    semctl(semid, 0, SETVAL, 1); 
    semctl(semid, 1, SETVAL, 0); 
    
    for (int i = 0; i < 2; i++) {
        if (fork() == 0) {
            int *shm_ptr = shmat(shmid, NULL, 0);
            if (shm_ptr == (int *) -1) {
                perror("shmat");
                exit(1);
            }
            
            while (1) {
                sem_wait(semid, 0);
                
                int request = *shm_ptr;
                printf("Child %d handling HTTP request %d\n", i + 1, request);
                
                (*shm_ptr)++;
                
                sem_signal(semid, 1);
                
                if (request >= NUM_REQUESTS-1) {
                    shmdt(shm_ptr);
                    exit(1);
                }
            }
        }
    }
    
    int *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (int *) -1) {
        perror("shmat");
        exit(1);
    }
    
    /*for (int request = 0; request < 2; request++) {
        sem_wait(semid, 1);
        
        *shm_ptr = request;
        
        sem_signal(semid, 0);
    }*/
    
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }
    
    semctl(semid, 0, IPC_RMID, 0);
    
    shmdt(shm_ptr);
    
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}

