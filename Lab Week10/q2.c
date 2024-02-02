#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<stdbool.h>
#define SIZE 9

int sudoku[SIZE][SIZE];
int checkarray[19];
typedef struct {
    int row;
    int col;
} parameters;

void *checkvalues(void *param) {
    parameters *data = (parameters *)param;
    int row=data->row;
    int column = data->col;
    if(column>=0&&row==-1)
    {
       bool correct=true;
           int check[9]={0};
           correct=true;
           for(int i=0;i<9;i++)
           {
               check[sudoku[i][column]-1]++;
               if(check[sudoku[i][column]-1]>1)
               {
                    correct=false;
                    break;
               }
           }
       if(correct==true)
       {
           checkarray[9+column]=1;
       }
       else checkarray[9+column]=0;
    }
    else if(row>=0&&column==-1)
    {
       bool correct=true;
           int check[9]={0};
           correct=true;
           for(int j=0;j<9;j++)
           {
               check[sudoku[row][j]-1]++;
               if(check[sudoku[row][j]-1]>1)
               {
                    correct=false;
                    break;
               }
           }
       if(correct==true)
       {
           checkarray[row]=1;
       }
       else checkarray[row]=0;
    }
    else if(row>=0&&column>=0)
    {
       bool correct=true;
       for(int k=0;k<9;k++)
       {
           int row1=(k/3)*3;
           int column1=(k%3)*3;
           int check[9]={0};
           for(int i=row1;i<=row1+2;i++)
           {
              for(int j=column1;j<=column1+2;j++)
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
           if(correct==false) break;
        }
       if(correct==false)
       {
           checkarray[ 18 ] =0;
       }
       else checkarray[ 18 ]=1;
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
    pthread_t colThreads[9];
    pthread_t rowThreads[9];
    pthread_t subThreads[1];
    parameters forcolumn[9];
    for(int i=0;i<9;i++)
    {
       forcolumn[i].row=-1;
       forcolumn[i].col=i;
       pthread_create(&colThreads[i], NULL, checkvalues, &forcolumn[i]);
    }
    parameters forrow[9];
    for(int i=0;i<9;i++)
    {
       forrow[i].row=i;
       forrow[i].col=-1;
       pthread_create(&rowThreads[i], NULL, checkvalues, &forrow[i]);
    }
    parameters forsubgrid[1];
        forsubgrid[0].row = 0;
        forsubgrid[0].col = 0;
        pthread_create(&subThreads[0], NULL, checkvalues, &forsubgrid[0]);
        for(int i=0;i<9;i++)
        {
          pthread_join(colThreads[i],NULL);
          pthread_join(rowThreads[i],NULL);
        }
    pthread_join(subThreads[0],NULL);
    bool correct=true;
    for(int i=0;i<19;i++)
    {
       if(checkarray[i]==0)
       {
          correct=false;
          break;
       }
   }
   //for(int i=0;i<19;i++) printf("%d\n",checkarray[i]);
   if(correct==false) printf("The sudoku is not Valid\n");
   else printf("The sudoku is Valid\n");
    return 0;
}
