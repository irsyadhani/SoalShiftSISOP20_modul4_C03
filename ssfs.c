#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/irsyad/Documents";
static const char *logpath = "/home/irsyad/fs.log";

char kode[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

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

void dekrip1(char *path, int key){
  int panjang = strlen(path);
  int start = 0;

  for (int i = 1; i < panjang; i++)
  {
    if(path[i] == '/' || path[i + 1] == '\0')
    {
      start = i + 1;
      break;
    }
  }

  for (int i = strlen(path); i >= 0; i--)
  {
    if (path[i] == '.')
    {
      panjang = i - 1;
      break;
    }
  }

  for (int i = start; i <= panjang; i++)
  {
    if(path[i] == '/')
      continue;

    int index = strlen(kode) - 1;
    while(1)
    {
      if(path[i] == kode[index])
      {
        //int index = index - key;
        // while(index < 0) index += strlen(kode);
        // path[i] = kode[index];
        path[i] = kode[index - key];
        break;
      }
      index--;
    }
  }
}

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

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
  printf("ini readdir\n");
  char fpath[1000];
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }

  int res = 0;
  DIR *dp;
  struct dirent *de;

  (void) offset;
  (void) fi;

  dp = opendir(fpath);
  if (dp == NULL)
    return -errno;

  while ((de = readdir(dp)) != NULL) {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
      continue;

    struct stat st;

    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;

    char tmp[1000];
    strcpy(tmp, de->d_name);
    if(strncmp(path, "/encv1_", 7) == 0)
      enkrip1(tmp, 10);

    res = (filler(buf, tmp, &st, 0));

    if(res!=0) break;
  }

  closedir(dp);
  return 0;

}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
  char fpath[1000];
  printf("ini read\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }

  int res = 0;
  int fd = 0 ;

  (void) fi;

  fd = open(fpath, O_RDONLY);

  if (fd == -1)
  return -errno;

  res = pread(fd, buf, size, offset);
  if (res == -1)
  res = -errno;

  close(fd);

  char desc[128];
  sprintf(desc, "READ::%s", fpath);
  writeLog("INFO", desc);

  return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
  char fpath[1000];
  printf("ini write\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

  char desc[128];
  sprintf(desc, "WRITE::%s", fpath);
  writeLog("INFO", desc);

	return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
  char fpath[1000];
  printf("ini open\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }
	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);

  char desc[128];
  sprintf(desc, "OPEN::%s", fpath);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
  char fpath[1000];
  printf("ini truncate\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }
	int res;

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "TRUNC::%s", fpath);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
  char fpath[1000];
  printf("ini mknod\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }

	int res;
	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "CREATE::%s", fpath);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
  char fpath[1000];
  printf("ini mkdir\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }
	int res;

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "MKDIR::%s", fpath);
  writeLog("INFO", desc);


	return 0;
}

static int xmp_rmdir(const char *path)
{
  char fpath[1000];
  printf("ini rmdir\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }
	int res;

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "RMDIR::%s", fpath);
  writeLog("WARNING", desc);

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
  char c_from[1000];
  char c_to[1000];
  printf("ini rename\n");
  if(strcmp(from,"/") == 0){
    from=dirpath;
    sprintf(c_from,"%s",from);
  }else {
    char temp[1000];
    strcpy(temp, from);
    if(strncmp(from, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(c_from, "%s%s",dirpath,temp);
  }

  if(strcmp(to,"/") == 0){
    to=dirpath;
    sprintf(c_to,"%s",to);
  }else {
    char temp[1000];
    strcpy(temp, to);
    if(strncmp(to, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(c_to, "%s%s",dirpath,temp);
  }


	int res;

	res = rename(c_from, c_to);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "RENAME::%s::%s", c_from, c_to);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_unlink(const char *path)
{
  char fpath[1000];
  printf("ini unlink\n");
  if(strcmp(path,"/") == 0){
    path=dirpath;
    sprintf(fpath,"%s",path);
  }else {
    char temp[1000];
    strcpy(temp, path);
    if(strncmp(path, "/encv1_", 7) == 0)
      dekrip1(temp, 10);
    sprintf(fpath, "%s%s",dirpath,temp);
  }
	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

  char desc[128];
  sprintf(desc, "UNLINK::%s", fpath);
  writeLog("WARNING", desc);

	return 0;
}



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

int main(int argc, char *argv[])
{
  umask(0);
  return fuse_main(argc, argv, &xmp_oper, NULL);
}
