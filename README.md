# SoalShiftSISOP20_modul4_C03
Kelompok C03:
* _Irsyadhani Dwi Shubhi (0511184000022)_
* _Reihan Nanda Ramadhan (05111840000132)_

----------------------------------------------------------------
## Soal
* [Soal 1](#soal-1-enkripsi-versi-1)
* [Soal 2](#soal-1-enkripsi-versi-2)
* [Soal 3](#soal-3-sinkronisasi-direktori-otomatis)
* [Soal 4](#oal-4-log-system)
----------------------------------------------------------------
# Soal 1 (Enkripsi Versi 1)
   _**Soal:**_
   1. Enkripsi Versi 1
   > Source Code: [Soal 1](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/soal1.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
#
# Soal 2 (Enkripsi Versi 2)
   _**Soal:**_
   2. Enkripsi Versi 2
   > Source Code: [Soal 2](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/soal2.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
#
# Soal 3 (Sinkronisasi Direktori Otomatis)
   _**Soal:**_
   3. Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:
a.	Kedua directory memiliki parent directory yang sama.
b.	Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:
i.	Nama dari setiap berkas di dalamnya sama.
ii.	Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.
c.	Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.
d.	Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory.

Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.
Implementasi dilarang menggunakan symbolic links dan thread.

   > Source Code: [Soal 3](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/soal3.c)

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
e.	Format untuk logging yaitu: [LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]

   > Source Code: [Soal 4](https://github.com/irsyadhani22/SoalShiftSISOP20_modul4_C03/blob/master/soal4.c)

#

_**Penyelesaian:**_

#
_**Kendala:**_
Masih bingung untuk membuat log
