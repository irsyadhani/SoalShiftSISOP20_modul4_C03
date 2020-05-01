# SoalShiftSISOP20_modul4_C03
Kelompok C03:
* _Irsyadhani Dwi Shubhi (0511184000022)_
* _Reihan Nanda Ramadhan (05111840000132)_

----------------------------------------------------------------
## Soal
* [Soal 1](#soal-1-enkripsi-versi-1)
* [Soal 2](#soal-2-enkripsi-versi-2)
* [Soal 3](#soal-3-sinkronisasi-direktori-otomatis)
* [Soal 4](#soal-4-log-system)
----------------------------------------------------------------
# Soal 1 (Enkripsi Versi 1)
   _**Soal:**_
* a. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut
akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
* b. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut
akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
* c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi
adirektori tersebut akan terdekrip.
* d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan
tercatat ke sebuah database/log berupa file.
* e. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher
dengan key.
#
   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)
#
_**Penyelesaian:**_
Pertama, kita membuat fungsi main yang akan memanggil fungsi ```fuse_main``` . Pada parameter ketiga ```fuse_main``` akan memanggil ```&xmp_oper```.
```c
int main(int argc, char *argv[])
{
  umask(0);
  return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
Kedua, pada ```struct fuse_operations xmp_oper``` yang dipanggil dari ```fuse_main``` , berisi semua fungsi-fungsi yang akan diimplementasikan pada fuse.
```c
static struct fuse_operations xmp_oper = {
  .getattr  = xmp_getattr,
  .readdir  = xmp_readdir,
  .mknod    = xmp_mknod,
  .mkdir    = xmp_mkdir,
  .unlink   = xmp_unlink,
  .rmdir    = xmp_rmdir,
  .rename   = xmp_rename,
  .truncate = xmp_truncate,
  .open     = xmp_open,
  .read     = xmp_read,
  .write    = xmp_write,
};
```
Selanjutnya, kita harus mendefinisikan direktori yang akan di-mount.
```c
static  const  char *dirpath = "/home/irsyad/Documents";
```
Kemudian, karakter-karakter caesar chiper dideklarasikan pada variabel kode
```c
char kode[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";
```
Lalu, pada program fuse fungsi pertama yang dijalankan terdapat ```getattr```, yang berfungsi mengambil nama file atau path file yang ada pada direktori yang selanjutnya di-mount ```dirpath =/home/irsyad/Documents```.
Pada fungsi ```getattr```parameter pertama ```const char *path``` akan berisi sebuah nama file atau nama path file dari mount direkotori. Contoh, ```/home/irsyad/Documents``` terdapat file yang bernamakan ```soal.txt```, maka pathtersebut berisi nama file yang diawali dengan tanda ```/``` slash, sehuingga isi dari path ialah ```/soal.txt```. Selain itu jika isi dari ```/home/irsyad/Documents```terdapat folder ```dokumen``` yang didalamnya berisi ```soal.txt```, maka path nya adalah path file tersebut, maka path akan berisi ```/dokumen/soal.txt```.
Selanjutnya, path tersebut di-copy isinya ke array temp. dilakukan pemeriksaan dengan funsgi strncmp yang berfungsi membandingkan jumlah karakter tertentu, yakni parameter ketiga, perumpamaannya membandingkan isi dari path (bisa berisi lebih dari 7 karakter karena sebuah nama path file atau file) dengan kalimat ```/encv1_``` sampai dengan 7 karakter, jika sama, maka mount direktori terdapat sebuah folder dengan awalan ```encv1_```, sehingga folder berisi enkrip dari hasil enkripsi versi 1. Agar semua file di-mount direktori muncul dalam nama asli tidak akan terenkrip... Maka harus didekrip semua dile pada folder yang mengandung nama ```encv1_```. Fungsi  dekrip akan diimplementasikan pada semua fungsi yang ada pada struct fuse operations. Pada fungsi dekrip 1 akan menerima 2 parameter. Pertama temp yang berisi copy an dari path berupa nama file atau file, kedua adalah key dari caesar chiper sebanyak 10 key (sesuai soal)
```c
static  int  xmp_getattr(const char *path, struct stat *stbuf){
  int res;
  char temp[1000];

  strcpy(temp, path);

  if(strncmp(path, "/encv1_", 7) == 0)
    dekrip1(temp, 10);

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, temp);
    printf("ini getattr, path nya : %s, fpathnya : %s\n", path,fpath);

  res = lstat(fpath, stbuf);

  if(res == -1)
    return -errno;

  return 0;
}
```
Pada fungsi ```enkrip1``` menerima 2 parameter, yaitu path yang berisi nama path file dan key dari caesar chipper sebanyak 10. Variabel panjang akan berisi panjang string dari path yang diterima. Misal, path yang akan diterima adalah ```/soal.txt``` maka ```strlen(path)``` adalah 9. Lalu masuk ke looping pertama, yang akan mengecek nama path dari karakter paling belakang. Dilakukan 2 pengecekan yaiu jika menemukan tanda ```/``` pada nama path berfungsi untuk looping langsung berhenti dan beralih ke loop berikutnya untuk mencari start dan menandai awal dimulainya karakter-karakter yang akan dienkrip dan mengecek menemukan tanda ```.``` pada nama path yang berfungsi untuk mencari ekstensi dari nama path, agar ekstensi tidak ikut terenrkrip. 
```c
void enkrip1(char *path, int key)
{
  int panjang = strlen(path);
  int start = 0;

  for (int i = strlen(path); i >= 0; i--)
  {
    if(path[i] == '/')
      break;

    if(path[i] == '.')
    {
      panjang = i - 1;
      break;
    }
  }
```
Jika ```path[i]='/'```, maka start akan dimulai dari karakter tersebut untuk proses enkripsi. DI dalam ```for``` selanjutnya ika nama file ditemukan karakter ```/``` maaka continue (diabaikan) atau karakter ```/``` tidak akan terenkripsi. Metode enkripsi dan dekripsi bersifat rekursif untuk subfolder-subfolder yang terdapat didalamnya. Metode dekripsi penjelasannya hampir mirip dengan enkripsi yang ,menjadi pembeda karakter-karakternya akan di-replace dengan ```index-key(10)```. 
```c
for (int i = 1; i < panjang; i++)
    if (path[i] == '/')
      start = i;

  for (int i = start; i <= panjang; i++)
  {
    if(path[i] == '/')
      continue;

    int index = 0;
    while(1)
    {
      if(path[i] == kode[index])
      {
        //int index = (index + key) % strlen(kode);
        //path[i] = kode[index];
        path[i] = kode[index + key];
        break;
      }
      index++;
    }
  }
}
```
Bagian kode tersebut berada di dalam fungsi ```readir```, yang mana ```de->d_name``` akan mendapatkan nama path file atau file yang selanjutnya di-copy ke variabel tmp. Maka dilakukanlah pengecekan jika path file atau file dibandingkan 7 karakter pertama yang sama  dengan ```/encv1_``` maka akan dilakukan metode enkripsi versi 1 dengan memanggil funsgi  ```enkrip1(temp, 10)```, ```temp``` berisi nama path file atau file. Selanjutnya fungsi ```filler``` yang ```tmp``` telah terenkrip akan diisi di dalam direktori mount.
```c
char tmp[1000];
    strcpy(tmp, de->d_name);
    if(strncmp(path, "/encv1_", 7) == 0)
      enkrip1(tmp, 10);

    res = (filler(buf, tmp, &st, 0));
```
Fungsi implemetasi pada fuse:
```c
.getattr  = mendapatkan atribut file atau nama path file
.readdir  = membaca direktori
.mknod    = membuat file
.mkdir    = membuat direktori
.unlink   = meremove file
.rmdir    = meremove direktori
.rename   = mengubah nama file atau direktori
.truncate = mengedit file atau mengecilkan size file
.open     = membuka file atau direktori
.read     = membaca file atau direktori
.write    = menulis ke file
```
Hasil eksekusi program:
Pertama, folder path ```/home/irsyad/Document``` masih belum di-mount.
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal1/soal1_1.png "Hasil Soal 1_1")
Kedua, folder path ```/home/irsyad/Document``` masih sudah di-mount.
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal1/soal1_2.png "Hasil Soal 1_2")
Ketiga, file terenkrip di folder ```encv1_satu```.
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal1/soal1_3.png "Hasil Soal 1_3")
Keempat, rename nama folder biasa tanpa ```encv1_```.
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal1/soal1_4.png "Hasil Soal 1_4")
Kelima, file di dalam folder ```satu``` terdekrip.
![alt text](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/gambar/soal1/soal1_5.png "Hasil Soal 1_5")
# Soal 2 (Enkripsi Versi 2)
   _**Soal:**_
Enkripsi Versi 2
   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
#
# Soal 3 (Sinkronisasi Direktori Otomatis)
   _**Soal:**_
Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:

   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
Belum tahu cara menyinkronisasi direktori
#
# Soal 4 (Log System)
 _**Soal:**_
* a.	Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
* b.	Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
* c.	Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
* d.	Sisanya, akan dicatat dengan level INFO.
* e.	Format untuk logging yaitu: ```[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]```
#
   > Source Code: [ssfs.c](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/ssfs.c)
#
_**Penyelesaian:**_
Soal nomor 4 meminta untuk membuat log yang fungsinya untuk melakukan pencatatan, format pada file system-nya adalah ```[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]```. Terdapat 2 parameter log yaitu : ```WARNING``` untuk mencatat setiap systemcall rmdir serta unlink dan ```INFO``` untuk selain itu. Contoh isi dari log tersebut, yaitu:
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
  char desc[128];
  sprintf(desc, "READ::%s", fpath);
  writeLog("INFO", desc);
```
Fungsi ```log_path``` berfungsi untuk menyimpan nama path file yang akan digunakan untuk membuat file ```fs.log```.
```c
static const char *logpath = "/home/irsyad/fs.log";
```
Pada fungsi ```writeLog``` diatas menerima 2 parameter, yaitu level berisi string level log ```WARNING``` dan ```INFO``` serta ```cmd_desc``` yang berisik syscall dan nama path file. Pertama kita deklarasikan variabel ```*fp``` bertipe file, fp berisi fungsi ```fopen``` yang berfungsi menerima 2 parameter. Parameter pertama ialah ```log_path```  yag berisi path file dari ```fs.log``` yang akan dibuat. Parameter kedua berisi option file ```a+``` untuk ditujukan di parameter pertama. Yang dimaksudkan untuk membuka file untuk membaca dan menambahkan isi ke dalam file (menambahkan tulisan di akhir file), file dibuat jika belum ada. Fungsi ```writeLog``` akan dipanggil dalam semua fungsi fuse (kecuali getattr dan readdir). 
```c
void writeLog(char *level, char *cmd_desc)
{
  FILE * fp;
  fp = fopen (logpath, "a+");

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
