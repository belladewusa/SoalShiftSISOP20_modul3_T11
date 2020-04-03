#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CAPTURESTATE 4
#define UNREADY -1
#define READY 3
#define UPDATED 1
#define TAKEN 2

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

  int state;
  printf("HALO CUY LU MAU NGAPAIN???\n");
  printf("1. Cari Pokemon\n");
  printf("2. Buka Pokedex\n");
  printf("3. Shop\n");
  printf("Input anda : ");
  scanf("%d",&state);

  pid_t pid = fork();
  if(pid == 0){
    char *argv[] = {"clear", NULL};
    execv("/usr/bin/clear", argv);
  }
  while(wait(NULL) > 0);

  // Buat Share CurrentState
  key_t          ShmKEY;
  int            ShmID;
  struct currentState  *ShmPTR;

  ShmKEY = ftok("state",100);
  ShmID = shmget(ShmKEY,sizeof(struct currentState),IPC_CREAT|0666);
  if(ShmID < 0){
    printf("ERROR DI SHMGET!\n");
    exit(1);
  }

  ShmPTR = (struct currentState *) shmat(ShmID, NULL, 0);

  ShmPTR->status = UNREADY;
  ShmPTR->data = state;
  ShmPTR->status = READY;
  printf("\t\t\t\tMencari Pokemon");
  int x = 0;
  while (ShmPTR->status != UPDATED){
        if(x > 2){
        printf("\b\b\b");
          x = 0;
          sleep(0.5);
        }else{
          printf("*");
          x++;
        }
      }


 if(ShmPTR->status == CAPTURESTATE){
   pid_t pid = fork();
   if(pid == 0){
     char *argv[] = {"clear", NULL};
     execv("/usr/bin/clear", argv);
   }
   while(wait(NULL) > 0);

   // Nerima Data Pokemon

   key_t          ShmPokemonKEY;
   int            ShmPokemonID;
   struct Pokemon  *ShmPokemonPTR;

   ShmPokemonKEY = ftok("key",90);
   ShmPokemonID = shmget(ShmPokemonKEY,sizeof(struct Pokemon),0666);
   if(ShmPokemonID < 0){
     printf(" ** SHMERROR CLIENT POKEMON! ** \n");
     exit(1);
   }
   ShmPokemonPTR = (struct Pokemon*) shmat(ShmPokemonID, NULL, 0);

   printf("A Wild %d Has Appeared",ShmPokemonPTR->nama);


   printf("1. Serang\n");
   printf("2. Item\n");
   printf("3. Lari\n");
  }


 printf("Server has detected the completion of its child...\n");
 shmdt((void *) ShmPTR);
 printf("Server has detached its shared memory...\n");
 shmctl(ShmID, IPC_RMID, NULL);
 printf("Server has removed its shared memory...\n");
 printf("Server exits...\n");













  printf("%d",state);




  return 0;

}
