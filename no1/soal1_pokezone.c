#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<time.h>
#include <errno.h>

#define CAPTURESTATE 4
#define UNREADY -1
#define READY 3
#define UPDATED 1
#define TAKEN 2

int chance;
void* randomize(void* arg){
  srand(time(NULL));
    chance = rand()%100+1;
}


struct currentState{
    int status;
    int data;
};

struct Pokemon{
  int counterRate;
  int escapeRate;
  int captureRate;
  int pokeDollar;
  int nama;
};

int main(){


     key_t          ShmKEY;
     int            ShmID;
     struct currentState  *ShmPTR;

     ShmKEY = ftok("state",100);
     ShmID = shmget(ShmKEY,sizeof(struct currentState),0666);
     if(ShmID < 0){
       printf(" ** SHMERROR CLIENT! ** \n");
       exit(1);
     }
     ShmPTR = (struct currentState*) shmat(ShmID, NULL, 0); //ATTACH BERHASIL
     // if ((int) ShmPTR == -1) {
     //      printf("*** shmat error (client) ***\n");
     //      exit(1);
     // }

     while (ShmPTR->status != READY){ // INCASE SERVER BLUM CONNECT
            ;
          }

      //THREADING BUAT POKEMON LALU PASSING DATA POKEMON KE TRAIZONE DAN BUAH CURRENTSTATE KE CAPTURE STATE
      pthread_t tid[1];
      while(ShmPTR->status != CAPTURESTATE){

        key_t ShmPokemonKEY;
        int ShmPokemonID;
        struct Pokemon *ShmPokemonPTR;

        ShmPokemonKEY = ftok("key",90);
        ShmPokemonID = shmget(ShmPokemonKEY,sizeof(struct Pokemon),IPC_CREAT|0666);
        if(ShmPokemonID < 0){
          printf("ERROR DI SHMGET POKEMON!\n %d\n",errno);
          exit(1);
        }
        ShmPokemonPTR = (struct Pokemon *) shmat(ShmPokemonID, NULL, 0);

          pthread_create(&(tid[0]), NULL, &randomize, NULL);
          pthread_join(tid[0], NULL);
            if(chance <= 80){
              pthread_create(&(tid[0]), NULL, &randomize, NULL);
              pthread_join(tid[0], NULL);
              if(chance <= 20){
                    ShmPokemonPTR->nama = 1;
              }else if( chance <= 40){
                    ShmPokemonPTR->nama = 2;
              }else if(chance <= 60){
                    ShmPokemonPTR->nama = 3;
              }else if(chance <= 80){
                    ShmPokemonPTR->nama = 4;
              }else if(chance <= 100){
                    ShmPokemonPTR->nama = 5;
              }
              ShmPTR->status = CAPTURESTATE;
            }else if(chance <= 85){
              pthread_create(&(tid[0]), NULL, &randomize, NULL);
              pthread_join(tid[0], NULL);
            }else if(chance <= 100){
              pthread_create(&(tid[0]), NULL, &randomize, NULL);
              pthread_join(tid[0], NULL);
            }
          sleep(1);


      }




      //ShmPTR->data[0], ShmPTR->data[1],
      //ShmPTR->data[2], ShmPTR->data[3]);

      ShmPTR->status = UPDATED; // ARTINYA INFO KE 4a udah diambil datanya

      while(ShmPTR->status != TAKEN){
          ;
      }

      shmdt((void *) ShmPTR);
      //Dilepas datanya sama client

      printf("\n");


    return 0;
}
