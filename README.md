# SoalShiftSISOP20_modul3_B05

## 4.Norland adalah seorang penjelajah terkenal. Pada suatu malam Norland menyusuri jalan setapak menuju ke sebuah gua dan mendapati tiga pilar yang pada setiap pilarnya ada sebuah batu berkilau yang tertancap. Batu itu berkilau di kegelapan dan setiap batunya memiliki warna yang berbeda. Norland mendapati ada sebuah teka-teki yang tertulis di setiap pilar. Untuk dapat mengambil batu mulia di suatu pilar, Ia harus memecahkan teka-teki yang ada di pilar tersebut. Norland menghampiri setiap pilar secara bergantian.

### 4a. Batu mulia pertama​ . Emerald. Batu mulia yang berwarna hijau mengkilat. Pada batu itu Ia menemukan sebuah kalimat petunjuk. Ada sebuah teka-teki yang berisi: 1. Buatlah program C dengan nama "​ 4a.c​ ", yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah ​ 4x2​ , dan matriks kedua ​ 2x5​ . Isi dari matriks didefinisikan ​ di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (​ tidak perlu​ dibuat filter angka). 2. Tampilkan matriks hasil perkalian tadi ke layar.
Langkah-langkah :
- Membuat key ``` key_t key = 1234; ```
- Get ID and Create IPC
``` int shmid = shmget(key, 20*sizeof(int), IPC_CREAT | 0666); ```
- Assign value (pointer untuk array) agar nanti di shared memory
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
