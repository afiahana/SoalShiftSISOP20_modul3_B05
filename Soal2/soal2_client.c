#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

// untuk thread
pthread_t tid[3];
// untuk cd process
pid_t cd;
// set socket dan valread
    int sock = 0, valread;
// variabel registrasi
    char regist[512];
//variabel akun
    char akun[60];
//variabel sandi
    char sandi[60];

void* screen1(void *arg)
//inisiasi argumen
{char *argv[] = {"clear",NULL};
//perbandingan antar theread
    pthread_t id = pthread_self();
//formulasi if statement
    if(pthread_equal(id,tid[0])){
//membuat fork untuk proses cdnya
        cd = fork();
//jika cdnya 0
        if(cd == 0)
//jalankan execnya
        {execv("/usr/bin/clear",argv);}}
//kondisi lainnya
    else if(pthread_equal(id,tid[1]))
    {   char masukan[25];
        scanf("%s",masukan);
//membandingkan login dengan masukan
        if(strcmp("login",masukan)==0){
	    //kirim data ke server
            send(sock, masukan, strlen(masukan), 0);
	    //tampilkan halaman login
            printf("[> LOGIN PAGE <]\n");
	    //tampilkan masukan u-sername
            printf("Username: ");
	    //input akun
            scanf("%s",akun);
	    //kirim data ke server
            send(sock, akun, strlen(akun), 0);
	    // tampilkan masukkan kata sandi
            printf("Password: ");
	    //input kata sandi
            scanf("%s",sandi);
	    //kirim data ke server
            send(sock, sandi, strlen(sandi), 0);}
// untuk kondisi registrasi
        else if(strcmp("register",masukan)==0)
	    //kirim data ke server
        {   send(sock, masukan, strlen(masukan), 0);
	    //masuk ke halaman registrasi
            printf("[> REGISTRATION PAGE <]\n");
	    //tampilkan page user
            printf("Username: ");
	    //masukkan data akun
            scanf("%s",akun);
	    //kirim data server
            send(sock, akun, strlen(akun), 0);
	    //tampilkan sandi
            printf("Password: ");
	    //masukkan sandi
            scanf("%s",sandi);
	    //kirim data server
            send(sock, sandi, strlen(sandi), 0);}

        read(sock, regist, 512);
	// jika berhasil cetak regis sukses
        if(strcmp("Auth Success",regist) == 0)
        {printf("register Success\n");}
	//jika gagal register gagal
        else if(strcmp("Authentic Fail",regist) == 0)
        { printf("register Failed\n");}}
}
int main(int argc, char const *argv[]){
	//inisiasi variabel1
    	struct sockaddr_in serv_address;
	//inisiasi variabel2
	struct sockaddr_in addres;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\nsocket cration failed\n");
        return -1;}

    memset(&serv_address, '0', sizeof(serv_address));

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0){
        printf("\n Invalid address / address not supported\n");
        return -1;}

    if(connect(sock, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0)
    {printf("\nConnection failed\n");
        return -1;}

    pthread_create(&(tid[0]),NULL,&screen1,NULL);
    pthread_create(&(tid[1]),NULL,&screen1,NULL);

    pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);}
