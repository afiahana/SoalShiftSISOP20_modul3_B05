#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
int *value;
int matBaru[20], i = 0;

void* factorial(void* arg){
    int f = 0, j, v;
    v = *(value+i);
    for(j = 1; j <= v; j++){
        f += j;
    }
    matBaru[i] = f;
    i++;
}

int main(){
    key_t key = 1234;
    int j;

    int shmid = shmget(key, 20*sizeof(int), IPC_EXCL);
    value = (int *)shmat(shmid, NULL, 0);

    for(j = 0; j < 20; j++){
        if(j == 4 || j == 9 || j == 14 || j == 19) printf("%5d\n", *(value+j));
        else printf("%5d", *(value+j));
    }
    printf("\n");

    pthread_t tid[20];

    for(j = 0; j < 20; j++){
        pthread_create(&tid[j], NULL, &factorial, NULL);
    }

    for(j = 0; j < 20; j++){
        pthread_join(tid[j], NULL);
    }

    for(j = 0; j < 20; j++){
        if(j == 4 || j == 9 || j == 14 || j == 19) printf("%5d\n", matBaru[j]);
        else printf("%5d", matBaru[j]);
    }

    shmdt((void *) value);
}
