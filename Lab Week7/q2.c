#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_SIZE 25

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
    int semid;
    int shmid;
    int *shm_ptr;
    
    key1 = ftok("q2.c", 'R');

    semid = semget(key1, 2, IPC_CREAT | 0644);
    
    key2 = ftok("q1.c", 'R');
    
    shmid = shmget(key2, SHM_SIZE, 0644);
    
    shm_ptr = (int *)shmat(shmid, NULL, 0);
    if (shm_ptr == (int *) -1) {
        perror("shmat");
        exit(1);
    }
    
    for (int request = 0; request <= 101; request++) {
        sem_wait(semid, 1);
        
        *shm_ptr = request;
        
        sem_signal(semid, 0);
    }
    
    shmdt(shm_ptr);
    
    return 0;
}

