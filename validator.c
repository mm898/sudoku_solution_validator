/*
* This is a multiethreaded suduko validator program created using pthreads.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define Threads_Num 27 //number of threads in the program
int sud[9][9]; //glopal 2D array to store the solution
int valid[Threads_Num]={0}; //this array will be used by threads to store if row/col/3x3 is valid

/* structure for passing data to threads*/
typedef struct{
  int row;
  int column;
} parameters;

// method to validate that each column has numbers 1-9                                              
void *checkRow(void *parm){
  parameters *parms = (parameters*) parm;
  int r = (*parms).row;
  int c = (*parms).column;
  int Crow[9] = {0};

  //check if row contains all number from 1-9, if not then set the Crow array to be 0 
  //in order to check for all numbers. then set the valid array to 1 if row is valid.
  for(int i=0; i<9 ; i++){
    int num = sud[r][i];
    if((num<1) || (num>9) || (Crow[num - 1] == 1)){
      pthread_exit(NULL);
    }
    else{
      Crow[num-1] = 1;
    }
  }
  //set the valid array starting from 10-18
  valid[9 + r] = 1;
  pthread_exit(NULL);
}

// method to validate that each square has numbers 1-9                                              
void *checkSquares(void* param) {
  parameters *parms = (parameters*) param;
  int r = (*parms).row;
  int c = (*parms).column;
  int Csqua[9] = {0};
  
  //check if squares contain all number from 1-9, if not then set the csqua array to be 0 
  //in order to check for all numbers. then set the valid array to 1 if row is valid.
  for (int i=r; i < r + 3; i++) {
    for (int j=c; j < c + 3; j++) {
      int num = sud[i][j];
      if (num < 1 || num > 9 || Csqua[num - 1] == 1) {
        pthread_exit(NULL);
      } 
      else {
        Csqua[num - 1] = 1;
      }
    }
  }
  //set the valid array starting from 1-9
  valid[r+c/3] = 1; 
  pthread_exit(NULL);
}


// method to validate that each column has numbers 1-9
void *checkColumn(void *parm){
  parameters *parms = (parameters*) parm;
  int r = (*parms).row;
  int c = (*parms).column;
  int Ccol[9] = {0};

  //check if column contains all number from 1-9, if not then set the Ccol array to be 0 
  //in order to check for all numbers. then set the valid array to 1 if col is valid.
  for(int i=0; i<9 ; i++){
    int num = sud[i][c];
    if((num<1) || (num>9) || (Ccol[num - 1] == 1)){
      pthread_exit(NULL);
    }
    else{
      Ccol[18+c] = 1;
    }
  }
  //set the valid array starting from 19-27
  valid[18 + c] = 1;
  pthread_exit(NULL);
}


int main(){
   int index=0;
  pthread_t threads[Threads_Num];

  //read from user and enter the values to the sud array
  printf("Please enter a suduko solution:\n");
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      scanf("%d", &sud[i][j]);
    }
  }

  //print suduko after reading
  printf("The given solution is:\n");
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      printf("%d ", sud[i][j]);
    }
    printf("\n");
  }

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      //threads to check 3X3 squares
      if (i%3 == 0 && j%3 == 0) {
        parameters *square = (parameters*) malloc(sizeof(parameters));
        (*square).row = i;
        (*square).column = j;
        pthread_create(&threads[index++], NULL, checkSquares, square); 
      }
      //threads to check coloumn 
      if(i==0){
        parameters *col = (parameters *) malloc(sizeof(parameters));
        (*col).row = i;
        (*col).column = j;
        pthread_create(&threads[index++], NULL, checkColumn, col);
      }
      //threads to check row 
      if(j==0){
      	parameters *row = (parameters *) malloc(sizeof(parameters));
      	(*row).row = i;
        (*row).column = j;
      	pthread_create(&threads[index++], NULL, checkRow, row);
      }
    }
  }

  //waiting for all threads to finish
  for (int i = 0; i < Threads_Num; i++) {
    pthread_join(threads[i], NULL);
  }

  //check if any value of the valid array is set to 0 that means invalid solution
  for (int i = 0; i < Threads_Num; i++) {
    if (valid[i] == 0) {
      printf("Invalid Solution\n");
      return EXIT_SUCCESS;
    }
  }

  printf("Valid Solution\n");
  return 0;
}
