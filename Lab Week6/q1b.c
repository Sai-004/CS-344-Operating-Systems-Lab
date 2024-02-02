#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>
#include <unistd.h>

struct msg {
    long mtype;
    int mrow;
    int mcol;
};

int calc(int row, int col, int K[3][3], int img[258][258]){
    int g;

    g = img[row-1][col-1]*K[0][0] + img[row-1][col]*K[0][1] + img[row-1][col+1]*K[0][2]
        + img[row][col-1]*K[1][0] + img[row][col]*K[1][1] + img[row][col+1]*K[1][2]
        + img[row+1][col-1]*K[2][0] + img[row+1][col]*K[2][1] + img[row+1][col+1]*K[2][2];

    return g;
}

int main() {
    FILE *f1 = fopen("ML2-input.txt", "r");

    int rows_Kx, cols_Kx;
    fscanf(f1, "%d", &rows_Kx);
    fscanf(f1, "%d", &cols_Kx);


    int Kx[rows_Kx][cols_Kx];

    for (int i = 0; i < rows_Kx; ++i) {
        for (int j = 0; j < cols_Kx; ++j) {
            fscanf(f1, "%d", &Kx[i][j]);
        }
    }

    int rows_Ky, cols_Ky;
    fscanf(f1, "%d", &rows_Ky);
    fscanf(f1, "%d", &cols_Ky);


    int Ky[rows_Ky][cols_Ky];

    for (int i = 0; i < rows_Ky; ++i) {
        for (int j = 0; j < cols_Ky; ++j) {
            fscanf(f1, "%d", &Ky[i][j]);
        }
    }

    int rows_I, cols_I;
    fscanf(f1, "%d", &rows_I);
    fscanf(f1, "%d", &cols_I);


    int img[rows_I][cols_I];

    for (int i = 0; i < rows_I; ++i) {
        for (int j = 0; j < cols_I; ++j) {
            fscanf(f1, "%d", &img[i][j]);
        }
    }

    key_t key = ftok("ML2-input.txt", 'B');

    int msqid = msgget(key, 0666);

    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg buf;

    buf.mtype = 1;
   

    FILE *f2 = fopen("Output.txt", "w");

    pid_t children ;
    int num_c = 0;

    double output[256][256];

    for(;;) { /* Ron never quits! */
        if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        // printf("Harry: (%d,%d)\n", buf.mrow, buf.mcol);

        if(num_c == 8){
            int status;
            wait(&status);
            num_c--;
        }

        children = fork();

        if(children < 0){
           fprintf(stderr, "Fork Failed");
            return -1;
        }

        else if(children == 0){
            int gx = calc(buf.mrow, buf.mcol, Kx, img);
            int gy = calc(buf.mrow, buf.mcol, Ky, img);
            gx *= gx;
            gy *= gy;
            double y = sqrt(gx + gy);
            
            fprintf(f2, "(%d, %d) : %f\n",buf.mrow, buf.mcol, y);


            exit(1);
        }

        else{
            num_c++;
            wait(NULL);
        }
    }




    return 0;
}