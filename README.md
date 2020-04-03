# SoalShiftSISOP20_modul3_B05

## 4.Norland adalah seorang penjelajah terkenal. Pada suatu malam Norland menyusuri jalan setapak menuju ke sebuah gua dan mendapati tiga pilar yang pada setiap pilarnya ada sebuah batu berkilau yang tertancap. Batu itu berkilau di kegelapan dan setiap batunya memiliki warna yang berbeda. Norland mendapati ada sebuah teka-teki yang tertulis di setiap pilar. Untuk dapat mengambil batu mulia di suatu pilar, Ia harus memecahkan teka-teki yang ada di pilar tersebut. Norland menghampiri setiap pilar secara bergantian.

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
