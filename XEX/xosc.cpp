#include "stdafx.h"
#include "Xosc.h"
#include "Utilities.h"
#include "NetConnect.h"
#include "Config.h"
#include "KeyVault.h"
#include "HvPeekPoke.h"
#include "SysHooks.h"
 
#define hvKvPtrDev      0x00000002000162e0
#define hvKvPtrRetail   0x0000000200016240

extern XEX_EXECUTION_ID xeExecutionIdSpoof;
extern DWORD dwHvKeysStatusFlags;
extern KEY_VAULT keyVault;
extern BYTE kvDigest[];
extern BYTE cpuKeyDigest[];
typedef BOOL (*pfnXamLoaderIsTitleTerminatePending)();

typedef enum{
	XENON = 0x34323761,
	ZEPHYR = 0x39383130,
	FALCON = 0x39393430,
	JASPER = 0x39393564,
	TRINITY = 0x39386662,
	CORONA = 0x39386661
}CONSOLE_TYPES;

DWORD CreateXOSCBufferOffline(DWORD dwTaskParam1, BYTE* pbDaeTableName, DWORD cbDaeTableName, XOSC* pbBuffer, DWORD cbBuffer) {	
	ZeroMemory(pbBuffer, cbBuffer);//xosc not many sources have this
	BYTE kv[0x4000];
	QWORD kvAddress = (HvPeekQWORD(hvKvPtrRetail));
	HvPeekBytes(kvAddress, kv, 0x4000);
	XEX_EXECUTION_ID* pExecutionId;
    XamGetExecutionId(&pExecutionId);
	DWORD titleID = pExecutionId->TitleID;
	if (titleID == 0xFFFF0055 || titleID == 0xC0DE9999 || titleID == 0xFFFE07FF || titleID == 0xF5D20000 || titleID == 0xFFFF011D){
		SetMemory(pExecutionId, &xeExecutionIdSpoof, sizeof(XEX_EXECUTION_ID));
		titleID = pExecutionId->TitleID;
	}
	QWORD qwOperations;
	if (pExecutionId >= 0) qwOperations |= 0x4;
	if (XamLoaderIsTitleTerminatePending()) qwOperations |= 0x4000000000000000;
    if (XamTaskShouldExit()) qwOperations |= 0x2000000000000000;
	WORD BLDRFlags = 0xD83E, BLDR_FLAGS_KV1 = (~0x20);
	DWORD Type1KvHash = 0x304000D, policy_flash_size = (DWORD)(&kv + 0x24); 
	if (type1KV == 1) { Type1KvHash = 0x10B0400; BLDRFlags = (WORD)(BLDRFlags & BLDR_FLAGS_KV1); policy_flash_size = 0;} 
	DWORD hv_keys_status_flags = 0x023289D3;
	if (crl) hv_keys_status_flags |= 0x10000;
    if (fcrt) hv_keys_status_flags |= 0x1000000;
	QWORD hv_protected_flags = 4 | (*(QWORD*)0x8E038678 & 1);
	DWORD hardware_flags = 0x40000207;
	switch(HvPeekDWORD(kvAddress + 0x2245)){ 
	  case XENON: hardware_flags = CONSOLE_TYPE_XENON | 0x00000207; break;
	  case ZEPHYR: hardware_flags = CONSOLE_TYPE_ZEPHYR | 0x00000207; break;
	  case FALCON: hardware_flags = CONSOLE_TYPE_FALCON | 0x00000207; break;
	  case JASPER: hardware_flags = CONSOLE_TYPE_JASPER | 0x00000207; break;
	  case TRINITY: hardware_flags = CONSOLE_TYPE_TRINITY | 0x00000207; break;
	  case CORONA: hardware_flags = CONSOLE_TYPE_CORONA | 0x00000207; break;
	  default: hardware_flags = CONSOLE_TYPE_WINDCHESTER | 0x00000207;
	}
	pbBuffer->dwResult = 0;
	pbBuffer->xoscMajor = 9;
	pbBuffer->xoscMinor = 2;
	pbBuffer->qwOperations = qwOperations;
	pbBuffer->dvd_ioctl_res = 0;
	pbBuffer->xekeysgetkey_res = 0;
	pbBuffer->dwExecutionResult = (DWORD)pExecutionId;
	memset(&pbBuffer->console_id_null, 0, 0x5);
	pbBuffer->unk_hash_res = 0xC8003003;
	pbBuffer->dae_result = 0;
    memcpy(&pbBuffer->xexExecutionId, pExecutionId, 0x18);
	memcpy(&pbBuffer->cpuKeyHash, cpuKeyDigest, 0x10);
	memcpy(&pbBuffer->kvHash, keyVault.HmacShaDigest, 0x10);
	memcpy(&pbBuffer->sec_fuses, (PVOID)0x8E03AA50, 0x10);
	pbBuffer->drivePhaseLevel = (BYTE)(kv + 0xC89);
	pbBuffer->titleID = titleID;
	memcpy(&pbBuffer->driveData1, (kv + 0xC8A), 0x24);
	memcpy(&pbBuffer->driveData2, (kv + 0xC8A), 0x24);
	memcpy(&pbBuffer->consoleSerial, keyVault.ConsoleSerialNumber, 0xC);
	pbBuffer->serial_byte = 0;
	pbBuffer->bldr_flags = BLDRFlags;
	pbBuffer->xamRegion = (WORD)(kv + 0xC8);
	pbBuffer->xamOdd = (WORD)(kv + 0x1C);
	pbBuffer->beta_bldr = 0;
	pbBuffer->policy_flash = policy_flash_size;
	pbBuffer->xosc_region = 0x70000; 
	pbBuffer->hv_keys_status_flags = hv_keys_status_flags;
	pbBuffer->xosc_dae = 0x000D000800000008;
	pbBuffer->kv_restricted_priv = 0;
	pbBuffer->hv_protected_flags = hv_protected_flags;
	memcpy(&pbBuffer->console_id, (kv +0x9CA), 0x5);
	pbBuffer->hardware_flags = hardware_flags;
    pbBuffer->sizeMu0 = getDeviceSize("\\Device\\Mu0\\");
    pbBuffer->sizeMu1 = getDeviceSize("\\Device\\Mu1\\");
    pbBuffer->sizeMuSfc = getDeviceSize("\\Device\\BuiltInMuSfc\\");
    pbBuffer->sizeMuUsb = getDeviceSize("\\Device\\BuiltInMuUsb\\Storage\\");
    pbBuffer->sizeExUsb0 = getDeviceSize("\\Device\\Mass0PartitionFile\\Storage\\");
    pbBuffer->sizeExUsb1 = getDeviceSize("\\Device\\Mass1PartitionFile\\Storage\\");
    pbBuffer->sizeExUsb2 = getDeviceSize("\\Device\\Mass2PartitionFile\\Storage\\");
	pbBuffer->crl_version = 0x00000006;
	pbBuffer->xosc_footer = 0x5F534750;
	memset(pbBuffer->unused_filler, 0xAA, 120);
	*(int*)(0x9001639C) = 0x39200001;
	*(int*)(0x900163B4) = 0x39400001;
	*(int*)(0x900163C4) = 0x38C00020;
	*(int*)(0x900161D4) = 0x3900FFFF;
	*(int*)(0x90016414) = 0x3900FFFF;
	*(int*)(0x900162BC) = 0x38C00008;
	*(int*)(0x9001516C) = 0x39200000;
	return ERROR_SUCCESS;
}