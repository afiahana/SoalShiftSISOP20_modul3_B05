#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<dirent.h>

pthread_t tid[5000];
char *curDir;
char *tempDir;

void moveFileUtil(char source[], char dest[]);
char* getFilename(char str[]);
char* getExt(char str[]);
void* move(void *arg);
void* moveAll(void *arg);
void* moveChoosen(void *arg);

void moveFileUtil(char source[], char dest[]){
    int ch;
    FILE *fp1, *fp2;

    fp1 = fopen(source, "r");
    fp2 = fopen(dest, "w");
    
    if (!fp1) {
        printf("Unable to open source file to read!!\n");
        fclose(fp2);
        return ;
    }

    if(!fp2) {
        printf("Unable to open target file to write\n");
        return ;
    }

    while((ch = fgetc(fp1)) != EOF) {
        fputc(ch, fp2);
    }

    fclose(fp1);
    fclose(fp2);
    remove(source);
    return ;
}

char* getFilename(char str[]){
    char* pch;
    char* result;
    pch = strchr(str,'/');
    if(pch == NULL)
        return str;

    while(pch != NULL) {
        result = pch+1;
        pch = strchr(pch+1,'/');
    }
    return result;
}

char* getExt(char str[]){
    char* pch = getFilename(str);
    char* result = strchr(pch, '.');
    if(result == NULL) return NULL;
    else return (result+1);
}

void* move(void *arg){
    pthread_t id = pthread_self();
    char *extFolder;
    char destFolder[5000];

    extFolder = getExt((char *)arg);

    if(extFolder == NULL){
        strcpy(destFolder, "Unknown");
    }
    else{
        strcpy(destFolder, extFolder);
        int i;
        for(i = 0; i < strlen(destFolder); i++){
            if(destFolder[i] > 64 && destFolder[i] < 91)
                destFolder[i] += 32;
        }
    }
    
    if(mkdir(destFolder, 0777) == -1);

    char destPath[5000];
    snprintf(destPath, 5000, "%s/%s/%s", curDir, destFolder, getFilename((char *)arg));
    moveFileUtil((char *)arg, destPath);
}

void* moveAll(void *arg){
    pthread_t id = pthread_self();
    char *extFolder;
    char destFolder[5000];

    extFolder = getExt((char *)arg);

    if(extFolder == NULL){
        strcpy(destFolder, "Unknown");
    }
    else{
        strcpy(destFolder, extFolder);
        int i;
        for(i = 0; i < strlen(destFolder); i++){
            if(destFolder[i] > 64 && destFolder[i] < 91)
                destFolder[i] += 32;
        }
    }

    if(mkdir(destFolder, 0777) == -1);

    char destPath[5000];
    char sourcePath[5000];
    snprintf(sourcePath, 5000, "%s/%s", curDir, (char *)arg);
    snprintf(destPath, 5000, "%s/%s/%s", curDir, destFolder, getFilename((char *)arg));
    moveFileUtil(sourcePath, destPath);
}

void* moveChoosen(void *arg){
    pthread_t id = pthread_self();
    char *extFolder;
    char destFolder[5000];

    extFolder = getExt((char *)arg);

    if(extFolder == NULL)
        strcpy(destFolder, "Unknown");
    else{
        strcpy(destFolder, extFolder);
        int i;
        for(i=0; i<strlen(destFolder); i++)
            if(destFolder[i] > 64 && destFolder[i] < 91)
                destFolder[i] += 32;
    }
            
    if(mkdir(destFolder, 0777) == -1);

    char destPath[5000];
    char sourcePath[5000];
    snprintf(sourcePath, 5000, "%s/%s", tempDir, (char *)arg);
    snprintf(destPath, 5000, "%s/%s/%s", curDir, destFolder, getFilename((char *)arg));
    moveFileUtil(sourcePath, destPath);
}

int main(int argc, char **argv){
    char buff[1000];
    curDir = getcwd(buff, 1000);
    int i, j;
    i = 2;

    if(strcmp(argv[1], "-f") == 0){
        while(argv[i] != NULL){
            pthread_create(&(tid[i-2]), NULL, &move, (void *)argv[i]);
            i++;
        }
        for(j = 0; j < (i-1); j++){
            pthread_join(tid[j], NULL);
        }
    }
    else if(strcmp(argv[1], "*") == 0){
        DIR *d;
        struct dirent *dir;
        struct stat myFile;
        d = opendir(".");
        if(d){
            while((dir = readdir(d)) != NULL){
                if (stat(dir->d_name, &myFile) < 0);
                else if (!S_ISDIR(myFile.st_mode)){
                    pthread_create(&(tid[i-2]), NULL, &moveAll, (void *)dir->d_name);
                    i++;
                } else;
            }
            for(j = 0; j < (i-1); j++)
                pthread_join(tid[j],NULL);
        }
    }
    else if(strcmp(argv[1], "-d") == 0){
        chdir(argv[2]);
        tempDir = getcwd(buff, 1000);
        DIR *d;
        struct dirent *dir;
        struct stat myFile;
        d = opendir(".");
        if(d){
            while((dir = readdir(d)) != NULL){
                if (stat(dir->d_name, &myFile) < 0);
                else if (!S_ISDIR(myFile.st_mode)){
                    pthread_create(&(tid[i-2]), NULL, &moveChoosen, (void *)dir->d_name);
                    i++;
                } else;
            }
            for(j = 0; j < (i-1); j++)
                pthread_join(tid[j],NULL);
        }
    }
}