#include <assert.h>

#include "ps4.h"
#include "defines.h"
#include "debug.h"

#define kernel_printf(format, ...) (void)0

#define PS4_UPDATE_FULL_PATH "/update/PS4UPDATE.PUP"

const uint8_t payload_data_const[] =
{
#include "payload_data.inc"
};

int do_patch();

int install_payload(struct thread *td, struct install_payload_args* args)
{
  uint64_t cr0;
  typedef uint64_t vm_offset_t;
  typedef uint64_t vm_size_t;
  typedef void* vm_map_t;

  void* (*kernel_memcpy)(void* dst, const void* src, size_t len);
  vm_offset_t (*kmem_alloc)(vm_map_t map, vm_size_t size);

  uint8_t* kernel_base = (uint8_t*)(__readmsr(0xC0000082) - 0x30EB30);

  *(void**)(&kernel_memcpy) = &kernel_base[0x286CF0];
  *(void**)(&kmem_alloc) = &kernel_base[0x369500];
  vm_map_t kernel_map = *(void**)&kernel_base[0x1FE71B8];

  kernel_printf("\n\n\n\npayload_installer: starting\n");
  kernel_printf("payload_installer: kernel base=%lx\n", kernel_base);

  if (!args->payload_info)
  {
    kernel_printf("payload_installer: bad payload info\n");
    return -1;
  }

  uint8_t* payload_data = args->payload_info->buffer;
  size_t payload_size = args->payload_info->size;
  struct payload_header* payload_header = (struct payload_header*)payload_data;

  if (!payload_data ||
      payload_size < sizeof(payload_header) ||
      payload_header->signature != 0x5041594C4F414433ull)
  {
    kernel_printf("payload_installer: bad payload data\n");
    return -2;
  }

  int desired_size = (payload_size + 0x3FFFull) & ~0x3FFFull; // align size

  // TODO(idc): clone kmem_alloc instead of patching directly
  cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);
  kernel_base[0x36958D] = 7;
  kernel_base[0x3695A5] = 7;
  writeCr0(cr0);

  kernel_printf("payload_installer: kmem_alloc\n");
  uint8_t* payload_buffer = (uint8_t*)kmem_alloc(kernel_map, desired_size);
  if (!payload_buffer)
  {
    kernel_printf("payload_installer: kmem_alloc failed\n");
    return -3;
  }

  // TODO(idc): clone kmem_alloc instead of patching directly
  cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);
  kernel_base[0x36958D] = 3;
  kernel_base[0x3695A5] = 3;
  writeCr0(cr0);

  kernel_printf("payload_installer: installing...\n");
  kernel_printf("payload_installer: target=%lx\n", payload_buffer);
  kernel_printf("payload_installer: payload=%lx,%lu\n",
    payload_data, payload_size);

  kernel_printf("payload_installer: memcpy\n");
  kernel_memcpy((void*)payload_buffer, payload_data, payload_size);

  kernel_printf("payload_installer: patching payload pointers\n");
  if (payload_header->real_info_offset != 0 &&
    payload_header->real_info_offset + sizeof(struct real_info) <= payload_size)
  {
    struct real_info* real_info =
      (struct real_info*)(&payload_data[payload_header->real_info_offset]);
    for (
      ; real_info->payload_offset != 0 && real_info->kernel_offset != 0
      ; ++real_info)
    {
      uint64_t* payload_target =
        (uint64_t*)(&payload_buffer[real_info->payload_offset]);
      void* kernel_target = &kernel_base[real_info->kernel_offset];
      *payload_target = (uint64_t)kernel_target;
      kernel_printf("  %x(%lx) = %x(%lx)\n",
        real_info->payload_offset, payload_target,
        real_info->kernel_offset, kernel_target);
    }
  }

  kernel_printf("payload_installer: patching caves\n");
  if (payload_header->cave_info_offset != 0 &&
    payload_header->cave_info_offset + sizeof(struct cave_info) <= payload_size)
  {
    struct cave_info* cave_info =
      (struct cave_info*)(&payload_data[payload_header->cave_info_offset]);
    for (
      ; cave_info->kernel_call_offset != 0 &&
        cave_info->kernel_ptr_offset != 0 &&
        cave_info->payload_offset != 0
      ; ++cave_info)
    {
      uint8_t* kernel_call_target = &kernel_base[cave_info->kernel_call_offset];
      uint8_t* kernel_ptr_target = &kernel_base[cave_info->kernel_ptr_offset];
      void* payload_target = &payload_buffer[cave_info->payload_offset];
      int32_t new_disp = (int32_t)(kernel_ptr_target - &kernel_call_target[6]);

      if (&kernel_call_target[6] == kernel_ptr_target)
      {
        kernel_printf("  %lx(%lx) = %d\n",
          cave_info->kernel_call_offset, kernel_call_target,
          new_disp);

        if ((uint64_t)(kernel_ptr_target - &kernel_call_target[6]) != 0)
        {
          kernel_printf("  error: new_disp != 0!\n");
        }
      }
      else
      {
        kernel_printf("  %lx(%lx) -> %lx(%lx) = %d\n",
          cave_info->kernel_call_offset, kernel_call_target,
          cave_info->kernel_ptr_offset, kernel_ptr_target,
          new_disp);

        if ((uint64_t)(kernel_ptr_target - &kernel_call_target[6]) > UINT32_MAX)
        {
          kernel_printf("  error: new_disp > UINT32_MAX!\n");
        }
      }
      kernel_printf("    %lx(%lx)\n",
        cave_info->payload_offset, payload_target);

#pragma pack(push,1)
      struct
      {
        uint8_t op[2];
        int32_t disp;
      }
      jmp;
#pragma pack(pop)
      jmp.op[0] = 0xFF;
      jmp.op[1] = 0x25;
      jmp.disp = new_disp;
      cr0 = readCr0();
      writeCr0(cr0 & ~X86_CR0_WP);
      kernel_memcpy(kernel_call_target, &jmp, sizeof(jmp));
      kernel_memcpy(kernel_ptr_target, &payload_target, sizeof(void*));
      writeCr0(cr0);
    }
  }

  kernel_printf("payload_installer: patching calls\n");
  if (payload_header->disp_info_offset != 0 &&
    payload_header->disp_info_offset + sizeof(struct disp_info) <= payload_size)
  {
    struct disp_info* disp_info =
      (struct disp_info*)(&payload_data[payload_header->disp_info_offset]);
    for (
      ; disp_info->call_offset != 0 && disp_info->cave_offset != 0
      ; ++disp_info)
    {
      uint8_t* cave_target = &kernel_base[disp_info->cave_offset];
      uint8_t* call_target = &kernel_base[disp_info->call_offset];

      int32_t new_disp = (int32_t)(cave_target - &call_target[5]);

      kernel_printf("  %lx(%lx)\n",
        disp_info->call_offset + 1, &call_target[1]);
      kernel_printf("    %lx(%lx) -> %lx(%lx) = %d\n",
        disp_info->call_offset + 5, &call_target[5],
        disp_info->cave_offset, cave_target,
        new_disp);

      cr0 = readCr0();
      writeCr0(cr0 & ~X86_CR0_WP);
      *((int32_t*)&call_target[1]) = new_disp;
      writeCr0(cr0);
    }
  }

  if (payload_header->entrypoint_offset != 0 &&
    payload_header->entrypoint_offset < payload_size)
  {
    kernel_printf("payload_installer: entrypoint\n");
    void (*payload_entrypoint)();
    *((void**)&payload_entrypoint) =
      (void*)(&payload_buffer[payload_header->entrypoint_offset]);
    payload_entrypoint();
  }

  kernel_printf("payload_installer: done\n");
  return 0;
}

int kernel_payload(struct thread *td, struct kernel_payload_args* args)
{
  struct ucred* cred;
  struct filedesc* fd;

  uint64_t (*sceRegMgrSetInt)(uint32_t regId, int value);

  fd = td->td_proc->p_fd;
  cred = td->td_proc->p_ucred;

  void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-0x30EB30];
  uint8_t* kernel_ptr = (uint8_t*)kernel_base;
  void** got_prison0 =   (void**)&kernel_ptr[0xF26010];
  void** got_rootvnode = (void**)&kernel_ptr[0x206D250];
  *(void**)(&sceRegMgrSetInt) = &kernel_ptr[0x4CEAB0];

  cred->cr_uid = 0;
  cred->cr_ruid = 0;
  cred->cr_rgid = 0;
  cred->cr_groups[0] = 0;

  cred->cr_prison = *got_prison0;
  fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

  // escalate ucred privs, needed for access to the filesystem ie* mounting & decrypting files
  void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred
	
  // sceSblACMgrIsSystemUcred
  uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
  *sonyCred = 0xffffffffffffffff;
	
  // sceSblACMgrGetDeviceAccessType
  uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
  *sceProcType = 0x3801000000000013; // Max access
	
  // sceSblACMgrHasSceProcessCapability
  uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
  *sceProcCap = 0xffffffffffffffff; // Sce Process

  // enable permanent Internet Web Browser
  sceRegMgrSetInt(0x3C040000, 0);
  
  // Disable write protection
  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  // specters debug settings patches
  *(char *)(kernel_base + 0x2001516) |= 0x14;
  *(char *)(kernel_base + 0x2001539) |= 3;
  *(char *)(kernel_base + 0x200153A) |= 1;
  *(char *)(kernel_base + 0x2001558) |= 1; 

  // sealabs debug menu full patches
  *(uint32_t *)(kernel_base + 0x4CECB7) = 0;
  *(uint32_t *)(kernel_base + 0x4CFB9B) = 0;

  // target_id patches
  *(uint16_t *)(kernel_base + 0x1FE59E4) = 0x8101;
  *(uint16_t *)(kernel_base + 0X1FE5A2C) = 0x8101;
  *(uint16_t *)(kernel_base + 0x200151C) = 0x8101;

  // flatz disable RSA signature check for PFS
  *(uint32_t *)(kernel_base + 0x68E990) = 0x90C3C031;

  // flatz enable debug RIFs
  *(uint64_t *)(kernel_base + 0x6215B4) = 0x812EEB00000001B8;	 

  // Restore write protection
  writeCr0(cr0);

  return 0;
}

static inline void patch_update(void)
{
  DIR* directory = opendir(PS4_UPDATE_FULL_PATH);

  if(directory != NULL)
  {
    closedir(directory);
    return;
  }

  unlink(PS4_UPDATE_FULL_PATH);
  mkdir(PS4_UPDATE_FULL_PATH, 0777);
}

int _main(struct thread *td) {
  int result;

  initKernel();	
  initLibc();

#ifdef DEBUG_SOCKET
  initNetwork();
  initDebugSocket();
#endif

  printfsocket("Starting...\n");

  result = kexec(&kernel_payload, NULL);
  printfsocket("kernel_payload: %d\n", result);
  if (result) goto exit;

  patch_update();

  result = errno = do_patch();
  printfsocket("do_patch: %d\n", result);
  if (result) goto exit;

  uint8_t* payload_data = (uint8_t*)(&payload_data_const[0]);
  size_t payload_size = sizeof(payload_data_const);
  struct payload_info payload_info;
  payload_info.buffer = payload_data;
  payload_info.size = payload_size;
  errno = 0;

  result = kexec(&install_payload, &payload_info);
  result = !result ? 0 : errno;
  printfsocket("install_payload: %d\n", result);
  if (result) goto exit;

exit:
  printfsocket("Done.\n");

#ifdef DEBUG_SOCKET
  closeDebugSocket();
#endif

  return !result ? 0 : errno;
}
