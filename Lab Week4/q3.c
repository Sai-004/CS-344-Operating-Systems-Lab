#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
 
int main() {
    char input[256];
 
    printf("Enter a command (or 'quit' to exit): ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    if (strcmp(input, "quit") == 0) {
        printf("Exiting program.\n");
    }
    else if(strcmp(input, "./compute_sequence_1 | ./compute_sequence_2") == 0)
    {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            close(fd[1]);

            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            execlp("./compute_sequence_2", "./compute_sequence_2", "sequence", "2", NULL);
            perror("Exec failed");
            exit(1);
        }
        else {
            close(fd[0]);

            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            execlp("./compute_sequence_1", "./compute_sequence_1", "sequence", "1", NULL);
            perror("Exec failed");
            exit(1);
        }
    }
    else
    {
        printf("Invalid Input");
    }
    return 0;
}