#ifndef __OFFSETS_H__
#define __OFFSETS_H__
#pragma once

// 9.50
// data
#define XFAST_SYSCALL_addr               0x000001C0
#define M_TEMP_addr                      0x01A4ECB0
#define MINI_SYSCORE_SELF_BINARY_addr    0x00D42930
#define ALLPROC_addr                     0x0221D2A0
#define SBL_DRIVER_MAPPED_PAGES_addr     0x0263A6D0
#define SBL_PFS_SX_addr                  0x02658650
#define SBL_KEYMGR_KEY_SLOTS_addr        0x02648B78
#define SBL_KEYMGR_KEY_RBTREE_addr       0x02648B88
#define SBL_KEYMGR_BUF_VA_addr           0x0264C000
#define SBL_KEYMGR_BUF_GVA_addr          0x0264C808
#define FPU_CTX_addr                     0x0263AEC0
#define DIPSW_addr                       0x0221A3D0
                                         
// common                                
#define memcmp_addr                      0x0047CB80
#define _sx_xlock_addr                   0x0042BB40
#define _sx_xunlock_addr                 0x0042BD00
#define malloc_addr                      0x0029D330
#define free_addr                        0x0029D4F0
#define strstr_addr                      0x00248480
#define fpu_kern_enter_addr              0x002BDDA0
#define fpu_kern_leave_addr              0x002BDE60
#define memcpy_addr                      0x00201CC0
#define memset_addr                      0x000C1720
#define strlen_addr                      0x003F1980
#define printf_addr                      0x00205470
#define eventhandler_register_addr       0x00285720
                                         
// Fself                                 
#define sceSblACMgrGetPathId_addr        0x00032640
#define sceSblServiceMailbox_addr        0x006276E0
#define sceSblAuthMgrSmIsLoadable2_addr  0x0063A970
#define _sceSblAuthMgrGetSelfInfo_addr   0x0063B1B0
#define _sceSblAuthMgrSmStart_addr       0x00633FA0
#define sceSblAuthMgrVerifyHeader_addr   0x0063A9D0
                                         
// Fpkg                                  
#define RsaesPkcs1v15Dec2048CRT_addr     0x002BA420
#define Sha256Hmac_addr                  0x0021B230
#define AesCbcCfb128Encrypt_addr         0x003681A0
#define AesCbcCfb128Decrypt_addr         0x003683D0
#define sceSblDriverSendMsg_0_addr       0x00613C30
#define sceSblPfsSetKeys_addr            0x0061F6D0
#define sceSblKeymgrSetKeyStorage_addr   0x0061F3F0
#define sceSblKeymgrSetKeyForPfs_addr    0x00619DF0
#define sceSblKeymgrCleartKey_addr       0x0061A130
#define sceSblKeymgrSmCallfunc_addr      0x006199C0
                                         
// Patch                                 
#define vmspace_acquire_ref_addr         0x00191BA0
#define vmspace_free_addr                0x001919D0
#define vm_map_lock_read_addr            0x00191D30
#define vm_map_unlock_read_addr          0x00191D80
#define vm_map_lookup_entry_addr         0x00192370
#define proc_rwmem_addr                  0x00479620
                                         
#define printf_hook                      0x01A72E98

// Fself hooks
#define sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook          0x006390EC
#define sceSblAuthMgrIsLoadable2_hook                               0x0063923E
#define sceSblAuthMgrVerifyHeader_hook1                             0x006399D6
#define sceSblAuthMgrVerifyHeader_hook2                             0x0063A6B9
#define sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook   0x006363ED
#define sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook     0x00637028

// Fpkg hooks
#define sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook         0x0061F495
#define sceSblKeymgrInvalidateKey__sx_xlock_hook                    0x0061AFAD
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook      0x00645DD0
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook           0x00646B9E
#define mountpfs__sceSblPfsSetKeys_hook1                            0x006A69E9
#define mountpfs__sceSblPfsSetKeys_hook2                            0x006A6C1A

//SceShellUI patches - debug patches
#define sceSblRcMgrIsAllowDebugMenuForSettings_patch                0x0001CE50
#define sceSblRcMgrIsStoreMode_patch                                0x0001D1B0

// SceShellUI patches - remote play patches                                
// #define CreateUserForIDU_patch                                      0x0
// #define remote_play_menu_patch                                      0x0

// SceRemotePlay patches - remote play patches
// #define SceRemotePlay_patch1                                        0x0
// #define SceRemotePlay_patch2                                        0x0

// call sceVshAvcapSetInfo
//#define sceVshAvcapSetInfo_patch1       0x0
//#define sceVshAvcapSetInfo_patch2       0x0
//#define sceVshAvcapSetInfo_patch3       0x0
//#define sceVshAvcapSetInfo_patch4       0x0
//#define sceVshAvcapSetInfo_patch5       0x0
//#define sceVshAvcapSetInfo_patch6       0x0

// SceShellCore patches
// call sceKernelIsGenuineCEX
#define sceKernelIsGenuineCEX_patch1    0x0016CD64
#define sceKernelIsGenuineCEX_patch2    0x008630A4
#define sceKernelIsGenuineCEX_patch3    0x008B1CA2
#define sceKernelIsGenuineCEX_patch4    0x00A122C4

// call nidf_libSceDipsw
#define nidf_libSceDipsw_patch1         0x0016CD92
#define nidf_libSceDipsw_patch2         0x0024A35C
#define nidf_libSceDipsw_patch3         0x008630D2
#define nidf_libSceDipsw_patch4         0x00A122F2

// enable data mount
#define enable_data_mount_patch         0x0031D651

// enable fpkg
#define enable_fpkg_patch               0x003D4E9F
 
// debug pkg free string
#define fake_free_patch                 0x00FBD319

// make pkgs installer working with external hdd
#define pkg_installer_patch             0x009FB811

// enable support with 11.00 external hdd
#define ext_hdd_patch                   0x00610AED

// enable debug trophies on retail
#define debug_trophies_patch            0x00741E09

// disable screenshot block
// #define disable_screenshot_patch        0x0

#endif
