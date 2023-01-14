#include "kernel.h"
#include "libc.h"
#include "module.h"
#include "syscall.h"

#include "sysutil.h"

int sysUtilHandle;
int libSceSystemService;
int libSceUserService;

int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char *message);
int (*sceSystemServiceLaunchWebBrowser)(const char *uri, void *);
int (*sceUserServiceInitialize)(void *);
int (*sceUserServiceGetLoginUserIdList)(SceUserServiceLoginUserIdList *);
int (*sceUserServiceGetUserName)(int32_t userId, char *userName, const size_t size);
int (*sceUserServiceGetInitialUser)(int32_t *);
int (*sceUserServiceTerminate)();
int (*sceKernelReboot)();

void initSysUtil(void) {
  if (!sysUtilHandle) {
    sysUtilHandle = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, 0, 0, NULL, NULL);

    RESOLVE(sysUtilHandle, sceSysUtilSendSystemNotificationWithText);
  }

  if (!libSceSystemService) {
    libSceSystemService = sceKernelLoadStartModule("/system/common/lib/libSceSystemService.sprx", 0, 0, 0, NULL, NULL);

    RESOLVE(libSceSystemService, sceSystemServiceLaunchWebBrowser);
  }
}

void initUserService(void) {
  if (libSceUserService) {
    return;
  }

  libSceUserService = sceKernelLoadStartModule("/system/common/lib/libSceUserService.sprx", 0, 0, 0, NULL, NULL);

  RESOLVE(libSceUserService, sceUserServiceInitialize);
  RESOLVE(libSceUserService, sceUserServiceGetInitialUser);
  RESOLVE(libSceUserService, sceUserServiceGetLoginUserIdList);
  RESOLVE(libSceUserService, sceUserServiceGetUserName);
  RESOLVE(libSceUserService, sceUserServiceTerminate);
}

void openBrowser(char *uri) {
  sceSystemServiceLaunchWebBrowser(uri, NULL);
}

int getUserIDList(SceUserServiceLoginUserIdList *userIdList) {
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetLoginUserIdList(userIdList) == 0) {
      sceUserServiceTerminate();
      return 0;
    }
  }
  return -1;
}

int32_t getUserID() {
  SceUserServiceLoginUserIdList userIdList;
  getUserIDList(&userIdList);
  for (int i = 0; i < 1; i++) {
    if (userIdList.userId[i] != -1) {
      return userIdList.userId[i];
    }
  }
  return -1;
}

char *getUserName(int32_t userId) {
  char *retval = malloc(SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1);
  if (retval == NULL) {
    return NULL;
  }
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    char username[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
    if (sceUserServiceGetUserName(userId, username, sizeof(username)) == 0) {
      strcpy(retval, username);
      sceUserServiceTerminate();
      return retval;
    }
  }
  free(retval);
  return NULL;
}

int32_t getInitialUser() {
  int32_t userId;
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetInitialUser(&userId) == 0) {
      sceUserServiceTerminate();
      return userId;
    }
  }
  return -1;
}

void shutdown() {
  int evf = syscall(540, "SceSysCoreReboot");
  syscall(546, evf, 0x4000, 0);
  syscall(541, evf);
  syscall(37, 1, 30);
}

void reboot() {
  int libkernel = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, 0, 0, NULL, NULL);
  RESOLVE(libkernel, sceKernelReboot);
  sceKernelReboot();
}

/*uint16_t ret;                    // Numerical representation of the firmware version. ex: 505 for 5.05, 702 for 7.02, etc
  uint32_t offset;                 // Offset for firmware's version location
  uint16_t elf_header_size;        // ELF header size
  uint16_t elf_header_entry_size;  // ELF header entry size
  uint16_t num_of_elf_entries;     // Number of entries in the ELF header
  char binary_fw[2] = {0};         // 0x0000
  char string_fw[5] = {0};         // "0000\0"
  char sandbox_path[33];           // `/XXXXXXXXXX/common/lib/libc.sprx` [Char count of 32 + nullterm]

  snprintf_s(sandbox_path, sizeof(sandbox_path), "/%s/common/lib/libc.sprx", sceKernelGetFsSandboxRandomWord());
  int fd = open(sandbox_path, O_RDONLY, 0);
  if (fd < 0) {
    // Assume it's currently jailbroken
    fd = open("/system/common/lib/libc.sprx", O_RDONLY, 0);
    if (fd < 0) {
      // It's really broken
      return -1;
    }
  }

  lseek(fd, 0x154, SEEK_SET);
  read(fd, &elf_header_size, sizeof(elf_header_size));
  lseek(fd, 0x156, SEEK_SET);
  read(fd, &elf_header_entry_size, sizeof(elf_header_entry_size));
  lseek(fd, 0x158, SEEK_SET);
  read(fd, &num_of_elf_entries, sizeof(num_of_elf_entries));
  offset = elf_header_size + num_of_elf_entries * elf_header_entry_size;

  // Align
  while (offset % 0x10 != 0) {
    offset += 1;
  }

  // 0x120 is the size of the header on encrypted ELFs, 0x14 is the location of the version after the ELF headers
  offset += 0x120 + 0x14;

  lseek(fd, offset, SEEK_SET);
  read(fd, &binary_fw,  sizeof(binary_fw));
  close(fd);

  snprintf_s(string_fw, sizeof(string_fw), "%02x%02x", binary_fw[1], binary_fw[0]);

  ret = atoi(string_fw);

  g_firmware = ret;
  return ret;
}*/