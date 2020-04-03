#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int matA[4][2];
int matB[2][5];
int matC[4][5];
int step = 0;
int *value;

void* kali(void* arg) {
  int i, j, k, tmp;
  int idx = -1;

    for(i = 0; i < 4; i++){
        for(j = 0; j < 5; j++){
            idx += 1;
            for(k = 0; k < 2; k++){
                matC[i][j] = matC[i][j] + (matA[i][k] * matB[k][j]);
                value[idx] = matC[i][j];
            }
        }
    }
}

int main() {
    key_t key = 1234;

    int shmid = shmget(key, 20*sizeof(int), IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);

  srand(time(NULL));
  printf("A = \n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      matA[i][j] = rand()%3;
      printf("%5d", matA[i][j]);
    }
    printf("\n");
  }
  printf("B = \n");
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 5; j++) {
      matB[i][j] = rand()%3;
      printf("%5d", matB[i][j]);
    }
    printf("\n");
  }

  pthread_t t;
  pthread_create(&t, NULL, &kali, NULL);
  pthread_join(t, NULL);

  printf("C = A*B :\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%5d", matC[i][j]);
    }
    printf("\n");
  }
  
    shmdt((void *) value);
    //shmctl(shmid, IPC_RMID, NULL);
}
