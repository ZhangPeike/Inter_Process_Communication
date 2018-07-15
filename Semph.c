#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
//#include "semun.h"
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};
static int set_sem_val();
static void del_sem_val();
static int semaphore_p();
static int semaphore_v();
static int sem_id;
int main(int argc, char* argv) {
  int pause = 0;
  char word = '0';
  srand((unsigned) getpid());
  sem_id = semget((key_t)111, 1, 0666 | IPC_CREAT);
  if(argc > 1) {
  if(!set_sem_val()) {
    printf("set semaphor 1 error\n");
    exit(EXIT_FAILURE);
  }
  word = '1';
  sleep(3);
  }
  for(int i = 10; i > 0; i--) {
    if(!semaphore_p()) {
      fprintf(stderr,"P operation error\n");
      exit(EXIT_FAILURE);
    }
    printf("%c", word); 
    fflush(stdout);
    pause = rand() % 3;
    sleep(pause);
    printf("%c", word); 
    fflush(stdout);
    if(!semaphore_v()) {
      fprintf(stderr, "V operation error\n");
      exit(EXIT_FAILURE);      
    }
    pause = rand() % 5;
    sleep(pause);    
  } 
  printf("\n %d finished\n", getpid());
  if(argc > 1) {
    sleep(10);
    del_sem_val();
  }
  exit(0); 
}
static int set_sem_val() {
  union semun sem_union;
  sem_union.val = 1;
  if(semctl(sem_id, 0, SETVAL, sem_union) == -1) return 0;
  return 1;
}
static void del_sem_val() {
  union semun sem_union;
  if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1) printf("error del semaphor!\n");
}
static int semaphore_p() {
  struct sembuf sem_buf;
  sem_buf.sem_num = 0;
  sem_buf.sem_op = -1;
  sem_buf.sem_flg = SEM_UNDO;
  if(semop(sem_id, &sem_buf, 1) == -1) return 0;
    return 1;
}
static int semaphore_v() {
  struct sembuf sem_buf;
  sem_buf.sem_num = 0;
  sem_buf.sem_op = 1;
  sem_buf.sem_flg = SEM_UNDO;
  if(semop(sem_id, &sem_buf, 1) == -1) {
    return 0;
  }
  return 1;
}
