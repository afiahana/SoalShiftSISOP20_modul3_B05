#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#define PORT 8080

// fungsi utama
int main(int *argc, char const *argv[])
	//deklarasi variabel int
        int serv_id, n_sock, valread;
	//deklarasi variabel struct
        struct sockaddr_in address;
	//deklarasi variabel option
        int opotion = 1;
	//deklarasi variabel addlen
        int addrlen = sizeof(address);
	//deklarasi variabel untuk input akun
        char akun[512];
	//deklarasi variabel untuk input sandi
        char sandi[512];
	//deklarasi variabel s_akun
        char S_akun[512];
	//deklarasi variabel s_sandi
        char S_sandi[512];
	////deklarasi variabel check
        char check[512] = {0};
	//deklarasi variabel succes
        char *athen1 = "Authentification Success";
	//deklarasi variabel failed
        char *athen0 = "Authentification Failed";
	//deklarasi variabel coba dan compare
        int coba1,coba2,comp;
	// kondisi if
        if((serv_id = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {// jika eror tampilkan failed
            perror("socked failed")
            exit(EXIT_FAILURE);
        }
	//kondisi ketika set socket
        if((setsockopotion(serv_id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opotion, sizeof(opotion))))
        {//jika eror maka exit
            perror("setsockopotion");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
	//pengkondisian ketika size adres < o maka
        if(bind(serv_id, (struct sockaddr *)&address, sizeof(address)) < 0)
        {//lakukan koneksi gagal
            perror("bind failed");
            exit(EXIT_FAILURE);}

	//jika listen servid 3< 0 maka exit juga        
	if(listen(serv_id,3) < 0)
        {   perror("listen");
            exit(EXIT_FAILURE);}

	//jika socket diterima        
	if((n_sock = accept(serv_id, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {   perror("accept");
            exit(EXIT_FAILURE);}
	    //valread set input value
            valread = read( n_sock, check, 512);
	    //jika hasil comparenya sama maka lakukan
            if( strcmp("register",check)==0 )
            {	//variabel char c
                char c;
		//fp akan digunakan untuk file open
                FILE *fp,*fp2,*fp3;
		//jika berhasil akun akan tersimpan
                fp = fopen("akun.txt","a");
		//dipakai ketika akan dijalankan login
                read( n_sock, S_akun, 512);
                read( n_sock, S_sandi, 512);
		//kemudian print akun yang tersedia
                fprintf(fp,"%s",S_akun);
                fputs(" ",fp);
		//kemudian print sandi yang tersedia
                fprintf(fp,"%s",S_sandi);
                // fputc('\0',fp);
                fputs("\n",fp);
		//lalu close
                fclose(fp);

                fp2 = fopen("akun.txt","r");
		//input file sampai end of file
                while((c = getc(fp2)) != EOF)
                {//tampilkan hasil
                    printf("%c",c);}
                fclose(fp2);

                fp3 = fopen("akun.txt","r");
                while((c = getc(fp3)) != EOF)
                {
                    // printf("%s\n",S_akun);
                    // printf("%s\n",S_sandi);
                    fscanf(fp3,"%s",akun);
                    fscanf(fp3,"%s",sandi);
                    coba1 = strcmp(S_akun,akun);
                    coba2 = strcmp(S_sandi,sandi);

                    // perbandingan coba1 dan coba2
                    if(coba1==0 && coba2==0)
                    { //set comp menjadi 1
                        comp=1;
                        if(comp==1)
                        {//kirim datanya
                            send(n_sock, athen1, strlen(athen1), 0);}
                        else
                        {//kirim datanya
                            send(n_sock, athen0, strlen(athen0), 0);}
                        break;
                        
                    }
                }
                fclose(fp3);

            }

            else if( strcmp("login",check)==0 )
            {	
		//deklarasi variabel char c
                char c;
		//deklarasi variabel1 dan variabel2
                int coba1,coba2;
		//create pointer fp4
                FILE *fp4;
		//set fp4 = data dibawah 
                fp4 = fopen("akun.txt","r");
                read( n_sock, S_akun, 512);
		//lakukan read akun
                read( n_sock, S_sandi, 512);
		//lakukan read sandi

                while((c = getc(fp4)) != EOF)
                {   //masukan data akun
                    fscanf(fp4,"%s",akun);
		    //masukan data sandi
                    fscanf(fp4,"%s",sandi);
		    //compare string akun
                    coba1 = strcmp(S_akun,akun);
		    //compare string sandi
                    coba2 = strcmp(S_sandi,sandi);

                    if(coba1==0 && coba2==0)
		    //printf("data tidak valid")
                    { break; } }

                if(coba1==0 && coba2==0)
		//printf("data tersedia)                
		{ printf("%s\n",athen1);}
                else
		//printf("data tersedia)
                { printf("%s\n",athen0);} }
}
