# SoalShiftSISOP20_modul3_B05

## Soal 1
Mohon maaf belum

## 2. TapTap Game adalah game online berbasis text console. Terdapat 2 program yaitu tapserver.c dan tapplayer.c Spesifikasi Game :  CLIENT SIDE Screen 1 :  1. Login 2. Register  Choices : {your input} screen 2 : Screen 2 : 1. Find Match 2. Logout  Choices : {your input} dengan ketentuan lain yang terdapat pada soal shift 3.

===BAGIAN CLIENT===

```
pthread_t process[3];
pid_t chd;

    int stat = 0, valread;
    char daftar[512];
    char akun[60];
    char sandi[60];
```
Itu untuk membuat variabelnya, beberapa variabel diatas digunakan dalam program seperti daftar,akun dan sandi
untuk masing-masing menyimpan register,username dan, password.

```
void* screen1(void *arg)
{
    char *arr[] = {"clear",NULL};
    pthread_t id = pthread_self();

    if(pthread_equal(id,process[0]))
    {
        chd = fork();
        if(chd == 0)
        {
            execv("/usr/bin/clear",arr);
        }
    }
    else if(pthread_equal(id,process[1]))
    {
        char input[25];
        scanf("%s",input);

        if(strcmp("login",input)==0)
        {
            send(stat, input, strlen(input), 0);
            printf("[[LOGIN ]]\n");
            printf("Username:\t");
            scanf("%s",akun);
            send(stat, akun, strlen(akun), 0);
            printf("Password:\t");
            scanf("%s",sandi);
            send(stat, sandi, strlen(sandi), 0);
        }

        else if(strcmp("register",input)==0)
        {
            send(stat, input, strlen(input), 0);
            printf("[[ REGISTRATION ]]\n");
            printf("Username:\t");
            scanf("%s",akun);
            send(stat, akun, strlen(akun), 0);
            printf("Password:\t");
            scanf("%s",sandi);
            send(stat, sandi, strlen(sandi), 0);
        }

        read(stat, regist, 512);
        if(strcmp("Auth Success",regist) == 0)
        {
            printf("register Success\n");
        }
        else if(strcmp("Auth Failed",regist) == 0)
        {
            printf("register Failed\n");
        }

    }
}
```
untuk fungsinya dibentuk fugsi screen1 berupa void yang nantinya akan di gunakan untuk menampilkan case yang pertama yaitu register dan login. untuk melakukan login pertama diharuskan membuat akun dengan cara register terlebih dahulu. 
pada register dan login cara kerjanya sama yaitu menghitung panjang string lalu di kirim ke server. apabila cocok maka akan di print auth succes dan jika gagal auth failed.

```
int main(int argc, char const *arr[])
{
    struct stataddr_in addres;
    struct stataddr_in serv_address;
    if((stat = statet(AF_INET, STAT_STREAM, 0)) < 0)
    {
        printf("\nstatet cration failed\n");
        return -1;
    }

    memset(&serv_address, '0', sizeof(serv_address));

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0)
    {
        printf("\n Invalid address / address not supported\n");
        return -1;
    }

    if(connect(stat, (struct stataddr *)&serv_address, sizeof(serv_address)) < 0)
    {
        printf("\nConnection failed\n");
        return -1;
    }

    pthread_create(&(process[0]),NULL,&screen1,NULL);
    pthread_create(&(process[1]),NULL,&screen1,NULL);

    pthread_join(process[0],NULL);
	pthread_join(process[1],NULL);
}

```
pada fungsi main digunakan beberapa fungsi template yang terdapat pada modul untuk koneksi antara client dan sever.
apabila tidak terjadi koneksi antara client dan server maka sistem akan print koneksi failed. dan jika berhasil maka akan menampilkan
halaman yang sesuai dengan input user.
lalu create thread dan join thread dipanggil pada bagian ini agar dapat eksekusi file thread yang sudah dibuat sebelumnya.
jika tahapan benar maka login dan register dapat dijalankan.

===BAGIAN SERVER===

```
int main(int *argc, char const *argv[])
{

        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char username[1024];
        char password[1024];
        char S_username[1024];
        char S_password[1024];
        char status[1024] = {0};
        char *auth1 = "Auth Success";
        char *auth0 = "Auth Failed";
```
Deklarasi vaiabel diletakkan dalam fungsi main.
beberapa variabel tersebut fungsinya beberapa sama seperti yang ada di server seperti username password, dan status.
ada beberapa tambahan variabel char untuk authentifikasi file.

```
        int test1,test2,comp;

        if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socked failed");
            exit(EXIT_FAILURE);
        }

        if((setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if(listen(server_fd,3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }


            valread = read( new_socket, status, 1024);
            // printf("%d",valread);
            // printf("%s\n",status);
```
pada intinya bagian tersebut berfungsi untuk check status antara client dan server apakah sudah terhubung atau tidak. jika terlah terjadi koneksi maka proses
dapat dilanjutkan menuju screen 1 yaitu register dan login.

```
            if( strcmp("register",status)==0 )
            {
                char c;
                FILE *fp,*fp2,*fp3;
                fp = fopen("akun.txt","a");
                read( new_socket, S_username, 1024);
                read( new_socket, S_password, 1024);

                fprintf(fp,"%s",S_username);
                fputs(" ",fp);
                fprintf(fp,"%s",S_password);

                fputs("\n",fp);
                fclose(fp);

                fp2 = fopen("akun.txt","r");
                while((c = getc(fp2)) != EOF)
                {
                    printf("%c",c);
                }
                fclose(fp2);

                fp3 = fopen("akun.txt","r");
                while((c = getc(fp3)) != EOF)
                {

                    fscanf(fp3,"%s",username);
                    fscanf(fp3,"%s",password);
                    test1 = strcmp(S_username,username);
                    test2 = strcmp(S_password,password);

                    if(test1==0 && test2==0)
                    {
                        comp=1;
                        if(comp==1)
                        {
                            send(new_socket, auth1, strlen(auth1), 0);
                        }
                        else
                        {
                            send(new_socket, auth0, strlen(auth0), 0);
                        }
                        break;
                        
                    }
                }
                fclose(fp3);

            }
```
untuk kondisi apabila user menginputkan register
username dan password akan disimpan dan digunakan atau di compare dengan variabel yang ada pada login
dan apabila sama maka status login akan suskses.

```
            else if( strcmp("login",status)==0 )
            {
                char c;
                int test1,test2;
                FILE *fp4;
                fp4 = fopen("akun.txt","r");
                read( new_socket, S_username, 1024);
                read( new_socket, S_password, 1024);

                while((c = getc(fp4)) != EOF)
                {
                	
                    fscanf(fp4,"%s",username);
                    fscanf(fp4,"%s",password);
                    test1 = strcmp(S_username,username);
                    test2 = strcmp(S_password,password);

                    if(test1==0 && test2==0)
                    {
                        break;
                    }
                }

                if(test1==0 && test2==0)
                {
                    printf("%s\n",auth1);
                }
                else
                {
                    printf("%s\n",auth0);
                }

            }
}
```
jika yang diinputkan login maka dia akan membandingkan string yang ada pada login dengan register
sehingga dapat mengauthentifikasi apakah username dan id sama. jika sama maka jalankan game jika gagal maka authentikasi akan gagal.

## Soal 3
Mohon maaf belum

## 4. Norland adalah seorang penjelajah terkenal. Pada suatu malam Norland menyusuri jalan setapak menuju ke sebuah gua dan mendapati tiga pilar yang pada setiap pilarnya ada sebuah batu berkilau yang tertancap. Batu itu berkilau di kegelapan dan setiap batunya memiliki warna yang berbeda. Norland mendapati ada sebuah teka-teki yang tertulis di setiap pilar. Untuk dapat mengambil batu mulia di suatu pilar, Ia harus memecahkan teka-teki yang ada di pilar tersebut. Norland menghampiri setiap pilar secara bergantian.

### 4a. Batu mulia pertama​ . Emerald. Batu mulia yang berwarna hijau mengkilat. Pada batu itu Ia menemukan sebuah kalimat petunjuk. Ada sebuah teka-teki yang berisi: 1. Buatlah program C dengan nama "​ 4a.c​ ", yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah ​ 4x2​ , dan matriks kedua ​ 2x5​ . Isi dari matriks didefinisikan ​ di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (​ tidak perlu​ dibuat filter angka). 2. Tampilkan matriks hasil perkalian tadi ke layar.
Langkah-langkah :
- Membuat key ``` key_t key = 1234; ```
- Get ID and Create IPC
``` int shmid = shmget(key, 20*sizeof(int), IPC_CREAT | 0666); ```
- Assign value (pointer untuk array) untuk menempelkan shared memory yang ditunjukkan ke ruang alamat program
``` value = (int *)shmat(shmid, NULL, 0); ```
- Deklarasi matrix (disini menggunakan rand() untuk mendapatkan angka random untuk matrix)
- Pakai thread untuk memanggil function multiply (kali)
```
pthread_t t;
pthread_create(&t, NULL, &kali, NULL);
pthread_join(t, NULL);
```
- Outputkan hasil perkalian matrix
- Melepaskan segmen shared memory yang terletak di alamat
``` shmdt((void *) value); ```

### 4b. Batu kedua adalah Amethyst. Batu mulia berwarna ungu mengkilat. Teka-tekinya adalah: 1. Buatlah program C kedua dengan nama "​ 4b.c​ ". Program ini akan mengambil variabel ​ hasil perkalian matriks dari program "4a.c" (program sebelumnya), dan tampilkan hasil matriks tersebut ke layar. (​ Catatan!​ : gunakan shared memory) 2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai ​ faktorialnya​ , dan tampilkan hasilnya ke layar dengan format seperti matriks.
Langkah-langkah : 
- Membuat key ``` key_t key = 1234; ```
- Get ID and Execute IPC ``` int shmid = shmget(key, 20*sizeof(int), IPC_EXCL); ```
- Assign value (pointer untuk array) untuk menempelkan shared memory yang ditunjukkan ke ruang alamat program
``` value = (int *)shmat(shmid, NULL, 0); ```
- Print matrix perkalian
- Deklarasi 20 thread (sesuai banyak elemen matrix) ``` pthread_t tid[20]; ```
- Loop untuk membuat 20 thread yang akan menjalankan function factorial (ini uda dimodiv supaya + bukan *
```
for(j = 0; j < 20; j++){
        pthread_create(&tid[j], NULL, &factorial, NULL);
    }
```
- Loop untuk join thread
```
for(j = 0; j < 20; j++){
        pthread_join(tid[j], NULL);
    }
```
- Output matrix factorial
- Melepaskan segmen shared memory yang terletak di alamat
``` shmdt((void *) value); ```

### 4c. Batu ketiga adalah Onyx. Batu mulia berwarna hitam mengkilat. Pecahkan teka-teki berikut! 1. Buatlah program C ketiga dengan nama "​ 4c.c​ ". Program ini tidak memiliki hubungan terhadap program yang lalu. 2. Pada program ini, Norland diminta mengetahui jumlah file dan folder di direktori saat ini dengan command "​ ls | wc -l​ ". Karena sudah belajar IPC, Norland mengerjakannya dengan semangat. (​ Catatan!​ : Harus menggunakan IPC Pipes)
Langkah-langkah : 
- Buat array untuk pipe ``` int pi[2]; ```
- Buat pipe ``` pipe(pi);
```
if(fork() == 0){
        dup2(pi[1], 1);
        close(pi[0]);
        close(pi[1]);

        char *argv[] = {"ls", NULL};
        execv("/bin/ls", argv);
    }
```
Disini ``` dup2(pi[1], 1); ``` adalah untuk menyimpan hasil ``` ls ``` di pi[1] dan argumen selanjutnya adalah 1 karena kita ingin write. Lalu jangan lupa menutup pipe.
```
if(fork() == 0){
        dup2(pi[0], 0);
        close(pi[1]);
        close(pi[0]);
            
        char *argv[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv);
    }
```
Disini ``` dup2(pi[0], 0); ``` adalah untuk mengambil hasil ``` ls ``` kemudian menaruhnya di pi[0] dan argumen selanjutnya adalah 0 karena kita ingin read. Hasil read tersebut akan dipakai untuk execute ``` wc -l ```. Lalu jangan lupa menutup pipe.
