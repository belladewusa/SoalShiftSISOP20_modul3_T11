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

typedef long long ll;

struct shared{
    int status;
    int data[100];
};
int baris = 0;;
int row = 0;
void* sumOf(void* arg) {
  int i = *((int*)arg);
  free(arg);;
  int total = 0;
  for(int j = 0; j <= i ;j++){
    total += j;
  }
  if(row > 4){
    printf("\n");
    row = 0;
  }
  printf("%d %10d ",i,total);
  row ++;
}


int main()
{
     key_t          ShmKEY;
     int            ShmID;
     struct shared  *ShmPTR;

     ShmKEY = ftok("key",100);
     ShmID = shmget(ShmKEY,sizeof(struct shared),0666);
     if(ShmID < 0){
       printf(" ** SHMERROR CLIENT! ** \n");
       exit(1);
     }
     ShmPTR = (struct shared*) shmat(ShmID, NULL, 0); //ATTACH BERHASIL
     // if ((int) ShmPTR == -1) {
     //      printf("*** shmat error (client) ***\n");
     //      exit(1);
     // }

     while (ShmPTR->status != READY) // INCASE SERVER BLUM CONNECT
        ;

      //DATA DAH SIAP
      pthread_t tid[20];
      for(int i = 0; i < 20;i++){
         int *x =  malloc(sizeof(*x));
         if( x == NULL){
           printf("ERROR CANT MAKE NEW SPACE\n");
           exit(1);
         }
         *x = ShmPTR->data[i];
         pthread_create(&(tid[i]), NULL, &sumOf, x);
        pthread_join(tid[i], NULL);

      }
      //ShmPTR->data[0], ShmPTR->data[1],
      //ShmPTR->data[2], ShmPTR->data[3]);

      ShmPTR->status = TAKEN; // ARTINYA INFO KE 4a udah diambil datanya
      shmdt((void *) ShmPTR);
      //Dilepas datanya sama client

      printf("\n");


    return 0;
}
