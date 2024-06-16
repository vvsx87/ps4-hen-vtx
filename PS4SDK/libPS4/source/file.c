#include "libc.h"
#include "syscall.h"

#include "file.h"

SYSCALL(read, 3);
SYSCALL(write, 4);
SYSCALL(open, 5);
SYSCALL(close, 6);
SYSCALL(unlink, 10);
SYSCALL(link, 9);
SYSCALL(readlink, 58);
SYSCALL(symlink, 57);
SYSCALL(mount, 21);
SYSCALL(nmount, 378);
SYSCALL(unmount, 22);
SYSCALL(fchown, 123);
SYSCALL(fchmod, 124);
SYSCALL(rename, 128);
SYSCALL(mkdir, 136);
SYSCALL(rmdir, 137);
SYSCALL(stat, 188);
SYSCALL(fstat, 189);
SYSCALL(lstat, 190);
SYSCALL(getdents, 272);
SYSCALL(lseek, 478);
SYSCALL(fstatat, 493);

int getSandboxDirectory(char *destination, int *length) {
  return syscall(602, 0, destination, length);
}

int file_exists(char *fname) {
  int file = open(fname, O_RDONLY, 0);
  if (file != -1) {
    close(file);
    return 1;
  }
  return 0;
}

int dir_exists(char *dname) {
  DIR *dir = opendir(dname);
  if (dir) {
    closedir(dir);
    return 1;
  }
  return 0;
}

int symlink_exists(const char *fname) {
  struct stat statbuf;
  if (lstat(fname, &statbuf) < 0) {
    return -1;
  }
  if (S_ISLNK(statbuf.st_mode) == 1) {
    return 1;
  } else {
    return 0;
  }
}

void touch_file(char *destfile) {
  int fd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd != -1) {
    close(fd);
  }
}

void copy_file(char *sourcefile, char *destfile) {
  int src = open(sourcefile, O_RDONLY, 0);
  if (src != -1) {
    int out = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (out != -1) {
      char *buffer = malloc(4194304);
      if (buffer != NULL) {
        size_t bytes;
        while ((bytes = read(src, buffer, 4194304)) > 0) {
          write(out, buffer, bytes);
        }
        free(buffer);
      }
      close(out);
    }
    close(src);
  }
}

void copy_dir(char *sourcedir, char *destdir) {
  DIR *dir = opendir(sourcedir);
  struct dirent *dp;
  struct stat info;
  char src_path[1024];
  char dst_path[1024];

  if (!dir) {
    return;
  }
  mkdir(destdir, 0777);
  while ((dp = readdir(dir)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
      continue;
    } else {
      sprintf(src_path, "%s/%s", sourcedir, dp->d_name);
      sprintf(dst_path, "%s/%s", destdir, dp->d_name);

      if (!stat(src_path, &info)) {
        if (S_ISDIR(info.st_mode)) {
          copy_dir(src_path, dst_path);
        } else if (S_ISREG(info.st_mode)) {
          copy_file(src_path, dst_path);
        }
      }
    }
  }
  closedir(dir);
}

int file_compare(char *fname1, char *fname2) {
  int res = 0;
  int file1 = open(fname1, O_RDONLY, 0);
  int file2 = open(fname2, O_RDONLY, 0);
  char *buffer1 = malloc(65536);
  char *buffer2 = malloc(65536);

  if (file1 && file2 && buffer1 != NULL && buffer2 != NULL) {
    lseek(file1, 0, SEEK_END);
    lseek(file2, 0, SEEK_END);
    long size1 = lseek(file1, 0, SEEK_CUR);
    long size2 = lseek(file2, 0, SEEK_CUR);
    lseek(file1, 0L, SEEK_SET);
    lseek(file2, 0L, SEEK_SET);
    if (size1 == size2) {
      int lastBytes = 100;
      if (size1 < lastBytes) {
        lastBytes = size1;
      }
      lseek(file1, -lastBytes, SEEK_END);
      lseek(file2, -lastBytes, SEEK_END);
      int bytesRead1 = read(file1, buffer1, sizeof(char));
      int bytesRead2 = read(file2, buffer2, sizeof(char));
      if (bytesRead1 > 0 && bytesRead1 == bytesRead2) {
        res = 1;
        for (int i = 0; i < bytesRead1; i++) {
          if (buffer1[i] != buffer2[i]) {
            res = 0;
            break;
          }
        }
      }
    }
  }

  free(buffer1);
  free(buffer2);
  close(file1);
  close(file2);
  return res;
}

int rmtree(const char *path) {
  DIR *d = opendir(path);
  int r = -1;

  if (d) {
    struct dirent *p;

    r = 0;
    while (!r && (p = readdir(d))) {
      int r2 = -1;
      char *buf;
      size_t len;

      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
        continue;
      }

      len = strlen(path) + strlen(p->d_name) + 2;
      buf = malloc(len);

      if (buf) {
        struct stat statbuf;

        snprintf(buf, len, "%s/%s", path, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode)) {
            r2 = rmtree(buf);
          } else {
            r2 = unlink(buf);
          }
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }

  if (!r) {
    r = rmdir(path);
  }

  return r;
}

int fgetc_pointer(int fp) {
  char c;
  if (read(fp, &c, 1) == 0) {
    return (-1);
  }
  return (c);
}

void build_iovec(struct iovec **iov, int *iovlen, const char *name, const void *val, size_t len) {
  int i;
  if (*iovlen < 0) {
    return;
  }
  i = *iovlen;
  *iov = realloc(*iov, sizeof **iov * (i + 2));
  if (*iov == NULL) {
    *iovlen = -1;
    return;
  }
  (*iov)[i].iov_base = strdup(name);
  (*iov)[i].iov_len = strlen(name) + 1;
  ++i;
  (*iov)[i].iov_base = (void *)val;
  if (len == (size_t)-1) {
    if (val != NULL) {
      len = strlen(val) + 1;
    } else {
      len = 0;
    }
  }
  (*iov)[i].iov_len = (int)len;
  *iovlen = ++i;
}

int mount_large_fs(const char *device, const char *mountpoint, const char *fstype, const char *mode, unsigned int flags) {
  struct iovec *iov = NULL;
  int iovlen = 0;
  build_iovec(&iov, &iovlen, "fstype", fstype, -1);
  build_iovec(&iov, &iovlen, "fspath", mountpoint, -1);
  build_iovec(&iov, &iovlen, "from", device, -1);
  build_iovec(&iov, &iovlen, "large", "yes", -1);
  build_iovec(&iov, &iovlen, "timezone", "static", -1);
  build_iovec(&iov, &iovlen, "async", "", -1);
  build_iovec(&iov, &iovlen, "ignoreacl", "", -1);
  if (mode) {
    build_iovec(&iov, &iovlen, "dirmask", mode, -1);
    build_iovec(&iov, &iovlen, "mask", mode, -1);
  }
  return nmount(iov, iovlen, flags);
}
