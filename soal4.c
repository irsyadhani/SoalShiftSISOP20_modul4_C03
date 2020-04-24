#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

static const char *dirpath = "/home/umum/Documents/SisOpLab/FUSE";

void getFileName(char fname[], char fext[], char *fpath) {
  char fcopy[1000];
  char fnameext[1000];

  strcpy(fcopy, fpath);
  char *token = strtok(fcopy, "/");
  while (token != NULL) {
    sprintf(fnameext, "%s", token);
    token = strtok(NULL, "/");
  }

  strcpy(fcopy, fnameext);
  int count = 0;
  token = strtok(fcopy, ".");
  while (token != NULL) {
    count++;
    token = strtok(NULL, ".");
  }

  if (count <= 1) {
    strcpy(fname, fnameext);
    strcpy(fext, "");
  } else {
    strcpy(fcopy, fnameext);
    int count2 = 0;
    token = strtok(fcopy, ".");
    while (token != NULL) {
      if (++count2 == count) {
        strcpy(fext, token);
      } else {
        sprintf(fname, "%s%s.", fname, token);
      }
      token = strtok(NULL, ".");
    }
  }
}


static int _getattr(const char *path, struct stat *stbuf) {
  char fpath[1000];
  if (strcmp(path, "/") == 0) {
    sprintf(fpath, "%s", dirpath);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  int res = lstat(fpath, stbuf);

  if (res == -1) return -errno;

  return 0;
}

static int _readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  char fpath[1000];
  if (strcmp(path, "/") == 0) {
    sprintf(fpath, "%s", dirpath);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  int res = 0;

  DIR *dp;
  struct dirent *de;
  (void)offset;
  (void)fi;

  dp = opendir(fpath);
  if (dp == NULL) return -errno;

  while ((de = readdir(dp)) != NULL) {
    // if (de->d_type == DT_REG) {
      struct stat st;
      memset(&st, 0, sizeof(st));
      st.st_ino = de->d_ino;
      st.st_mode = de->d_type << 12;
      res = (filler(buf, de->d_name, &st, 0));
      if (res != 0) break;
    // }
  }
  closedir(dp);
  return 0;
}



static int _create(const char *path, mode_t mode, struct fuse_file_info *fi) {
  char fpath[1000];
	if (strcmp(path, "/") == 0) {
		sprintf(fpath, "%s", dirpath);
	} else {
		sprintf(fpath, "%s%s", dirpath, path);
	}
	int res;

	res = open(fpath, fi->flags, mode);
	if (res == -1) return -errno;

	fi->fh = res;
	return 0;
}

static int _open(const char *path, struct fuse_file_info *fi) {
	char fpath[1000];
	if (strcmp(path, "/") == 0) {
		sprintf(fpath, "%s", dirpath);
	} else {
		sprintf(fpath, "%s%s", dirpath, path);
	}
	int res;

	res = open(fpath, fi->flags);
	if (res == -1) return -errno;

	fi->fh = res;
	return 0;
}

static int _read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  char fpath[1000];
  if (strcmp(path, "/") == 0) {
    sprintf(fpath, "%s", dirpath);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  int fd;

  if (fi == NULL)
    fd = open(fpath, O_RDONLY);
  else
    fd = fi->fh;

  if (fd == -1) return -errno;
  int res = pread(fd, buf, size, offset);
  if (res == -1) res = -errno;
  if (fi == NULL)
    close(fd);
  return res;
}

static int _write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  char fpath[1000];
  char fname[1000];
  char fext[1000];
  char fpath2[1000];
  if (strcmp(path, "/") == 0) {
    sprintf(fpath, "%s", dirpath);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }
  (void) fi;

  char fbackup[1000];
  sprintf(fbackup, "%s/backup", dirpath);
  int backupfd = open(fbackup, O_RDONLY);
  if (backupfd == -1) {
    mkdir(fbackup, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  } else {
    close(backupfd);
  }

  memset(fname, 0, sizeof(fname));
  memset(fext, 0, sizeof(fext));
  getFileName(fname, fext, fpath);

  time_t timer;
  char ftime[1000];
  struct tm* tm_info;
  timer = time(NULL);
  tm_info = localtime (&timer);
  strftime(ftime, 1000, "%H:%M:%S_%d-%m-%Y", tm_info);

  if (strcmp(fext, "") == 0) {
    sprintf(fpath2, "%s/backup/%s.%s", dirpath, fname, ftime);
  } else {
    sprintf(fpath2, "%s/backup/%s%s.%s", dirpath, fname, ftime, fext);
  }

  int fd1;
  if (fi == NULL)
    fd1 = open(fpath, O_WRONLY);
  else
    fd1 = fi->fh;
  int fd2 = open(fpath2, O_WRONLY | O_CREAT);

  int res = pwrite(fd1, buf, size, offset);
  pwrite(fd2, buf, size, offset);
  chmod(fpath2, 0);
  if (fi == NULL)
    close(fd1);
  close(fd2);

  if (res == -1) res = -errno;
  return res;
}

static struct fuse_operations _oper = {
  .getattr = _getattr,
  .readdir = _readdir,
  .open = _open,
  .create = _create,
  .read = _read,
  .write = _write
};

int main(int argc, char *argv[]) {
  umask(0);
  return fuse_main(argc, argv, &_oper, NULL);
}
