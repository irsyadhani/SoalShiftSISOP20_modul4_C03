# SoalShiftSISOP20_modul4_C03
Kelompok C03:
* _Irsyadhani Dwi Shubhi (0511184000022)_
* _Reihan Nanda Ramadhan (05111840000132)_

----------------------------------------------------------------
## Soal
* [Soal 1](#soal-1-enkripsi-versi-1)
* [Soal 2](#soal-1-enkripsi-versi-2)
* [Soal 3](#soal-3-sinkronisasi-direktori-otomatis)
* [Soal 4](#soal-4-log-system)
----------------------------------------------------------------
# Soal 1 (Enkripsi Versi 1)
   _**Soal:**_
   1. Enkripsi Versi 1
a. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut
akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
b. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut
akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi
adirektori tersebut akan terdekrip.
d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan
tercatat ke sebuah database/log berupa file.
e. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher
dengan key.
   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
#
# Soal 2 (Enkripsi Versi 2)
   _**Soal:**_
   2. Enkripsi Versi 2
   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
#
# Soal 3 (Sinkronisasi Direktori Otomatis)
   _**Soal:**_
   3. Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:

   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
Belum tahu cara menyinkronisasi direktori
#
# Soal 4 (Log System)
   _**Soal:**_
4. a.	Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
b.	Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
c.	Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
d.	Sisanya, akan dicatat dengan level INFO.
e.	Format untuk logging yaitu: ```[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]```

   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_
Soal nomor 4 meminta untuk membuat log yang fungsinya untuk melakukan pencatatan, format pada file system-nya adalah ```[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]```. Terdapat 2 parameter log yaitu : ```WARNING``` untuk mencatat setiap systemcall rmdir serta unlink dan ```INFO``` untuk selain itu.
* Contoh isi dari log tersebut, yaitu:
```
INFO::200501-00:26:12::LS::/home/irsyad/Documents/jawabansoal3b (copy).png
INFO::200501-00:26:12::LS::/home/irsyad/Documents/jawabansoal3b (copy).png
WARNING::200501-00:26:13::REMOVE::/home/irsyad/Documents/jawabansoal3b (copy).png
INFO::200501-00:26:13::LS::/home/irsyad/Documents
INFO::200501-00:26:13::LS::/home/irsyad/Documents/jawabansoal3b (copy).png
INFO::200501-00:26:15::LS::/home/irsyad/Documents
```
Menaruh log ```INFO``` di fungsi ```MKDIR``` dan lainnya
Pada fungsi ```MKDIR``` diatas mendeklarasikan array ```desc```, di bagian ```sprintf``` melakukan penggabungan dimana fpath yang berformat ```%s``` (berisi nama file) digabung dengan ```WRITE```, hasil dari penggabungan tersebut akan disimpan di variabel ```desc```. Kemudian memanggil fungsi ```writeLog``` untuk mempassing log pada parameter yang berupa ```INFO``` serta parameter kedua yaitu hasil concate format ```WRITE``` dan nama path file yang disimpan di dalam array desc. Fungsi ```MKDIR``` tersebut tercatat pada file yang bernama ```fs.log``` jika kita membaca isi.
```c
char desc[100];
  sprintf(desc, "MKDIR::%s", fpath);
  writeLog("INFO", desc);
```
Fungsi ```log_path``` berfungsi untuk menyimpan nama path file yang akan digunakan untuk membuat file ```fs.log```.
```c
char *log_path = "/home/irsyad/fs.log";
```
Pada fungsi ```writeLog``` diatas menerima 2 parameter, yaitu level berisi string level log ```WARNING``` dan ```INFO``` serta ```cmd_desc``` yang berisik syscall dan nama path file. Pertama kita deklarasikan variabel ```*fp``` bertipe file, fp berisi fungsi ```fopen``` yang berfungsi menerima 2 parameter. Parameter pertama ialah ```log_path```  yag berisi path file dari ```fs.log``` yang akan dibuat. Parameter kedua berisi option file ```a+``` untuk ditujukan di parameter pertama. Yang dimaksudkan untuk membuka file untuk membaca dan menambahkan isi ke dalam file (menambahkan tulisan di akhir file), file dibuat jika belum ada. Fungsi ```writeLog``` akan dipanggil dalam semua fungsi fuse (kecuali getattr dan readdir). 
```c
void writeLog(char *level, char *cmd_desc)
{
  FILE * fp;
  fp = fopen (log_path, "a+");

  time_t rawtime = time(NULL);
  
  struct tm *info = localtime(&rawtime);
  
  char time[100];
  strftime(time, 100, "%y%m%d-%H:%M:%S", info);

  char log[100];
  sprintf(log, "%s::%s::%s\n", level, time, cmd_desc);
  fputs(log, fp);

  fclose(fp);
}
```
Hasil eksekusi program:
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal4/soal4.png "Hasil Soal 4")
#
_**Kendala:**_
