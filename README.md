# SoalShiftSISOP20_modul3_B05

## Soal 1
Soal ini meminta kami untuk membuat game berbasis text terminal mirip game Pokemon GO. Solusi soal ini terdiri dari 2 file yaitu soal1_traizone.c dan soal1_pokezone.c. Untuk menyelesaikan soal ini, kami menggunakan multithreading untuk menjalankan beberapa proses secara paralel (misal meng-handle perhitungan masing-masing Pokemon yang dimiliki), shared memory untuk menyimpan value stok Shop yang diakses baik di file soal1_traizone.c maupun soal1_pokezone.c, serta fork-exec agar saat mengeksekusi soal1_pokezone.c dan memberikan command Quit, maka program soal1_traizone.c akan berhenti (di-kill).

Untuk source code soal1_traizone.c terdiri dari 2 mode, yaitu:

Normal Mode
Capture Mode
Normal Mode
Saat sedang berada di Normal Mode, kita bisa memilih 5 menu:

Cari Pokemon
Pokedex
Store
Go to Capture Mode
Keluar
Kami tambahkan 1 menu, Go to Capture Mode agar mempermudah pemindahan mode. Source code:

```
printf("Normal Mode\n");
if(!cari) printf("1. Cari Pokemon\n2. Pokedex\n3. Shop\n4. Go to Capture Mode\nPick a number: ");
else printf("1. Berhenti Mencari\n2. Pokedex\n3. Shop\n4. Go to Capture Mode\nChoice : ");
```

Kemudian opsi pertama dapat berubah sesuai kondisi sekarang, jika sedang mencari Pokemon, maka opsi pertama menjadi Berhenti Mencari. Untuk mengetahui kondisi sedang mencari atau tidak, digunakan variabel boolean cari sebagai penanda. 1 berarti sedang mencari, 0 berarti sedang tidak mencari.

### 1. Cari Pokemon
Untuk menu Cari Pokemon, ada beberapa ketentuan yaitu sebagai berikut:
Tiap 10 detik (sleep(10)) ada chance 60% untuk menemukan Pokemon. Yang tiap-tiap kelompok Pokemon (Normal, Rare, dan Legendary) memiliki tingkat encounter yang berbeda-beda (80%, 15%, dan 5%). Untuk menangani masalah ini, kami menggunakan perintah srand(time(NULL)); untuk mendapatkan angka random yang dipanggil dengan fungsi rand(). Sehingga untuk mendapatkan kemungkinan sekian persen dapat menggunakannya dalam syntax sebagai berikut:

```
void *capturePokemon()
{
	int esc_chance = (pokecapt % 15) / 5;
	if(esc_chance == 0) esc_chance = 1;
	else esc_chance *= 2;
	esc_chance *= 5;
	if(pokecapt >= 15) esc_chance += 5;
	while(1)
	{
		if(rand() % 100 < esc_chance)
		{
			printf("Pokemon escaped.\n");
			pthread_mutex_lock(&pokecapt_cek);
			pokecapt = -1;
			pthread_mutex_unlock(&pokecapt_cek);
			pthread_exit(0);
		}
		sleep(20);
	}
}

void *findPokemon()
{
	while(1)
	{
		if(rand() % 100 < 60)
		{
			cari = 0;
			pthread_mutex_lock(&pokecapt_cek);
			pokecapt = *vpoke;
			pthread_mutex_unlock(&pokecapt_cek);
			pthread_create(&th2, NULL, capturePokemon, NULL);
			normal = 0;
			pthread_exit(0);
		}
		sleep(10);
	}
}

int main()
{
    ...
    cari = cari ^ 1;
    if(cari) pthread_create(&th1, NULL, findPokemon, NULL);
    else pthread_cancel(th1);
    ...
}
```
Fungsi findPokemon dijalankan pada thread th1, di mana fungsi thread ini adalah untuk mencari pokemon secara random dengan 60% encounter rate sehingga digunakan syntax rand() % 100 < 60, di mana kemungkinan memenuhi persamaan ini adalah 60 dari 100 kemungkinan (dapat disebut 60%). Kemudian jika kita menemukan pokemon, maka akan digunakan fungsi pthread_mutex_lock(&pokecapt_cek); dan pthread_mutex_unlock(&pokecapt_cek); untuk mengunci agar tidak ada proses lain yang mengakses memory yang sedang diakses (mutual exclusion). Kemudian syntax pokecapt = *vpoke; untuk meng-assign nilai ID pokemon yang secara random didapatkan dari source code bagian soal1_pokezone.c fungsi randomPoke():
```
void *restockShop()
{
	while(1)
	{
		*vpow += 10; *vball += 10; *vberry += 10;
		if(*vpow > 200) *vpow = 200;
		if(*vball > 200) * vball = 200;
		if(*vberry > 200) *vberry = 200;
		sleep(10);
	}
}

void *randomPoke()
{
	while(1)
	{
		int newpoke = rand() % 100;
		int shiny = rand() % 8000;
		if(newpoke < 5) newpoke = (newpoke % 5) + 10;
		else if(newpoke < 20) newpoke = (newpoke % 5) + 5;
		else newpoke = (newpoke % 5);
		if(shiny == 1) newpoke += 15;
		*vpoke = newpoke;
		sleep(1);
	}
}
```

Dari fungsi atas (sebagian file soal1_pokezone.c), didapatkan bahwa variabel *vpoke berisi ID pokemon yang secara random didapatkan sesuai dengan encounter ratenya, yaitu 1/8000 untuk tipe Shiny (15 <= ID <= 29), 5/100 untuk tipe Legendary (10 <= ID <= 14), 15/100 untuk tipe Rare (5 <= ID <= 9), dan 80/100 tipe Normal (0 <= ID <= 4). Dan ada juga fungsi untuk restock stok setiap 10 detik.

Kemudian kembali ke bagian file soal1_traizone.c, saat di fungsi findPokemon sudah menemukan pokemon, maka membuat thread baru dengan nama th2, yang bertugas untuk meng-handle chance tertangkapnya suatu pokemon tersebut dengan escape rate yang berbeda-beda. Solusi penanganan mirip dengan penanganan encounter rate.

### 2. Pokedex
Untuk menu Pokedex, kami diminta menampilkan list Pokemon yang sudah tertangkap beserta nilai AP-nya, kemudian ada opsi untuk melepas Pokemon ("menjual") atau memberi Berry kepada semua Pokemon yang sudah tertangkap. Source code penyelesaian bagian ini:

```
typedef struct slot {
	pthread_t poketh;
	int pokeid, pokeap;
} slot;

slot pokeslot[10];

int main()
{
        ...
        printf("Pokedex\n");
	printf("No  Pokemon               AP\n");
	for(int i = 0; i < 7; i++)
	{
		printf("%d)  %-22s%d\n", i + 1, pokename[pokeslot[i].pokeid], pokeslot[i].pokeap);
	}
	printf("1. Lepas Pokemon\n2. Beri Berry\n3. Kembali\nChoice : ");
	scanf("%d", &cmd2);
	if(cmd2 == 1)
	{
		int no;
		while(1)
		{
			printf("Pilih Nomor Pokemon : ");
			scanf("%d", &no);
			if(no > 0 && no < 8 && pokeslot[no - 1].pokeid != -1) break;
			else printf("Pilih slot yang benar.\n");
		}
		pthread_mutex_lock(&poke_cek);
		pokeslot[no - 1].pokeap = -1;
		pthread_cancel(pokeslot[no - 1].poketh);
		int id = pokeslot[no - 1].pokeid;
		char nama[50];
		strcpy(nama, pokename[pokeslot[no - 1].pokeid]);
		if(id >= 15) dollar += 5000;
		else if(id < 5) dollar += 80;
		else if(id < 10) dollar += 100;
		else if(id < 15) dollar += 200;
		pokeslot[no - 1].pokeid = -1;
		pthread_mutex_unlock(&poke_cek);
		printf("Pokemon %s sudah dilepas, nilai poke dollar sekarang %d.\n", nama, dollar);
	}
	else if(cmd2 == 2)
	{
		if(berry <= 0)
		{
			printf("Tidak mempunyai berry.\n");
		}
		else
		{
			berry--;
			pthread_mutex_lock(&poke_cek);
			for(int i = 0; i < 7; i++)
			{
				if(pokeslot[i].pokeap != -1) pokeslot[i].pokeap += 10;
			}
			pthread_mutex_unlock(&poke_cek);
			printf("Pokemon sudah diberi berry.\n");
		}
	}
	else if(cmd2 == 3) printf("Keluar dari Pokedex.\n");
        ...
}
```
Agar mempermudah penyimpanan dan akses pokemon-pokemon yang dimiliki (7 slot), kami menggunakan array yang menampung tipe data struct yang berisi ID Pokemon, AP Pokemon, serta Thread Pokemon itu. Saat memasuki opsi Pokedex, otomatis mencetak semua isi 7 slot Pokemon yang dimiliki. Kemudian apabila ingin Melepas Pokemon, tinggal memilih no berapa dan kemudian slot pokemon itu akan direset dan threadnya dihentikan, serta Poke Dollar bertambah sesuai harga Pokemon yang dilepas. Untuk opsi Memberi Berry, maka akan dicek dulu apakah berry-nya tersedia (kita punya/tidak). Jika ada, maka dilakukan looping untuk mengecek masing-masing slot Pokemon, apabila bertemu ID Pokemon selain -1 (tanda kosong), maka AP nya akan ditambah 10. Tidak lupa harus menggunakan library pthread_mutex_lock dan pthread_mutex_unlock.

### 3. Shop
Untuk menu Shop, kami diminta untuk menampilkan item-item yang tersedia untuk dibeli dengan Poke Dollar yang dimiliki (inisiasi kami beri 100). Item yang ada adalah Lullaby Powder yang jika diaktifkan, dapat bertahan selama 10 detik untuk meningkatkan capture chance saat ingin menangkap Pokemon. Item kedua adalah Pokeball yang digunakan untuk menangkap Pokemon, dan yang ketiga adalah Berry untuk menambah AP setiap Pokemon sebanyak 10. Masing-masing item akan ditampilkan harga, stok, dan available (yang kita punya). Source code penyelesaian bagian ini:

```
int main()
{
	...
	printf("Shop\n");
	printf("No  Item            Harga  Stok  Available\n");
	printf("==========================================\n");
	printf("1)  Lullaby Powder  60     %-3d  %-3d\n", *vpow, powder);
	printf("2)  Pokeball        5      %-3d  %-3d\n", *vball, ball);
	printf("3)  Berry           15     %-3d  %-3d\n", *vberry, berry);
	printf("4)  Kembali\n");
	printf("Pokedollar yang dimiliki : %d\nChoice : ", dollar);
	scanf("%d", &cmd2);
	if(cmd2 == 4) printf("Keluar dari Shop.\n");
	else
	{
		int n;
		printf("Banyak : ");
		scanf("%d", &n);
		if(n > 99)
		{
			printf("Max item yang dibeli 99. Gagal membeli.\n");
		}
		else if(cmd2 == 1)
		{
			if((powder + n) > 99)
			{
				printf("Max item yang dimiliki 99. Gagal membeli.\n");
			}
			else if(*vpow < n)
			{
				printf("Stok tidak mencukupi.\n");
			}
			else if(dollar < n * 60)
			{
				printf("Pokedollar tidak mencukupi.\n");
			}
			else
			{
				dollar -= (n * 60);
				*vpow -= n;
				powder += n;
				printf("%d Lullaby Powder sudah terbeli.\n", n);
			}
		}
		else if(cmd2 == 2)
		{
			if((ball + n) > 99)
			{
				printf("Max item yang dimiliki 99. Gagal membeli.\n");
			}
			else if(*vball < n)
			{
				printf("Stok tidak mencukupi.\n");
			}
			else if(dollar < n * 5)
			{
				printf("Pokedollar tidak mencukupi.\n");
			}
			else
			{
				dollar -= (n * 5);
				*vball -= n;
				ball += n;
				printf("%d Pokeball sudah terbeli.\n", n);
			}
		}
		else if(cmd2 == 3)
		{
			if((berry + n) > 99)
			{
				printf("Max item yang dimiliki 99. Gagal membeli.\n");
			}
			else if(*vberry < n)
			{
				printf("Stok tidak mencukupi.\n");
			}
			else if(dollar < n * 15)
			{
				printf("Pokedollar tidak mencukupi.\n");
			}
			else
			{
				dollar -= (n * 15);
				*vberry -= n;
				berry += n;
				printf("%d Berry sudah terbeli.\n", n);
			}
		}
	}
	...
}
```
Terlihat bahwa saat memasuki Shop, maka akan langsung dicetak semua item yang dijual beserta dengan harga, stok, dan availablenya. Kemudian jika memilih untuk membeli suatu item, maka akan ditanyakan banyaknya item yang mau dibeli. Kemudian akan dicek terlebih dahulu apakah Poke Dollar yang dimiliki cukup untuk membeli item dengan banyak yang diinginkan, dan apakah stok ada, serta apakah jika melakukan pembelian, maka item melebihi 99 (max diizinkan memiliki 99 untuk setiap item). Apabila memenuhi semua syarat, barulah Poke Dollar akan terpotong, stok di shared memory akan berkurang, sedangkan available yang dimiliki pemain akan ditambah. Hal yang sama terjadi untuk setiap item.

Capture Mode
Saat sedang berada di Capture Mode, kita bisa memilih 3 menu:

Tangkap
Item
Keluar 

```
printf("Capture Mode\n");
if(pokecapt == -1) printf("1. Tangkap\n2. Item\n3. Keluar\nChoice : ");
else printf("Pokemon %s ditemukan.\n1. Tangkap\n2. Item\n3. Keluar\nChoice : ", pokename[pokecapt]);
```

### 1. Tangkap
Untuk menu Tangkap, awalnya kita akan mengecek dulu apakah memiliki Pokeball untuk menangkap Pokemon, dan apakah sudah ada pokemon yang ditemukan. Jika kedua syarat terpenuhi barulah melakukan random capture chance berdasarkan keaktifan lullaby powder, dan tipe pokemon yang ditemukan itu. Apabila Pokemon tertangkap, maka akan dilakukan looping sebanyak slot Pokemon untuk mencari slot yang kosong. Apabila tidak ada, maka Pokemon tersebut otomatis terlepas dan kita mendapatkan uang sebanyak harga Pokemon tersebut langsung diakumulasi dalam Poke Dollar yang kita miliki. Jika tidak penuh, maka slot tersebut akan diisi dengan ID Pokemon baru, AP Pokemon baru (tidak lupa dengan lock dan unlock mutex), dan thread Pokemon tersebut akan dimulai, di mana bertugas untuk meng-counter apakah Pokemon akan terlepas (karena setiap 10 detik, AP masing-masing Pokemon berkurang 10 poin). Jika AP = 0, ada 10% kemungkinan AP menjadi 50, sedangkan 90% kemungkinan akan terlepas. Kemudian thread pencarian pokemon (th2) akan dihentikan. Source code:

```
void *pokeEscape(void *arg)
{
	int s = *(int *)arg;
	while(1)
	{
		if(!normal) continue;
		pthread_mutex_lock(&poke_cek);
		pokeslot[s].pokeap -= 10;
		if(pokeslot[s].pokeap == 0)
		{
			if(rand() % 100 < 10) pokeslot[s].pokeap += 50;
			else
			{
				printf("Pokemon %s lepas.\n", pokename[pokeslot[s].pokeid]);
				pokeslot[s].pokeap = -1;
				pokeslot[s].pokeid = -1;
				pthread_exit(0);
			}
		}
		pthread_mutex_unlock(&poke_cek);
		sleep(10);
	}
}

int main()
{
	...
	if(ball == 0)	
	{
		printf("Tidak mempunyai pokeball.\n");
	}
	else if(pokecapt == -1)
	{
		printf("Belum ada pokemon yang ditemukan.\n");
	}
	else
	{
		ball--;
		int chance = 0;
		if(powder_active) chance += 20;
		if(pokecapt >= 15) chance -= 20;
		int x = (pokecapt % 15) / 5;
		if(x == 0) chance += 70;
		else if(x == 1) chance += 50;
		else if(x == 2) chance += 30;
		if(rand() % 100 < chance)
		{
			printf("Pokemon %s tertangkap.\n", pokename[pokecapt]);
			bool empty = 0;
			int id;
			for(int i = 0; i < 7; i++)
			{
				if(pokeslot[i].pokeid == -1)
				{
					empty = 1;
					id = i;
					break;
				}
			}
			if(!empty)
			{
				if(pokecapt >= 15) dollar += 5000;
				int temp = (pokecapt % 15) / 5;
				if(temp == 0) dollar += 80;
				else if(temp == 1) dollar += 100;
				else if(temp == 2) dollar += 200;
				printf("Slot penuh, pokemon %s terlepas, poke dollar sekarang %d.\n", pokename[pokecapt], dollar);
			}
			else
			{
				pthread_mutex_lock(&poke_cek);
				pokeslot[id].pokeid = pokecapt;
				pokeslot[id].pokeap = 100;
				pthread_mutex_unlock(&poke_cek);
				pthread_create(&pokeslot[id].poketh, NULL, pokeEscape, (void *)&id);
				pthread_cancel(th2);
				pthread_mutex_lock(&pokecapt_cek);
				pokecapt = -1;
				pthread_mutex_unlock(&pokecapt_cek);
			}
		}
		else printf("Pokemon %s tidak tertangkap.\n", pokename[pokecapt]);
	}
	...
}
```
Fungsi pokeEscape berjalan pada masing-masing Pokemon untuk menghitung pengurangan AP Pokemon setiap 10 detik, serta meng-handle apa yang terjadi saat AP = 0, kejadian yang terjadi disesuaikan dengan peluangnya.

### 2. Item
Untuk menu Item, digunakan untuk memakai lullaby powder. Dapat digunakan apabila lullaby powder sudah tidak aktif dan memiliki lullaby powder. Source code:

```
void *lullabyPowder()
{
	while(10) sleep(10);
	powder_active = 0;
	printf("Lullaby Powder sudah tidak aktif.\n");
	pthread_exit(0);
}

int main()
{
	if(powder_active)
	{
		printf("Lullaby Power masih aktif.\n");
	}
	if(powder < 1)
	{
		printf("Lullaby Powder kosong.\n");
	}
	else
	{
		powder--;
		powder_active = 1;
		pthread_create(&th3, NULL, lullabyPowder(), NULL);
	}
}
```
Fungsi lullabyPowder dijalankan oleh thread th3 untuk menangani durasi masa aktif lullaby powder setiap digunakan yaitu 10 detik. Setelah sudah melebihi 10 detik, maka thread akan di-exit (berhenti).

Shared Memory & Thread_Mutex
Dikarenakan soal ini terdiri dari 2 file sedangkan mengakses beberapa value yang sama, digunakan shared memory untuk mengatasi pokemon yang dirandom (shmidpoke), stok lullaby powder (shmidpow), stok pokeball (shmidball), dan stok berry (shmidberry). Kemudian pointer ke lokasi memory ini akan disimpan dalam variabel *vpoke, *vpow, *vball, dan *vberry. Digunakan fungsi shmget untuk mengambil shared memory pada key tertentu, kemudian shmat untuk attack, dan shmdt untuk detach, serta smhctl.

Kemudian untuk inisiasi pthead mutual exclusion, digunakan fungsi pthread_mutex_init dan untuk mengakhirinya, digunakan fungsi pthread_mutex_destroy.

Source code:
```
key_t keypoke = 1234;
key_t keypow = 1235;
key_t keyball = 1236;
key_t keyberry = 1237;

int shmidpoke = shmget(keypoke, sizeof(int), IPC_CREAT | 0666);
vpoke = shmat(shmidpoke, NULL, 0);
int shmidpow = shmget(keypow, sizeof(int), IPC_CREAT | 0666);
vpow = shmat(shmidpow, NULL, 0);
int shmidball = shmget(keyball, sizeof(int), IPC_CREAT | 0666);
vball = shmat(shmidball, NULL, 0);
int shmidberry = shmget(keyberry, sizeof(int), IPC_CREAT | 0666);
vberry = shmat(shmidberry, NULL, 0);

pthread_mutex_init(&poke_cek, NULL);
pthread_mutex_init(&pokecapt_cek, NULL);
pthread_mutex_destroy(&poke_cek);
pthread_mutex_destroy(&pokecapt_cek);

shmdt(vpoke);
shmctl(shmidpoke, IPC_RMID, NULL);
shmdt(vpow);
shmctl(shmidpow, IPC_RMID, NULL);
shmdt(vball);
shmctl(shmidball, IPC_RMID, NULL);
shmdt(vberry);
shmctl(shmidberry, IPC_RMID, NULL);
exit(1);
```
Fork-exec
Digunakan pula fork-exec untuk menghentikan berjalannya game ini. Dengan menggunakan command bash kill yang dipanggil dengan mencari tahu dulu pid proses soal1_traizone.c yang ingin dihentikan. Dengan cara masuk ke dir /proc dan mencari file soal1_traizone.c, kemudian akan digunakan perintah fork dan execv untuk meng-kill PID proses tersebut. Setelah selesai meng-kill traizone, maka proses pokezone juga sudah selesai dan langsung berhenti juga. Source code:
```
struct dirent *dp;
DIR *dfd;
if((dfd = opendir("/proc")) == NULL)
{
	fprintf(stderr, "Can't open /proc\n");
	return 0;
}
char *endp;
while((dp = readdir(dfd)) != NULL)
{
	long pid = strtol(dp->d_name, &endp, 10);
	if(*endp != '\0') continue;
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "/proc/%ld/cmdline", pid);
	FILE *f = fopen(buffer, "r");
	if(f != NULL)
	{
		if(fgets(buffer, sizeof(buffer), f) != NULL) 
		{
			char *awal = strtok(buffer, " ");
			if(strcmp(awal, "./soal1_traizone") == 0)
			{
				pid_t cid;
				cid = fork();
				if(cid < 0) exit(0);
				if(cid == 0)
				{
					char killc[1024];
					sprintf(killc, "kill -INT %ld", pid);
					char *ag[] = {"sh", "-c", killc, NULL};
					execv("/bin/sh", ag);
				}
			}
		}
		fclose(f);
	}
}
closedir(dfd);
int stat;
while(wait(&stat) > 0);
```

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

=== BAGIAN SCREEN 2 ===
pada bagian ini akan terdapat menu matchmaking yaitu find yang berfungsi untuk menemukan lawan. dan menu logout yaitu untuk kembali ke menu screen 1.

```
void *finding()
{
    while(1)
    {
        printf("Waiting for player ...\n");
        sleep(1);
    }
}

void *playing(void *arg)
{
    while(1)
    {
        char ch = getchar();
        if(ch == ' ') 
        {
            printf("hit !!\n");
            send(*(int*) arg, &ch, sizeof(ch), 0);
        }
    }
}

int main()
{
	screen2:
	printf("1. Find Match\n2. Logout\nChoices : ");
	scanf("%s", cmd2);
	if(strcmp(cmd2, "logout") == 0)
	{
		send(sock, cmd2, strlen(cmd2), 0);
		goto screen1;
	}
	else if(strcmp(cmd2, "find") == 0)
	{
		int start;
		send(sock, cmd2, strlen(cmd2), 0);
		pthread_t th;
		pthread_create(&th, NULL, finding, NULL);
		read(sock, &start, sizeof(start));
		pthread_cancel(th);
		printf("Game dimulai silahkan \e[3mtap tap\e[0m secepat mungkin !!\n");
		struct termios prev, cur;
		tcgetattr(0, &prev);
		cur = prev;
		cur.c_lflag &= -ICANON;
		cur.c_lflag &= -ECHO;
		tcsetattr(0, TCSANOW, &cur);
		pthread_t th2;
		pthread_create(&th2, NULL, playing, (void *) &sock);
		int health = 100;
		while(1)
		{
		    read(sock, &health, sizeof(health));
		    if(health == -1) break;
		    printf("Health: %d\n", health);
		}
		pthread_cancel(th2);
		bool cek;
		read(sock, &cek, sizeof(cek));
		if(cek) printf("Game berakhir kamu menang\n");
		else printf("Game berakhir kamu kalah\n");
		tcsetattr(0, TCSANOW, &prev);
		goto screen2;
	}
	else
	{
		printf("invalid input\n");
		goto screen2;
	}
	...
}

```

jadi pada screen 2 ini akan menjalankan 2 thread, yaitu yang pertam berfungsi untuk menjalankan "waiting for player ..." (pthread_create(&th, NULL, finding, NULL);) ketika melakukan matchmaking. dan akan terus berjalan hingga ditemukan lawan. lalu untuk mengecek ada dan tidaknya pemain, memakai fungsi read(sock, &start, sizeof(start));, yang mana variabel start akan dipassing dari server side dan memberitahu bahwa game sudah siap dimulai. untuk fungsi termios yaitu agar pada saat bermain, saat user menginputkan karakter spasi, langsung terbaca hit lawan. jadi tidak perlu menekan tombol enter. Saat bermain maka akan dijalankan thread yang berfungsi menunggu player saat menginput spasi untuk menembak musuh. Saat health salah satu player sudah habis (<= 0), maka server akan mengirimkan socket value health -1, yang menandakan game dihentikan, kemudian server mengirimkan kode ke masing-masing player apakah ia menang atau kalah.

## 3. Buatlah sebuah program dari C untuk mengkategorikan file. Program ini akan memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan ● Pada opsi -f tersebut, user bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang user inginkan seperti contoh di atas.  ● Pada program kategori tersebut, folder jpg,c,zip tidak dibuat secara manual, melainkan melalui program c. Semisal ada file yang tidak memiliki ekstensi, maka dia akan disimpan dalam folder “Unknown”.  ● Program kategori ini juga menerima perintah (*) seperti di bawah; ● Artinya mengkategori seluruh file yang ada di working directory ketika menjalankan program C tersebut.  ● Selain hal itu program C ini juga menerima opsi -d untuk melakukan kategori pada suatu directory. Untuk opsi -d ini, user hanya bisa menginput 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. ● Hasilnya perintah di atas adalah mengkategorikan file di /path/to/directory dan hasilnya akan disimpan di working directory di mana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).  ● Program ini tidak rekursif. Semisal di directory yang mau dikategorikan, atau menggunakan (*) terdapat folder yang berisi file, maka file dalam folder tersebut tidak dihiraukan, cukup file pada 1 level saja.  ● Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat. Dilarang juga menggunakan fork-exec dan system.  ● Silahkan download soal3.zip sebagai percobaan. Namun silahkan dicoba-coba sendiri untuk kemungkinan test case lainnya yang mungkin belum ada di soal3.zip. 

Langkah-langkah : 
- Cek command, apakah -f atau * atau -d
- Jika -f, maka akan membuat thread dan memanggil function move
```
if(strcmp(argv[1], "-f") == 0){
        while(argv[i] != NULL){
            pthread_create(&(tid[i-2]), NULL, &move, (void *)argv[i]);
            i++;
        }
        for(j = 0; j < (i-1); j++){
            pthread_join(tid[j], NULL);
        }
    }
```
- Function move akan memindahkan file yang diinputkan ke folder sesuai ekstensinya
```
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
```
- Jika * maka akan memindahkan semua file di current directory ke folder sesuai extensinya
```
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
```
- Jika -d, maka akan memindah file yang ada di directory yang dituju ke folder dengan nama ekstensinya lalu folder tersebut diletakkan di current directory (directory tempat program dijalankan)
```
```

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
