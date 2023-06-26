#include "stdafx.h"
#include "Utilities.h"
#include "HvPeekPoke.h"
#include "SysHooks.h"
#include "NetReq.h"
#include "Config.h"
#include "NetReq.h"
#include "NetConnect.h"
#include <time.h>  
#include "NetReq.h"
#include "liveblock.h"
#include "HudHook.h"
#include "INISettings.h"

MESSAGEBOX_RESULT result;
XOVERLAPPED overlapped;
HANDLE hOutHereLive = NULL;
HANDLE hXam = NULL;
HANDLE dllHandle;
extern BOOL dashLoaded;
extern BOOL freemode;
extern BOOL Expired;
bool Initialized = false;
CHAR WelcomeMsgBuffer[256];
WCHAR WelcomeMsg[256];
CHAR WelcomeMsgBuffer1[256];
WCHAR WelcomeMsg1[256];
extern BYTE ckey[];
extern BYTE CPUKey[];

void welcome() {
	DoAuth();
	if (freemode) {
		sprintf(WelcomeMsgBuffer1, "Welcome %s To OutHereLive We Are In Freemode\n\nYou Have Been On Your Current Kv For %i Days %i Hours %i Minutes", cData.name, cData.kvdays, cData.kvhours, cData.kvminutes);
	}
	else if (Expired) {
		sprintf(WelcomeMsgBuffer1, "Welcome %s To OutHereLive Your Time Has Expired", cData.name);
	}
	else if (cData.days >= 500) {
		sprintf(WelcomeMsgBuffer1, "Welcome %s To OutHereLive You Have Lifetime Remaining\n\nYou Have Been On Your Current Kv For %i Days %i Hours %i Minutes", cData.name, cData.kvdays, cData.kvhours, cData.kvminutes);
	}
	else {
		sprintf(WelcomeMsgBuffer1, "Welcome %s To OutHereLive Live You Have %i Days %i Hours %i Minutes Of Time Remaining\n\nYou Have Been On Your Current Kv For %i Days %i Hours %i Minutes", cData.name, cData.days, cData.hours, cData.minutes, cData.kvdays, cData.kvhours, cData.kvminutes);
	}
	mbstowcs(WelcomeMsg1, WelcomeMsgBuffer1, strlen(WelcomeMsgBuffer1) + 1);
	LPCWSTR Buttons[1] = { L"Continue" };
	while (XShowMessageBoxUI(XUSER_INDEX_ANY, L"OutHereLive", WelcomeMsg1, 1, Buttons, 0, XMB_ALERTICON, &result, &overlapped) == ERROR_ACCESS_DENIED)
		Sleep(2000);
	while (!XHasOverlappedIoCompleted(&overlapped))
		Sleep(2000);

	if (result.dwButtonPressed == 0)
	{
	}
}

VOID waitForDash() {
	while (!dashLoaded) Sleep(1);
	Sleep(5000);
	HANDLE hThread;
	DWORD threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)welcome, NULL, 0x2);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(hThread);
	CloseHandle(hThread);
}

void decryptxzp() {
	PVOID Aidz;
	DWORD SecondLotOfAidz;
	BYTE EncryptionKey[0x10] = { 0x2D, 0x3C, 0x6A, 0xFF, 0x51, 0x74, 0x51, 0x71, 0x26, 0x26, 0x6D, 0xAD, 0x64, 0x98, 0x48, 0xA6 };
	XGetModuleSection(hOutHereLive, "816ED6E0", &Aidz, &SecondLotOfAidz);
	XeCryptRc4(EncryptionKey, 0x10, (PBYTE)Aidz, SecondLotOfAidz);
}

VOID Initialize() {
	if (XboxKrnlVersion->Build != 17526) {
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	if (CreateSymbolicLink(DRIVE_HDD, DEVICE_NAME_HDD, TRUE) != ERROR_SUCCESS) {
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	remove(PATH_LOG_HDD);
	DbgPrint("Log Start:");
	DbgPrint("Created Symbolic Link");
	hOutHereLive = GetModuleHandle(NAME_XEX);
	hXam = GetModuleHandle(NAME_XAM);
	if (!hOutHereLive) {
		DbgPrint("Incorrect Module Name");
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	if (hOutHereLive) {
		Setliveblock(TRUE);
		Setlivestrong(TRUE);
		
		if (InitializeHvPeekPoke() != ERROR_SUCCESS) {
			DbgPrint("Hv Poke Failed");
			HalReturnToFirmware(HalPowerDownRoutine);
		}

		if (InitializeSystemHooks() != TRUE) {
			DbgPrint("Failed to Initialize System Hooks");
			HalReturnToFirmware(HalPowerDownRoutine);
		}

		if (ProcessCPUKey() != ERROR_SUCCESS) {
			DbgPrint("Failed to Process Cpu Key");
			HalReturnToFirmware(HalPowerDownRoutine);
		}

		if (FileExists(PATH_KEYVAULT_HDD)) {
			DbgPrint("Kv Loaded From: %s", PATH_KEYVAULT_HDD);
			if (SetKeyVault(PATH_KEYVAULT_HDD) != ERROR_SUCCESS) {
				DbgPrint("%s Load Failed", PATH_KEYVAULT_HDD);
				HalReturnToFirmware(HalPowerDownRoutine);
			}
		}
		else {
			DbgPrint("Kv Loaded From Console");
			PBYTE kv = (PBYTE)malloc(0x4000);
			HvPeekBytes(HvPeekQWORD(0x0000000200016240), kv, 0x4000);
			if (setKeyVault(kv) != ERROR_SUCCESS) {
				DbgPrint("Console Kv Load Failed");
				free(kv);
				HalReturnToFirmware(HalPowerDownRoutine);
			}
			free(kv);
		}
		//decryptxzp();
		if (svr::ini() != ERROR_SUCCESS) {
			DbgPrint("Failed To Start Server Connection");
			HalReturnToFirmware(HalPowerDownRoutine);
		}

		IniSettings();

		setcolornotify();
		//17526
		//Xam Patches
		*(DWORD*)(0x816824A4) = 0x60000000; //nop EvaluateContent Serial Check
		*(DWORD*)(0x8167984C) = 0x60000000; //nop MmGetPhysicalAddress For Challenge
		*(DWORD*)(0x8167F8D8) = 0x38600000; //XContent::ContentEvaluateLicense
		*(DWORD*)(0x8167C414) = 0x38600000; //XeKeysVerifyRSASignature
		*(DWORD*)(0x8192D670) = 0x60000000; //ProfileEmbeddedContent::Validate remove sig check
		//Gold Spoof
		*(DWORD*)(0x816DAAFC) = 0x38600006;
		*(DWORD*)(0x81A3BCB8) = 0x38600001;
		*(DWORD*)(0x816DD640) = 0x39600001;
		*(DWORD*)(0x816DD6B4) = 0x39600001;
		*(DWORD*)(0x816DD6AC) = 0x39600001;
		*(DWORD*)(0x816DD6A0) = 0x39600001;
		DWORD li = 0x38600001;
		li += 5;
		SetMemory((LPVOID)(0x816DAAC0 + 0x34), &li, 4);
		SetMemory((LPVOID)(0x816DAB20 + 0x38), &li, 4);

		if (!Expired) {
			Setliveblock(FALSE);
			Setlivestrong(FALSE);
		}
		HANDLE hThread;
		DWORD threadId;
		ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)waitForDash, NULL, 0x2);
		XSetThreadProcessor(hThread, 4);
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(hThread);
		CloseHandle(hThread);

		Initialized = true;
	}
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD res, LPVOID lpR) {
	if (res == DLL_PROCESS_ATTACH) {
		dllHandle = hModule;
		if (!IsTrayOpen()) {
			Initialize();
		}
		else {
			if (CreateSymbolicLink(DRIVE_HDD, DEVICE_NAME_HDD, TRUE) != ERROR_SUCCESS) {
				HalReturnToFirmware(HalPowerDownRoutine);
			}
			Setliveblock(TRUE);
			Setlivestrong(TRUE);
		}
		return TRUE;
	}
}