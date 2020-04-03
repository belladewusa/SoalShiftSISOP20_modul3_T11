#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#define UNREADY -1
#define READY 0
#define TAKEN 1
#define MAX 100

struct shared{
    int status;
    int data[100];
};


int matA[4][2];
int matB[2][5];
int matC[4][5];

int baris = 0;
int row = 0;

void* kali(void* arg) {
  if(row >= 5){  /// <-- Ajg
    row = 0;
    baris++;
  }

  for(int i = 0; i < 2;i++){
    matC[baris][row] += matA[baris][i] * matB[i][row];
  }
  row++;
}


int main() {
  srand(time(NULL));
  printf("A = \n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      matA[i][j] = rand()%19+1;
      printf("%3d", matA[i][j]);
    }
    printf("\n");
  }
  printf("B = \n");
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 5; j++) {
      matB[i][j] = rand()%19+1;
      printf("%3d", matB[i][j]);
    }
    printf("\n");
  }

  pthread_t tid[20];

  for (int i = 0; i < 20; i++) {
    pthread_create(&(tid[i]), NULL, &kali, NULL);
  }

  for (int i = 0; i < 20; i++) {
    pthread_join(tid[i], NULL);
  }


  printf("C = A * B :\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%4d", matC[i][j]);
    }
    printf("\n");
  }

   key_t          ShmKEY;
   int            ShmID;
   struct shared  *ShmPTR;

   ShmKEY = ftok("key",100);
   ShmID = shmget(ShmKEY,sizeof(struct shared),IPC_CREAT|0666);
   if(ShmID < 0){
     printf("ERROR DI SHMGET!\n");
     exit(1);
   }
   // Buat attach memory
   ShmPTR = (struct shared *) shmat(ShmID, NULL, 0);
   // if((int)ShmPTR == -1){  // Ini error knp ya gk tau hmm
   //   printf("ERROR DI SHMAT!\n");
   //   exit(1);
   // }

   ShmPTR->status = UNREADY;
   int j = 0;
   int k = 0;

   for(int i = 0; i < 20; i++){
     if(k >= 5){
       j++;
       k = 0;
     }
     ShmPTR->data[i] = matC[j][k];
     k++;
   }
   ShmPTR->status = READY;

    printf("Please start the client in another window...\n");
    while (ShmPTR->status != TAKEN)
        sleep(1);

   // for(int i = 0; i < 20;i++){ // Cman buat ngecheck array dah masuk atau blum
   //   printf("%d\n",ShmPTR->data[i]);
   // }

   printf("Server has detected the completion of its child...\n");
    shmdt((void *) ShmPTR);
    printf("Server has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);







    return 0;
}
