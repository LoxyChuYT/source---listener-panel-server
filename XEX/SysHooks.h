#pragma once
#include "stdafx.h"



typedef struct _XAM_CHAL_RESP {
	//response starts at 0x20
	BYTE bReserved1[8];                            //0x20
	WORD wHvMagic;                                 //0x28
	WORD wHvVersion;                               //0x2A
	WORD wHvQfe;                                   //0x2C
	WORD wBldrFlags;                               //0x2E
	DWORD dwBaseKernelVersion;                     //0x30
	DWORD dwUpdateSequence;                        //0x34
	DWORD dwHvKeysStatusFlags;                     //0x38
	DWORD dwConsoleTypeSeqAllow;                   //0x3C
	QWORD qwRTOC;                                  //0x40
	QWORD qwHRMOR;                                 //0x48
	BYTE bHvECCDigest[XECRYPT_SHA_DIGEST_SIZE];    //0x50
	BYTE bCpuKeyDigest[XECRYPT_SHA_DIGEST_SIZE];   //0x64
	BYTE bRandomData[0x80];                        //0x78
	WORD hvExAddr;                                 //0xF8
	BYTE bHvDigest[0x6];                           //0xFA
} XAM_CHAL_RESP, *PXAM_CHAL_RESP;

#define hvKvPtrRetail   0x0000000200016240

typedef enum _XOSC_FLAGS : QWORD {
	XOSC_FLAGS_TASK_SHOULD_EXIT = 0x2000000000000000,
	XOSC_FLAGS_TITLE_TERMINATED = 0x4000000000000000
} XOSC_FLAGS;

typedef enum _SV_PROTECTED_FLAGS {
	FLAG_SV_PROTECTED_NONE = 0x0,
	FLAG_SV_PROTECTED_NO_EJECT_REBOOT = 0x1, //Set on dash and such. Means the box doesn't change titles if the disc tray is ejected.
	FLAG_SV_PROTECTED_DISC_AUTHENTICATED = 0x2, //Is set when a disc is put in the tray and completely verified.
	FLAG_SV_PROTECTED_AUTH_EX_CAPABLE = 0x4
} SV_PROTECTED_FLAGS;

typedef enum {
	XENON = 0x34323761,
	ZEPHYR = 0x39383130,
	FALCON = 0x39393430,
	JASPER = 0x39393564,
	TRINITY = 0x39386662,
	CORONA = 0x39386661,
	Unknown = 0x39386639
}CONSOLE_TYPES;

#pragma pack(1)
typedef struct _XOSC {
	DWORD                   dwResult;                     // 0x00
	WORD                    xoscMajor;                    // 0x04
	WORD                    xoscMinor;                    // 0x06
	QWORD                   qwOperations;                 // 0x08
	DWORD                   dvd_ioctl_res;                // 0x10
	DWORD                   xekeysgetkey_resC;            // 0x14
	DWORD                   dwExecutionResult;            // 0x18
	DWORD                   xekeysgetkey_resS;            // 0x1C
	DWORD					sata_res;                     // 0x20
	BYTE					unknown1[0x10]; 				  // 0x24
	DWORD					dae_result; 				  // 0x34
	XEX_EXECUTION_ID        xexExecutionId;               // 0x38
	BYTE                    cpuKeyHash[0x10];             // 0x50
	BYTE                    kvHash[0x10];                 // 0x60
	BYTE                    sec_fuses[0x10];              // 0x70
	DWORD                   drivePhaseLevel;              // 0x80
	DWORD                   Mediainfo;		              // 0x84
	DWORD                   titleID;		              // 0x88
	BYTE                    anti_piracy[0x64];            // 0x8C
	BYTE                    driveData1[0x24];			  // 0xF0
	BYTE                    driveData2[0x24];			  // 0x114
	BYTE                    consoleSerial[0xC];           // 0x138
	WORD                    serial_byte;                  // 0x144
	WORD                    bldr_flags;                   // 0x146
	WORD                    xamRegion;                    // 0x148
	WORD                    xamOdd;                       // 0x14A
	DWORD                   beta_bldr;                    // 0x14C
	DWORD                   policy_flash;                 // 0x150
	DWORD                   xosc_region;                  // 0x154
	QWORD                   hv_keys_status_flags;         // 0x158
	BYTE					unk_data2[0x10];			  // 0x160
	QWORD					xosc_dae;					  // 0x170
	BYTE					unk_data3[0x8];				  // 0x178
	QWORD					kv_restricted_priv;			  // 0x180
	BYTE					unk_data4[0x10];			  // 0x188
	QWORD					hv_protected_flags;			  // 0x198
	BYTE					console_id[0x5];			  // 0x1A0
	BYTE					unknown[0x2b];                // 0x1A8
	DWORD					hardware_flags;				  // 0x1D0
	BYTE					nulled_hardware_flags[0xD4];  // 0x1D4
	DWORD					sizeMu0;					  // 0x2A8
	DWORD					sizeMu1;					  // 0x2AC
	DWORD					sizeMuSfc;					  // 0x2B0
	DWORD					sizeMuUsb;					  // 0x2B4
	DWORD					sizeExUsb0;					  // 0x2B8
	DWORD					sizeExUsb1;					  // 0x2BC
	DWORD					sizeExUsb2;					  // 0x2C0
	DWORD					crl_version;				  // 0x2C4
	BYTE					unk_data5[0x10];			  // 0x2C8
	DWORD					xosc_footer;				  // 0x2D8
	BYTE					unk_data6[0x4];				  // 0x2DC
	BYTE					unused_filler[0x120];		  // 0x2E0

} XOSC, *pXOSC;

BOOL InitializeSystemHooks();
NTSTATUS XexLoadExecutableHook(PCHAR szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion);
NTSTATUS XexLoadImageHook(LPCSTR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle);