#ifndef __OFFSETS_H__
#define __OFFSETS_H__
#pragma once
// 6.72

// data
#define	XFAST_SYSCALL_addr              0x00001C0
#define M_TEMP_addr                     0x1540EB0
#define MINI_SYSCORE_SELF_BINARY_addr   0x156A588
#define ALLPROC_addr                    0x22BBE80
#define SBL_DRIVER_MAPPED_PAGES_addr    0x266AC68
#define SBL_PFS_SX_addr                 0x2679040
#define SBL_KEYMGR_KEY_SLOTS_addr       0x2694570
#define SBL_KEYMGR_KEY_RBTREE_addr      0x2694580
#define SBL_KEYMGR_BUF_VA_addr          0x2698000
#define SBL_KEYMGR_BUF_GVA_addr         0x2698808
#define FPU_CTX_addr                    0x2694080
#define DIPSW_addr                      0x1BD7FD0

// common
#define memcmp_addr                     0x207E40
#define _sx_xlock_addr                  0x426C0
#define _sx_xunlock_addr                0x42880
#define malloc_addr                     0x0D7A0
#define free_addr                       0x0D9A0
#define strstr_addr                     0x4817F0
#define fpu_kern_enter_addr             0x36B6E0
#define fpu_kern_leave_addr             0x36B7D0
#define memcpy_addr                     0x3C15B0
#define memset_addr                     0x1687D0
#define strlen_addr                     0x2433E0
#define printf_addr                     0x123280
#define eventhandler_register_addr      0x402E80

// Fself
#define sceSblACMgrGetPathId_addr       0x233C70
#define sceSblServiceMailbox_addr       0x64CC20
#define sceSblAuthMgrSmIsLoadable2_addr 0x65D7A0
#define _sceSblAuthMgrGetSelfInfo_addr  0x65E010
#define _sceSblAuthMgrSmStart_addr      0x65E490
#define sceSblAuthMgrVerifyHeader_addr  0x660260

// Fpkg
#define RsaesPkcs1v15Dec2048CRT_addr    0x1D6050
#define Sha256Hmac_addr                 0x335B70
#define AesCbcCfb128Encrypt_addr        0x3C0320
#define AesCbcCfb128Decrypt_addr        0x3C0550
#define sceSblDriverSendMsg_0_addr      0x637AE0
#define sceSblPfsSetKeys_addr           0x641520
#define sceSblKeymgrSetKeyStorage_addr  0x646E00
#define sceSblKeymgrSetKeyForPfs_addr   0x649800
#define sceSblKeymgrCleartKey_addr      0x649B80
#define sceSblKeymgrSmCallfunc_addr     0x6493D0

// Patch
#define vmspace_acquire_ref_addr        0x44CB90
#define vmspace_free_addr               0x44C9C0
#define vm_map_lock_read_addr           0x44CD40
#define vm_map_unlock_read_addr         0x44CD90
#define vm_map_lookup_entry_addr        0x44D330
#define proc_rwmem_addr                 0x10EE10

// Fself hooks
#define sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook          0x6591BC
#define sceSblAuthMgrIsLoadable2_hook                               0x65930F
#define sceSblAuthMgrVerifyHeader_hook1                             0x659AC6
#define sceSblAuthMgrVerifyHeader_hook2                             0x65A758
#define sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook   0x66092A
#define sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook     0x661571

// Fpkg hooks
#define sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook         0x646EA5
#define sceSblKeymgrInvalidateKey__sx_xlock_hook                    0x64AA3D
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook      0x669500
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook           0x66A313
#define mountpfs__sceSblPfsSetKeys_hook1                            0x6CDF15
#define mountpfs__sceSblPfsSetKeys_hook2                            0x6CE141

// SceShellUI patches - debug patches
#define sceSblRcMgrIsAllowDebugMenuForSettings_patch                0x1D670
#define sceSblRcMgrIsStoreMode_patch                                0x1D9D0

// SceShellUI patches - remote play patches
#define CreateUserForIDU_patch                                      0x1A0900
#define remote_play_menu_patch                                      0xEC8291

// SceRemotePlay patches - remote play patches
#define SceRemotePlay_patch1                                        0x10C6D4
#define SceRemotePlay_patch2                                        0x10C6EF

// SceShellCore patches
// call sceKernelIsGenuineCEX
#define sceKernelIsGenuineCEX_patch1    0x189602
#define sceKernelIsGenuineCEX_patch2    0x835642
#define sceKernelIsGenuineCEX_patch3    0x880492
#define sceKernelIsGenuineCEX_patch4    0xA12B92

// call nidf_libSceDipsw
#define nidf_libSceDipsw_patch1         0x189630
#define nidf_libSceDipsw_patch2         0x254107
#define nidf_libSceDipsw_patch3         0x835670
#define nidf_libSceDipsw_patch4         0xA12BC0

// enable data mount
#define enable_data_mount_patch         0x33943E

// enable fpkg
#define enable_fpkg_patch               0x3EFCF0
 
// debug pkg free string
#define fake_free_patch                 0xFD2BF1

// make pkgs installer working with external hdd
#define pkg_installer_patch             0x9FB311

// enable support with 6.xx external hdd
#define ext_hdd_patch1                  0x606A0D
#define ext_hdd_patch2                  0x149BF0

// enable debug trophies on retail
#define debug_trophies_patch            0x7268C9

// disable screenshot block
#define disable_screenshot_patch        0x0DD2A6

#endif