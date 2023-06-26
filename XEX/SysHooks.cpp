#include "stdafx.h"
#include "SysHooks.h"
#include "Utilities.h"
#include "GameHooks.h"
#include "Config.h"
#include "SysHooks.h"
#include "Utilities.h"
#include "GameHooks.h"
#include "Config.h"
#include "NetConnect.h"
#include <string>
#include <stdio.h>
#include "NetReq.h"
#include <bitset>
#include "HvPeekPoke.h"
#include <time.h>
#include "liveblock.h"

bool challenge = false;
extern BOOL commandPending;
extern BYTE kvDigest[];
extern KEY_VAULT keyVault;
extern BYTE cpuKeyDigest[];
XEX_EXECUTION_ID xeExecutionIdSpoof;
extern HANDLE dllHandle;
extern HANDLE hXam;
extern bool Initialized;
extern DWORD dwUpdateSequence;
extern BYTE CPUKey[];
BYTE Trinity_Hash[] = { 0xDB, 0xE6, 0x35, 0x87, 0x78, 0xCB, 0xFC, 0x2F, 0x52, 0xA3, 0xBA, 0xF8, 0x92, 0x45, 0x8D, 0x65 };//17526
BYTE Corona_Hash[] = { 0xD1, 0x32, 0xFB, 0x43, 0x9B, 0x48, 0x47, 0xE3, 0x9F, 0xE5, 0x46, 0x46, 0xF0, 0xA9, 0x9E, 0xB1 };//17526
BYTE Falcon_Hash[] = { 0x4E, 0xEA, 0xA3, 0x32, 0x3D, 0x9F, 0x40, 0xAA, 0x90, 0xC0, 0x0E, 0xFC, 0x5A, 0xD5, 0xB0, 0x00 };//17526
BYTE Jasper_Hash[] = { 0xFF, 0x23, 0x99, 0x90, 0xED, 0x61, 0xD1, 0x54, 0xB2, 0x31, 0x35, 0x99, 0x0D, 0x90, 0xBD, 0xBC };//17526

QWORD SpoofXamChallenge(PBYTE pBuffer, DWORD dwFileSize, PBYTE Salt, PXBOX_KRNL_VERSION krnlbuild, PDWORD r7, PDWORD r8) {
	while (!Initialized) Sleep(1);
	while (commandPending) Sleep(1);
	while (challenge) Sleep(1);
	challenge = true;
	DbgPrint("XamChallenge - Called");

	XeKeysExecute(pBuffer, dwFileSize, (PBYTE)MmGetPhysicalAddress(Salt), krnlbuild, r7, r8);

	SERVER_CHAL_REQUEST chalRequest;
	SERVER_CHAL_RESPONCE* pChalResponce = (SERVER_CHAL_RESPONCE*)pBuffer;

	memcpy(chalRequest.SessionKey, seshKey, 16);
	memcpy(chalRequest.Salt, Salt, 16);

	if (SendCommand(XSTL_SERVER_COMMAND_ID_GET_CHAL_RESPONCE, &chalRequest, sizeof(SERVER_CHAL_REQUEST), pChalResponce, sizeof(SERVER_CHAL_RESPONCE)) != ERROR_SUCCESS) {
		//HalReturnToFirmware(HalFatalErrorRebootRoutine);
		return 0;
	}

	if (pChalResponce->Status != XSTL_STATUS_STEALTHED)
	{
		DbgPrint("[XAMC] Bad Response");
		XNotifyUI(L"ReblLive - XAM Challenge Failed!");
		Sleep(5000);
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
		return 0;
	}

	pChalResponce->Status = 0;
	
	WORD BLDRFlags = 0xD83E;
	DWORD Type1KvHash = 0x304000D;
	DWORD hv_keys_status_flags = 0x023289D3;
	if (crl) { hv_keys_status_flags |= 0x10000; }
	if (fcrt) { hv_keys_status_flags |= 0x1000000; }
	if (type1KV) {
		Type1KvHash = 0x10B0400;
		BLDRFlags = 0xD81E;
	}
if (!type1KV){
		BYTE kv[2];
		QWORD kvAddress = HvPeekQWORD(hvKvPtrRetail);
		HvPeekBytes(kvAddress+0x9D1, kv, 0x2);
		BYTE mobo = ((kv[0] << 4) & 0xF0) | (kv[1] & 0x0F);
		if(mobo < 0x10) Type1KvHash = 0x010B0524;
		else if(mobo < 0x14) Type1KvHash = 0x010C0AD0;
		else if(mobo < 0x18) Type1KvHash = 0x010C0AD8;
		else if(mobo < 0x52) Type1KvHash = 0x010C0FFB;
		else if(mobo < 0x58) Type1KvHash = 0x0304000D;
		else Type1KvHash = 0x0304000E;
	}
	BYTE RANDDATA[0x80];
	XeCryptRandom(RANDDATA, 0x80);
	BYTE ECC[0x14];
	XeCryptRandom(ECC, 0x14);
	*(WORD*)(pBuffer + 0x2E) = BLDRFlags;//BLDRFlags
	*(DWORD*)(pBuffer + 0x34) = dwUpdateSequence;//updatesequence
	*(DWORD*)(pBuffer + 0x38) = hv_keys_status_flags;//StatusFlags
	*(DWORD*)(pBuffer + 0x3C) = Type1KvHash;//kvtype
	memcpy(pBuffer + 0x50, ECC, 0x14);//rand ecc
	memcpy(pBuffer + 0x64, cpuKeyDigest, 0x14);//cpu digest
	memcpy(pBuffer + 0x78, RANDDATA, 0x80);//hvrand

	//CWriteFile("Hdd:\\challengedumpafter.bin", pBuffer, dwFileSize);

	crl = TRUE;
	challenge = false;
	return 0;
}

VOID* RtlImageXexHeaderFieldHook(VOID* headerBase, DWORD imageKey) 
{
	VOID* retVal = RtlImageXexHeaderField(headerBase, imageKey);
	if(imageKey == 0x40006 && retVal)
	{
		switch (((XEX_EXECUTION_ID*)retVal)->TitleID)
		{
		case 0xFFFF0055: // Xex Menu
		case 0xC0DE9999: // Xex Menu alt
		case 0xFFFE07FF: // XShellXDK
		case 0xF5D20000: // FSD
		case 0xFFFF011D: // DashLaunch
		case 0x00000166: // Aurora
		case 0x00000189: // Simple360 NandFlasher
		case 0x00000188: // Flash 360
		case 0x00000176: // XM360
		case 0x00000167: // Freestyle 3
		case 0x00000177: // NXE2GOD
		case 0x00000170: // Xexmenu 2.0
		case 0xFFFEFF43: // Xell Launch GOD
		case 0xFEEDC0DE: // XYZProject
		case 0x58480880: // Internet Explorer HB
		case 0x00000001: // FXMenu
		case 0x00000171: // FCEUX
		case 0xFFED0707: // SNES360
		case 0x00000000: // freestylenew and some homebrew
		case 0x1CED2911: // pcsxr
		case 0xFFED7300: // FCE360
		case 0x00FBAFBA: // DSon360
		case 0x000003D0: //3dox
			{	
				SetMemory(retVal, &xeExecutionIdSpoof, sizeof(XEX_EXECUTION_ID));
				break;
			}
		}
	} 
	else if(imageKey == 0x40006 && !retVal) 
	{
		retVal = &xeExecutionIdSpoof;
	}
	return retVal;
}

extern XEX_EXECUTION_ID* pExecutionId;
typedef DWORD(*ExecuteSupervisorChallenge_t)(DWORD dwTaskParam1, PBYTE pbDaeTableName, DWORD cbDaeTableName, PBYTE pBuffer, DWORD cbBuffer);
DWORD XamLoaderExecuteAsyncChallengeHook(DWORD dwAddress, DWORD dwTaskParam1, PBYTE pbDaeTableName, DWORD szDaeTableName, PBYTE pBuffer, DWORD cbBuffer) {
	while (!Initialized) Sleep(1);
	DbgPrint("XoscChallenge - Called");
	while (challenge) Sleep(1);
	challenge = true;

	WORD BLDRFlags = 0xD83E;
	DWORD hv_status_flags = 0x023289D3;
	if (crl) { hv_status_flags |= 0x10000; }
	if (fcrt) {
		hv_status_flags |= 0x1000000;
		BLDRFlags = 0xD81E;
	}

	ExecuteSupervisorChallenge_t ExecuteSupervisorChallenge = (ExecuteSupervisorChallenge_t)dwAddress;
	ExecuteSupervisorChallenge(dwTaskParam1, pbDaeTableName, szDaeTableName, pBuffer, cbBuffer);

	SERVER_GET_module_REQUEST chalRequest;
	SERVER_XOSC_RESPONCE* pChalResponce = (SERVER_XOSC_RESPONCE*)pBuffer;

	memcpy(chalRequest.SessionKey, seshKey, 16);

	if (SendCommand(XSTL_SERVER_COMMAND_ID_GET_XOSC, &chalRequest, sizeof(SERVER_GET_module_REQUEST), pChalResponce, sizeof(SERVER_XOSC_RESPONCE)) != ERROR_SUCCESS) {
		//HalReturnToFirmware(HalFatalErrorRebootRoutine);
		return 0;
	}

	memcpy(pBuffer + 0x38, pExecutionId, 0x18);
	memcpy(pBuffer + 0x50, cpuKeyDigest, 0x10);
	memcpy(pBuffer + 0x60, kvDigest, 0x10);
	memcpy(pBuffer + 0x80, &keyVault.XeikaCertificate.Data.OddData.PhaseLevel, 0x1);
	*(DWORD*)(pBuffer + 0x84) = pExecutionId->MediaID;
	*(DWORD*)(pBuffer + 0x88) = pExecutionId->TitleID;
	memcpy(pBuffer + 0xF0, &keyVault.XeikaCertificate.Data.OddData.InquiryData.Data, 0x24);
	memcpy(pBuffer + 0x114, &keyVault.XeikaCertificate.Data.OddData.InquiryData.Data, 0x24);
	memcpy(pBuffer + 0x138, &keyVault.ConsoleSerialNumber, 0xC);
	*(WORD*)(pBuffer + 0x146) = BLDRFlags;
	*(DWORD*)(pBuffer + 0x150) = keyVault.PolicyFlashSize;
	*(DWORD*)(pBuffer + 0x158) = hv_status_flags;
	memcpy(pBuffer + 0x1A0, &keyVault.ConsoleCertificate.ConsoleId, 0x5);
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\Mu0\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\Mu1\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\BuiltInMuSfc\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\BuiltInMuUsb\\Storage\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\Mass0PartitionFile\\Storage\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\Mass1PartitionFile\\Storage\\");
	*(DWORD*)(pBuffer + 0x2A8) = getDeviceSize("\\Device\\Mass2PartitionFile\\Storage\\");

	BYTE kv[2];
	QWORD kvAddress = HvPeekQWORD(hvKvPtrRetail);
	HvPeekBytes(kvAddress + 0x9D1, kv, 0x2);
	BYTE mobo = ((kv[0] << 4) & 0xF0) | (kv[1] & 0x0F);
	if (mobo < 0x10) { //xenon
		memset(pBuffer + 0x70, 0, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x00000207;
	}
	else if (mobo < 0x14) { //zephyr
		memset(pBuffer + 0x70, 0, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x10000227;
	}
	else if (mobo < 0x18) { //falcon
		memcpy(pBuffer + 0x70, Falcon_Hash, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x20000227;
	}
	else if (mobo < 0x52) { //jasper
		memcpy(pBuffer + 0x70, Jasper_Hash, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x30000227;
	}
	else if (mobo < 0x58) { //trinity
		memcpy(pBuffer + 0x70, Trinity_Hash, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x40000227;
	}
	else { //corona and maybe winchester?
		memcpy(pBuffer + 0x70, Corona_Hash, 0x10);
		*(DWORD*)(pBuffer + 0x1D0) = 0x50000227;
	}

	//CWriteFile("Hdd:\\xoscdumpafter.bin", pBuffer, cbBuffer);

	challenge = false;
	return 0;
}

NTSTATUS XexLoadImageHook(LPCSTR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle)
{
	HANDLE mHandle = NULL;
	NTSTATUS result = XexLoadImage(szXexName, dwModuleTypeFlags, dwMinimumVersion, &mHandle);
	if (pHandle != NULL) *pHandle = mHandle;
	if (NT_SUCCESS(result)) InitializeGameHooks((PLDR_DATA_TABLE_ENTRY)mHandle);	
	return result;
}

NTSTATUS XexLoadExecutableHook(PCHAR szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion) 
{
	HANDLE mHandle = NULL;
	NTSTATUS result = XexLoadExecutable(szXexName, &mHandle, dwModuleTypeFlags, dwMinimumVersion);
	if (pHandle != NULL) *pHandle = mHandle;
	if (NT_SUCCESS(result)) InitializeGameHooks((PLDR_DATA_TABLE_ENTRY)*XexExecutableModuleHandle);		
	return result;
}

BOOL XexCheckExecutablePrivilegeHook(DWORD priv) {
	if (priv == 6)
		return TRUE;

	return XexCheckExecutablePrivilege(priv);
}

BOOL InitializeSystemHooks() 
{
	DWORD ver = ((XboxKrnlVersion->Major & 0xF) << 28) | ((XboxKrnlVersion->Minor & 0xF) << 24) | (XboxKrnlVersion->Build << 8) | (XboxKrnlVersion->Qfe);
	ZeroMemory(&xeExecutionIdSpoof, sizeof(XEX_EXECUTION_ID));
	xeExecutionIdSpoof.Version = ver;
	xeExecutionIdSpoof.BaseVersion = ver;
	xeExecutionIdSpoof.TitleID = 0xFFFE07D1;
	if (PatchModuleImport(NAME_XAM, NAME_KERNEL, 0x12B, (DWORD)RtlImageXexHeaderFieldHook) != S_OK) return S_FALSE;
	PatchInJump((DWORD*)0x8169CCD0, (DWORD)XamLoaderExecuteAsyncChallengeHook, FALSE);//17526
	PatchInJump((DWORD*)0x81A71414, (DWORD)SpoofXamChallenge, FALSE);//17526
	if (PatchModuleImport(NAME_XAM, NAME_KERNEL, 408, (DWORD)XexLoadExecutableHook) != S_OK) return S_FALSE;
	if (PatchModuleImport(NAME_XAM, NAME_KERNEL, 409, (DWORD)XexLoadImageHook) != S_OK) return S_FALSE;
	if (PatchModuleImport(NAME_XAM, NAME_KERNEL, 404, (DWORD)XexCheckExecutablePrivilegeHook) != S_OK) return S_FALSE;
	return TRUE;
}