#include "stdafx.h"
#include "KeyVault.h"
#include "Utilities.h"
#include "HvPeekPoke.h"

#define hvKvPtrRetail   0x0000000200016240

BYTE kvDigest[XECRYPT_SHA_DIGEST_SIZE];
extern BYTE cpuKeyDigest[];
KEY_VAULT keyVault;
DWORD dwUpdateSequence;
// Chal info
BOOL fcrt = FALSE;
BOOL crl = FALSE;
BOOL type1KV = FALSE;
extern BYTE CPUKey[0x10];


HRESULT SetMacAddress() {
	BYTE macAddress[6];
	macAddress[0] = 0x00;
	macAddress[1] = 0x22;
	macAddress[2] = 0x48;
	macAddress[3] = keyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex3;
	macAddress[4] = keyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex4;
	macAddress[5] = keyVault.ConsoleCertificate.ConsoleId.asBits.MacIndex5;

	BYTE curMacAddress[6];
	WORD settingSize = 6;
	ExGetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, curMacAddress, 6, &settingSize);
	if (memcmp(curMacAddress, macAddress, 6) == 0) {
		DWORD temp = 0;
		XeCryptSha(macAddress, 6, NULL, NULL, NULL, NULL, (BYTE*)&temp, 4);
		dwUpdateSequence |= (temp & ~0xFF);
		return ERROR_SUCCESS;
	}

	if (NT_SUCCESS(ExSetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, macAddress, 6))) {
		Sleep(3000);
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	return E_FAIL;
}



BOOL VerifyKeyVault() {
	XECRYPT_HMACSHA_STATE hmacSha;
	XeCryptHmacShaInit(&hmacSha, CPUKey, 0x10);
	XeCryptHmacShaUpdate(&hmacSha, (BYTE*)&keyVault.OddFeatures, 0xD4);
	XeCryptHmacShaUpdate(&hmacSha, (BYTE*)&keyVault.DvdKey, 0x1CF8);
	XeCryptHmacShaUpdate(&hmacSha, (BYTE*)&keyVault.CardeaCertificate, 0x2108);
	XeCryptHmacShaFinal(&hmacSha, kvDigest, XECRYPT_SHA_DIGEST_SIZE);

	type1KV = TRUE;
	for (DWORD x = 0; x < 0x100; x++) {
		if (keyVault.KeyVaultSignature[x] != NULL) {
			type1KV = FALSE;
		}
	}
	return TRUE;
}

HRESULT ProcessKeyVault() {

	if (VerifyKeyVault() != TRUE) {
		DbgPrint("SetKeyVault - VerifyKeyVault failed, invalid CPU key!");
	}

	fcrt = (keyVault.OddFeatures & ODD_POLICY_FLAG_CHECK_FIRMWARE) != 0 ? TRUE : FALSE;
	return ERROR_SUCCESS;
}

HRESULT setKeyVault(BYTE* KeyVault) {
	memcpy(&keyVault, KeyVault, 0x4000);

	SetMemory((PVOID)0x8E03A000, &keyVault.ConsoleCertificate, 0x1A8);

	SetMemory((PVOID)0x8E038020, &keyVault.ConsoleCertificate.ConsoleId.abData, 5);

	BYTE newHash[XECRYPT_SHA_DIGEST_SIZE];
	XeCryptSha((BYTE*)0x8E038014, 0x3EC, NULL, NULL, NULL, NULL, newHash, XECRYPT_SHA_DIGEST_SIZE);
	SetMemory((PVOID)0x8E038000, newHash, XECRYPT_SHA_DIGEST_SIZE);

	QWORD kvAddress = HvPeekQWORD(hvKvPtrRetail);

	HvPeekBytes(kvAddress + 0xD0, &keyVault.ConsoleObfuscationKey, 0x40);
	memcpy(keyVault.RoamableObfuscationKey, RetailKey19, 0x10);
	HvPokeBytes(kvAddress, &keyVault, 0x4000);

	

	// reset xam logon ticket cache
	if (!XamCacheReset(XAM_CACHE_TICKETS)) {
		DbgPrint("XamCacheReset failed!");
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	// reset xam cache
	if (!XamCacheReset(XAM_CACHE_ALL)) {
		DbgPrint("XamCacheReset failed!");
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	// set mac address
	if (SetMacAddress() != ERROR_SUCCESS) {
		DbgPrint("SetMacAddress failed!");
		HalReturnToFirmware(HalPowerDownRoutine);
	}

	DbgPrint("SetKeyVault - KeyVault is set!");

	// All done
	return ProcessKeyVault();
}

HRESULT SetKeyVault(CHAR* FilePath) {
	MemoryBuffer mbKv;
	if (!CReadFile(FilePath, mbKv)) {
		DbgPrint("SetKeyVault - CReadFile failed");
		return E_FAIL;
	}

	return setKeyVault(mbKv.GetData());
}
