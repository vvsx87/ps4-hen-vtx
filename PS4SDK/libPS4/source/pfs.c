// Copyright (C) 2013       Hykem <hykem@hotmail.com>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/gpl-2.0.txt

#include "file.h"
#include "libc.h"

#include "pfs.h"

int pfs;
size_t pfs_size, pfs_copied;
struct pfs_header_t *header;
struct di_d32 *inodes;

#define BUFFER_SIZE 0x100000

char *copy_buffer;

void memcpy_to_file(const char *fname, uint64_t ptr, uint64_t size) {
  int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd != -1) {
    size_t ix = 0;
    while (size > 0) {
      size_t bytes = (size > BUFFER_SIZE) ? BUFFER_SIZE : size;
      lseek(pfs, ptr + ix * BUFFER_SIZE, SEEK_SET);
      read(pfs, copy_buffer, bytes);
      write(fd, copy_buffer, bytes);
      size -= bytes;
      ix++;
      pfs_copied += bytes;
      if (pfs_copied > pfs_size) {
        pfs_copied = pfs_size;
      }
    }
    close(fd);
  }
}

static void parse_directory(int ino, int lev, char *parent_name, bool dry_run) {
  for (uint32_t z = 0; z < inodes[ino].blocks; z++) {
    uint32_t db = inodes[ino].db[0] + z;
    uint64_t pos = (uint64_t)header->blocksz * db;
    uint64_t size = inodes[ino].size;
    uint64_t top = pos + size;

    while (pos < top) {
      struct dirent_t *ent = malloc(sizeof(struct dirent_t));
      if (ent == NULL) {
        return;
      }

      lseek(pfs, pos, SEEK_SET);
      read(pfs, ent, sizeof(struct dirent_t));

      if (ent->type == 0) {
        free(ent);
        break;
      }

      char *name = malloc(ent->namelen + 1);
      if (name == NULL) {
        free(ent);
        return;
      }
      memset(name, 0, ent->namelen + 1);
      if (lev > 0) {
        lseek(pfs, pos + sizeof(struct dirent_t), SEEK_SET);
        read(pfs, name, ent->namelen);
      }

      char *fname;
      if (parent_name != NULL) {
        fname = malloc(strlen(parent_name) + ent->namelen + 2);
        if (fname == NULL) {
          free(ent);
          free(name);
          return;
        }
        sprintf(fname, "%s/%s", parent_name, name);
      } else {
        fname = malloc(ent->namelen + 2);
        if (fname == NULL) {
          free(ent);
          free(name);
          return;
        }
        sprintf(fname, "%s", name);
      }

      if ((ent->type == 2) && (lev > 0)) {
        if (dry_run) {
          pfs_size += inodes[ent->ino].size;
        } else {
          memcpy_to_file(fname, (uint64_t)header->blocksz * inodes[ent->ino].db[0], inodes[ent->ino].size);
        }
      } else if (ent->type == 3) {
        mkdir(fname, 0777);
        parse_directory(ent->ino, lev + 1, fname, dry_run);
      }

      pos += ent->entsize;

      free(ent);
      free(name);
      free(fname);
    }
  }
}

int unpfs(char *pfsfn, char *tidpath) {
  if (tidpath == NULL) {
    return -1;
  }

  copy_buffer = malloc(BUFFER_SIZE);
  if (copy_buffer == NULL) {
    return -1;
  }

  mkdir(tidpath, 0777);

  pfs = open(pfsfn, O_RDONLY, 0);
  if (pfs < 0) {
    free(copy_buffer);
    return -1;
  }

  header = malloc(sizeof(struct pfs_header_t));
  if (header == NULL) {
    free(copy_buffer);
    close(pfs);
    return -1;
  }
  lseek(pfs, 0, SEEK_SET);
  read(pfs, header, sizeof(struct pfs_header_t));

  inodes = malloc(sizeof(struct di_d32) * header->ndinode);
  if (inodes == NULL) {
    free(copy_buffer);
    close(pfs);
    free(header);
    return -1;
  }

  uint32_t ix = 0;

  for (uint32_t i = 0; i < header->ndinodeblock; i++) {
    for (uint32_t j = 0; (j < (header->blocksz / sizeof(struct di_d32))) && (ix < header->ndinode); j++) {
      lseek(pfs, (uint64_t)header->blocksz * (i + 1) + sizeof(struct di_d32) * j, SEEK_SET);
      read(pfs, &inodes[ix], sizeof(struct di_d32));
      ix++;
    }
  }

  pfs_size = 0;
  pfs_copied = 0;

  parse_directory(header->superroot_ino, 0, tidpath, 1);
  parse_directory(header->superroot_ino, 0, tidpath, 0);

  free(copy_buffer);
  free(header);
  close(pfs);
  free(inodes);

  return 0;
}
