#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<stdbool.h>
#define SIZE 9

int sudoku[SIZE][SIZE];
int checkarray[11];
typedef struct {
    int row;
    int col;
} parameters;

void *checkvalues(void *param) {
    parameters *data = (parameters *)param;
    int row=data->row;
    int column = data->col;
    if(column==-1&&row==-2)
    {
       bool correct=true;
       for(int j=0;j<9;j++)
       {
           int check[9]={0};
           correct=true;
           for(int i=0;i<9;i++)
           {
               check[sudoku[i][j]-1]++;
               if(check[sudoku[i][j]-1]>1)
               {
                    correct=false;
                    break;
               }
           }
           if(correct==false) 
           {
               break;
           }
       }
       if(correct==true)
       {
           checkarray[0]=1;
       }
       else checkarray[0]=0;
    }
    else if(row==-1&&column==-2)
    {
       bool correct=true;
       for(int i=0;i<9;i++)
       {
           int check[9]={0};
           correct=true;
           for(int j=0;j<9;j++)
           {
               check[sudoku[i][j]-1]++;
               if(check[sudoku[i][j]-1]>1)
               {
                    correct=false;
                    break;
               }
           }
           if(correct==false) 
           {
               break;
           }
       }
       if(correct==true)
       {
           checkarray[1]=1;
       }
       else checkarray[1]=0;
    }
    else if(row>=0&&column>=0)
    {
       bool correct=true;
       int check[9]={0};
       for(int i=row;i<=row+2;i++)
       {
          for(int j=column;j<=column+2;j++)
          {
              check[sudoku[i][j]-1]++;
              if(check[sudoku[i][j]-1]>1)
              {
                 correct=false;
                 break;
              }
          }
          if(correct==false) break;
       }
       if(correct==false)
       {
           checkarray[ row/3 * 3 + column/3 + 2 ] =0;
       }
       else checkarray[ row/3 * 3 + column/3 +2]=1;
    }
    pthread_exit(0);
}
int main()
{
    FILE* f;
    f=fopen("week10-ML2-input1.txt","r");
    for(int i=0;i<9;i++)
    {
       for(int j=0;j<9;j++)
       {
           fscanf(f,"%d",&sudoku[i][j]);
       }
    }
    fclose(f);
    pthread_t colThreads[1];
    pthread_t rowThreads[1];
    pthread_t subThreads[9];
    parameters forcolumn;
    forcolumn.row=-2;
    forcolumn.col=-1;
    pthread_create(&colThreads[0], NULL, checkvalues, &forcolumn);
    parameters forrow;
    forrow.row=-1;
    forrow.col=-2;
    pthread_create(&rowThreads[0], NULL, checkvalues, &forrow);
    parameters forsubgrid[9];
    for (int i = 0; i < SIZE; i++) {
        forsubgrid[i].row = (i / 3) * 3;
        forsubgrid[i].col = (i % 3) * 3;
        pthread_create(&subThreads[i], NULL, checkvalues, &forsubgrid[i]);
    }
    pthread_join(colThreads[0],NULL);
    pthread_join(rowThreads[0],NULL);
    for(int i=0;i<9;i++) pthread_join(subThreads[i],NULL);
    bool correct=true;
    for(int i=0;i<11;i++)
    {
       if(checkarray[i]==0)
       {
          correct=false;
          break;
       }
   }
   if(correct==false) printf("The sudoku is not Valid\n");
   else printf("The sudoku is Valid\n");
    return 0;
}
    
