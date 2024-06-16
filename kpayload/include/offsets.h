#ifndef __OFFSETS_H__
#define __OFFSETS_H__
#pragma once

//10.50
// data
#define	XFAST_SYSCALL_addr              0x000001C0
#define M_TEMP_addr                     0x01A5FE30
#define MINI_SYSCORE_SELF_BINARY_addr   0x01541E78
#define ALLPROC_addr                    0x02269F30
#define SBL_DRIVER_MAPPED_PAGES_addr    0x02646CA8
#define SBL_PFS_SX_addr                 0x0265C310
#define SBL_KEYMGR_KEY_SLOTS_addr       0x02660858
#define SBL_KEYMGR_KEY_RBTREE_addr      0x02660868
#define SBL_KEYMGR_BUF_VA_addr          0x02664000
#define SBL_KEYMGR_BUF_GVA_addr         0x02664808
#define FPU_CTX_addr                    0x026796C0
#define DIPSW_addr                      0x01BE45D0
                                        
// common                               
#define memcmp_addr                     0x0002A020
#define _sx_xlock_addr                  0x000977A0
#define _sx_xunlock_addr                0x00097960
#define malloc_addr                     0x0036E120
#define free_addr                       0x0036E2E0
#define strstr_addr                     0x002FDB20
#define fpu_kern_enter_addr             0x00300A80
#define fpu_kern_leave_addr             0x00300B40
#define memcpy_addr                     0x000D7370
#define memset_addr                     0x0000D090
#define strlen_addr                     0x00160DA0
#define printf_addr                     0x00450E80
#define eventhandler_register_addr      0x000ED020
                                        
// Fself                                
#define sceSblACMgrGetPathId_addr       0x001F4520
#define sceSblServiceMailbox_addr       0x00630550
#define sceSblAuthMgrSmIsLoadable2_addr 0x00643BF0
#define _sceSblAuthMgrGetSelfInfo_addr  0x00644430
#define _sceSblAuthMgrSmStart_addr      0x0063BEB0
#define sceSblAuthMgrVerifyHeader_addr  0x00643C50
                                        
// Fpkg                                 
#define RsaesPkcs1v15Dec2048CRT_addr    0x00350360
#define Sha256Hmac_addr                 0x00441BB0
#define AesCbcCfb128Encrypt_addr        0x0033EE60
#define AesCbcCfb128Decrypt_addr        0x0033F090
#define sceSblDriverSendMsg_0_addr      0x0061B3C0
#define sceSblPfsSetKeys_addr           0x00622F50
#define sceSblKeymgrSetKeyStorage_addr  0x00622910
#define sceSblKeymgrSetKeyForPfs_addr   0x00625670
#define sceSblKeymgrCleartKey_addr      0x006259B0
#define sceSblKeymgrSmCallfunc_addr     0x00625240
                                        
// Patch                                
#define vmspace_acquire_ref_addr        0x00476140
#define vmspace_free_addr               0x00475F70
#define vm_map_lock_read_addr           0x004762D0
#define vm_map_unlock_read_addr         0x00476320
#define vm_map_lookup_entry_addr        0x00476910
#define proc_rwmem_addr                 0x004244A0

#define printf_hook                     0x01A7AE88

// Fself hooks
#define sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook          0x0064117C
#define sceSblAuthMgrIsLoadable2_hook                               0x006412CE
#define sceSblAuthMgrVerifyHeader_hook1                             0x00641A66
#define sceSblAuthMgrVerifyHeader_hook2                             0x00642749
#define sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook   0x0063E2FD
#define sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook     0x0063EF38
                                                                    
// Fpkg hooks                                                       
#define sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook         0x006229B5
#define sceSblKeymgrInvalidateKey__sx_xlock_hook                    0x0062682D
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook      0x0064B880
#define sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook           0x0064C64E
#define mountpfs__sceSblPfsSetKeys_hook1                            0x006B59F9
#define mountpfs__sceSblPfsSetKeys_hook2                            0x006B5C2A
                                                                    
// SceShellUI patches - debug patches                               
#define sceSblRcMgrIsAllowDebugMenuForSettings_patch                0x0001CFC0
#define sceSblRcMgrIsStoreMode_patch                                0x0001D320
                                                                    
// SceShellUI patches - remote play patches                         
// #define CreateUserForIDU_patch                                      0x001B11D6
// #define remote_play_menu_patch                                      0x00EE638E
                                                                    
// SceRemotePlay patches - remote play patches                      
// #define SceRemotePlay_patch1                                        0x003C33F
// #define SceRemotePlay_patch2                                        0x003C35A

// SceShellCore patches
// call sceKernelIsGenuineCEX
#define sceKernelIsGenuineCEX_patch1    0x0016B664
#define sceKernelIsGenuineCEX_patch2    0x0085BAB4
#define sceKernelIsGenuineCEX_patch3    0x008ABCE2
#define sceKernelIsGenuineCEX_patch4    0x00A0CA84
                                        
// call nidf_libSceDipsw                
#define nidf_libSceDipsw_patch1         0x0016B692
#define nidf_libSceDipsw_patch2         0x00249B0C
#define nidf_libSceDipsw_patch3         0x0085BAE2
#define nidf_libSceDipsw_patch4         0x00A0CAB2
                                        
// enable data mount                    
#define enable_data_mount_patch         0x0031E890
                                        
// enable fpkg                          
#define enable_fpkg_patch               0x003D544F
                                        
// debug pkg free string                
#define fake_free_patch                 0x00FB5D59
                                        
// make pkgs installer working with external HDD
#define pkg_installer_patch             0x009F5FD1
                                        
// enable support with 6.xx external hdd
#define ext_hdd_patch                   0x00606B7D
                                        
// enable debug trophies on retail      
// #define debug_trophies_patch            0x0
                                        
// disable screenshot block             
// #define disable_screenshot_patch        0x0

#endif