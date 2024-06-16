#ifndef __OFFSETS_H__
#define __OFFSETS_H__
#pragma once

// 7.55
#define	XFAST_SYSCALL_addr              0x00001C0

// Names - Data
#define PRISON0_addr                    0x113B728
#define ROOTVNODE_addr                  0x1B463E0
#define PMAP_STORE_addr                 0x215EA40
#define DT_HASH_SEGMENT_addr            0x0C05068
#define global_settings_base            0x2228950

// Functions
#define pmap_protect_addr               0x01A9800
#define pmap_protect_p_addr             0x01A9847

// Patches
// debug menu error
#define debug_menu_error_patch1         0x04FF322
#define debug_menu_error_patch2         0x050059C

// disable signature check
#define disable_signature_check_patch   0x06DD9A0

// enable debug RIFs
#define enable_debug_rifs_patch1        0x0668140
#define enable_debug_rifs_patch2        0x0668170
	
// allow sys_dynlib_dlsym in all processes
#define sys_dynlib_dlsym_patch          0x04523C4

// sdk version spoof - enable all VR fws
#define sdk_version_patch               0x157FED0	
	
// enable debug log
#define enable_debug_log_patch          0x026F827

// enable uart output
#define enable_uart_patch               0x1564910

#endif