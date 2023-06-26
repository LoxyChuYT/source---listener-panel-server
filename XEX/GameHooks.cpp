#include "stdafx.h"
#include "Config.h"
#include "Utilities.h"
#include "GameHooks.h"
#include "SysHooks.h"
#include "NetConnect.h"
#include <xbdm.h>
#include <time.h>  
#include "HudHook.h"
#include "liveblock.h"
#include "Aimbot.h"
#include "B03.h"
#include "INIReader.h"

extern BOOL mspspoof;
extern BOOL gtabypass;
extern BOOL awbypass;
extern BOOL bo3bypass;
extern BOOL bo2bypass;
extern BOOL ghostbypass;
extern BOOL offhost;
extern HANDLE hXam;
BOOL dashLoaded;

VOID __cdecl APCWorker(void* Arg1, void* Arg2, void* Arg3) {
	// Call our completion routine if we have one
	if (Arg2)
		((LPOVERLAPPED_COMPLETION_ROUTINE)Arg2)((DWORD)Arg3, 0, (LPOVERLAPPED)Arg1);
}

DWORD XSecurityCreateProcessHook(DWORD dwHardwareThread) {
	return ERROR_SUCCESS;
}

VOID XSecurityCloseProcessHook() {}

DWORD XSecurityVerifyHook(DWORD dwMilliseconds, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
	// Queue our completion routine
	if (lpCompletionRoutine)
		NtQueueApcThread((HANDLE)-2, (PIO_APC_ROUTINE)APCWorker, lpOverlapped, (PIO_STATUS_BLOCK)lpCompletionRoutine, 0);

	// All done
	return ERROR_SUCCESS;
}

DWORD XSecurityGetFailureInfoHook(PXSECURITY_FAILURE_INFORMATION pFailureInformation) {
	if (pFailureInformation->dwSize != 0x18) return ERROR_NOT_ENOUGH_MEMORY;
	pFailureInformation->dwBlocksChecked = 0;
	pFailureInformation->dwFailedReads = 0;
	pFailureInformation->dwFailedHashes = 0;
	pFailureInformation->dwTotalBlocks = 0;
	pFailureInformation->fComplete = TRUE;
	return ERROR_SUCCESS;
}

DWORD XexGetProcedureAddressHook(HANDLE hand, DWORD dwOrdinal, PVOID* pvAddress) {
	if (hand == hXam) {
		switch (dwOrdinal) {
		case 0x9BB:
			*pvAddress = XSecurityCreateProcessHook;
			return 0;
		case 0x9BC:
			*pvAddress = XSecurityCloseProcessHook;
			return 0;
		case 0x9BD:
			*pvAddress = XSecurityVerifyHook;
			return 0;
		case 0x9BE:
			*pvAddress = XSecurityGetFailureInfoHook;
			return 0;
		}
	}
	return XexGetProcedureAddress(hand, dwOrdinal, pvAddress);
}

unsigned long XexGetModuleHandleHook(char* ModuleName) {
	INIReader reader("HDD:\\launch.ini");
	std::string temp1 = reader.Get("Plugins", "plugin1", "UNKNOWN");
	const char* plugin1 = temp1.c_str();
	std::string temp2 = reader.Get("Plugins", "plugin2", "UNKNOWN");
	const char* plugin2 = temp2.c_str();
	std::string temp3 = reader.Get("Plugins", "plugin3", "UNKNOWN");
	const char* plugin3 = temp3.c_str();
	std::string temp4 = reader.Get("Plugins", "plugin4", "UNKNOWN");
	const char* plugin4 = temp4.c_str();
	std::string temp5 = reader.Get("Plugins", "plugin5", "UNKNOWN");
	const char* plugin5 = temp5.c_str();
	if (strcmp(ModuleName, plugin1) == 0) {
		DbgPrint("Plugin 1 check success!");
		return 0xC0000225; //returning 0 flags you....
	}
	if (strcmp(ModuleName, plugin2) == 0) {
		DbgPrint("Plugin 2 check success!");
		return 0xC0000225; //returning 0 flags you....
	}
	if (strcmp(ModuleName, plugin3) == 0) {
		DbgPrint("Plugin 3 check success!");
		return 0xC0000225; //returning 0 flags you....
	}
	if (strcmp(ModuleName, plugin4) == 0) {
		DbgPrint("Plugin 4 check success!");
		return 0xC0000225; //returning 0 flags you....
	}
	if (strcmp(ModuleName, plugin5) == 0) {
		DbgPrint("Plugin 5 check success!");
		return 0xC0000225; //returning 0 flags you....
	}
	else return (unsigned long)GetModuleHandle(ModuleName);
}

// microsoft point spoof
BYTE patchmsp1[28] = { 0x38, 0x80, 0x00, 0x05, 0x80, 0x63, 0x00, 0x1C, 0x90, 0x83, 0x00, 0x04, 0x38, 0x80, 0x05, 0x39, 0x90, 0x83, 0x00, 0x08, 0x38, 0x60, 0x00, 0x00, 0x4E, 0x80, 0x00, 0x20 };
BYTE noppatch[4] = { 0x60, 0x00, 0x00, 0x00 };
BYTE patchmsp2[4] = { 0x48, 0x00, 0x00, 0xC8 };
BYTE zeropatch[4] = { 0x39, 0x60, 0x00, 0x00 };

VOID MSPSpoof() {
	SetMemory((PVOID)0x8168A658, patchmsp1, 28);
	SetMemory((PVOID)0x818E8F54, noppatch, 4);
	SetMemory((PVOID)0x818E928C, patchmsp2, 4);
	SetMemory((PVOID)0x818EDAD4, zeropatch, 4);
	SetMemory((PVOID)0x9015C108, noppatch, 4);
	XNotifyUI(L"MS Points Spoofing Successful!");
}

typedef enum _BO2_GameMode
{
	PUBLIC_MATCH = 0,
	PRIVATE_MATCH = 1,
	LOCAL_SPLITSCREEN = 2,
	WAGER_MATCH = 3,
	BASIC_TRAINING = 4,
	THEATER = 5,
	LEAGUE_MATCH = 6,
	RTS = 7,
	MAX = 8,
	INVALID = 9,
} BO2_GameMode;
static VOID __declspec(naked) BO2_AntiCheat_GiveProbation_Stub(DWORD dwUserIndex, BO2_GameMode gameMode, FLOAT probationTime) {
	__asm {
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		li r6, 0xAA5
	}
}

VOID BO2_AntiCheat_GiveProbation(DWORD dwUserIndex, BO2_GameMode gameMode, FLOAT probationTime) {
	probationTime = 0.0f;
	BO2_AntiCheat_GiveProbation_Stub(dwUserIndex, gameMode, probationTime);
}

typedef enum _BO2_ChallengeFlag : WORD
{
	CHECKSUM_CRC32 = 1,
	CHECKSUM_SHA = 2,
	CHECKSUM_CRC32_SPLIT = 3,
	DVAR = 4,
	DVAR_FLOAT = 5,
	EXPRESSION = 6,
	NOP1 = 7,
	MIPSTER = 8,
} BO2_ChallengeFlag;

typedef struct BO2_ChallengeArgument;
typedef struct BO2_Referencable;

template<typename T> struct BO2_ArgumentArray
{
	T* m_data;
	DWORD m_capacity, m_size;
};

template<typename T> struct BO2_ArgumentReference
{
	T* m_ptr;
};

typedef struct BO2_ReferencableVTable
{
	VOID *(__thiscall *__vecDelDtor)(BO2_Referencable* test, DWORD);
};

typedef struct BO2_Referencable
{
	BO2_ReferencableVTable* vfptr;
	volatile DWORD m_refCount;
};

typedef struct __declspec(align(2)) BO2_ByteBuffer : BO2_Referencable
{
	DWORD m_size;
	PCHAR m_data;
	PCHAR m_readPtr;
	PCHAR m_writePtr;
	BOOL m_typeChecked;
	BOOL m_typeCheckedCopy;
	BOOL m_allocatedData;
};

typedef struct BO2_ChallengeArgumentVTable {
	VOID *(__thiscall *__vecDelDtor)(BO2_ChallengeArgument* test, DWORD);
	BOOL(__thiscall *deserialize)(BO2_ChallengeArgument* test, BO2_ArgumentReference<BO2_ByteBuffer>);
	DWORD(__thiscall *sizeOf)(BO2_ChallengeArgument* test);
};

typedef struct BO2_ChallengeArgument {
	BO2_ChallengeArgumentVTable* vfptr;
	QWORD m_data;
	DWORD m_size;
};

BOOL BO2_AntiCheat_GetChallengeResponse(DWORD dwUserIndex, WORD FuncID, BO2_ArgumentArray<BO2_ChallengeArgument>* Arguments, QWORD* Response) {
	if (Arguments->m_data->m_size)
	{
		if (FuncID == BO2_ChallengeFlag::CHECKSUM_CRC32_SPLIT)
			*Response = 0x5F11FA86997C35DB;
		else if (FuncID == BO2_ChallengeFlag::DVAR || FuncID == BO2_ChallengeFlag::NOP1)
			*Response = FALSE;
		else if (FuncID == BO2_ChallengeFlag::MIPSTER)
			*Response = 0xB;
		else
		{
			XLaunchNewImage(NULL, NULL);
		}
	}
	return TRUE;
}

int XNetXnAddrToMachineIdHook(XNCALLER_TYPE xnc, XNADDR pxnaddr, unsigned long long MachineId)
{
	srand((unsigned int)time(0));
	MachineId = 0xFA00000000000000 | (0x2000000 | rand() % 0x7FFFFF);
	return 0;
}

inline __declspec() bool Live_GetConsoleDetailsSavestub(unsigned char internalIP[4], unsigned char onlineIP[4], unsigned long long *machineIDH, unsigned long long *null, unsigned long long *enet)
{
	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}

bool Live_GetConsoleDetailsHookAW(unsigned char internalIP[4], unsigned char onlineIP[4], unsigned long long *machineIDH, unsigned long long *null, unsigned long long *enet)
{
	srand(time(0));
	int iTargetAddress = 0;
	__asm mflr      iTargetAddress
	if (iTargetAddress == 0x822C9FF8 || iTargetAddress == 0x822C9908)
	{
		for (int i = 0; i < 4; i++)
		{
			internalIP[i] = rand() % 0xFF; onlineIP[i] = rand() % 0xFF;
		}
		*enet = 0x001DD8000000 | rand() % 0x7FFFFF;
		return true;
	}
	return Live_GetConsoleDetailsSavestub(internalIP, onlineIP, machineIDH, null, enet);
}

bool Live_GetConsoleDetailsHook(unsigned char internalIP[4], unsigned char onlineIP[4], unsigned long long *machineIDH, unsigned long long *null, unsigned long long *enet) {
	srand(time(0));
	int iTargetAddress = 0;
	__asm mflr iTargetAddress
	if (iTargetAddress == 0X822EC08C || iTargetAddress == 0X822EB7C4) {
		for (int i = 0; i < 4; i++) {
			internalIP[i] = rand() % 0xFF; onlineIP[i] = rand() % 0xFF;
		}
		*enet = 0x001DD8000000 | rand() % 0x7FFFFF;
		return true;
	}
	return Live_GetConsoleDetailsSavestub(internalIP, onlineIP, machineIDH, null, enet);
}

DWORD NetDll_XNetXnAddrToMachineIdHook(DWORD XNC, const XNADDR * pXNAddr, QWORD * pqwMachineID)
{
	srand((unsigned)time(NULL));
	*(QWORD*)pqwMachineID = 0xFA00000000000000 | (0x2000000 | rand() % 0x7FFFFF);
	return 0;
}

long XeKeysGetConsoleIDHook(unsigned char* buffer, int Unknown) {
	srand((unsigned int)time(0));
	for (int i = 0x00; i < 0xC; i++) buffer[i] = rand() % 0x7F;
	return 0;
}

NTSTATUS GhostsXeKeysGetKeyHook(WORD KeyId, PVOID KeyBuffer, PDWORD keyLength)
{
	if (KeyId == 0x14)
	{
		XUSER_SIGNIN_INFO userInfo;
		XamUserGetSigninInfo(0, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &userInfo);

		BYTE gamertagSha[0x10];
		XeCryptSha((PBYTE)userInfo.szUserName, strlen(userInfo.szUserName), NULL, NULL, NULL, NULL, gamertagSha, 0x10);
		BYTE temp = 0;
		for (int i = 0; i < 0xC; i++) {
			temp = (gamertagSha[i] & 0xE) + '0';
			SetMemory(&((BYTE*)(KeyBuffer))[i], &temp, 1);
		}
	}
	else return XeKeysGetKey(KeyId, KeyBuffer, keyLength);
	return STATUS_SUCCESS;
}

VOID dashtext() {
	SetMemory((LPVOID)0x92BAF010, "OutHere 17526 ", 14);
	SetMemory((LPVOID)0x92BAEFFC, "Pick Your Poison   ", 19);
	SetMemory((LPVOID)0x92D2DA2E, "OpenBitch", 9);
	SetMemory((LPVOID)0x92D2D9DC, "CloseBitch", 10);
	SetMemory((LPVOID)0x92E3F863, "Rekt By Emodz ", 14);
}

typedef HRESULT(*pXamInputGetState)(QWORD r3, QWORD r4, QWORD r5);
pXamInputGetState XamInputGetState = (pXamInputGetState)ResolveFunction(NAME_XAM, 401);

static BOOL isFrozen = FALSE;
HRESULT XamInputGetStateHook(QWORD r3, QWORD r4, QWORD r5) {
	if (isFrozen) {
		return 0;
	}
	HRESULT ret = XamInputGetState(r3, r4, r5);
	return ret;
}

//xbox kernel's bypasses
void INIT_BO2_CMD() {
	while (XamGetCurrentTitleId() == BO2) {
		*(DWORD*)0x82497EB0 = 0x60000000;
		*(DWORD*)0x8259A65C = 0x60000000;
		*(DWORD*)0x825BEA7C = 0x60000000;
		*(DWORD*)0x825C6070 = 0x60000000;
		Sleep(300000);
	}
}


//Code:
void INIT_GHOST_CMD() {
	while (XamGetCurrentTitleId() == GHOSTS) {
		*(DWORD*)0x8260B178 = 0x60000000;
		*(DWORD*)0x8414A1D4 = 0;
		Sleep(300000);
		
	}
}


//Code:
void INIT_AW_CMD() {
	while (XamGetCurrentTitleId() == AW) {
		*(DWORD*)0x822E4DF0 = 0x60000000;
		*(DWORD*)0x84DC5FE4 = 0;
		Sleep(300000);
	}
}

XEX_EXECUTION_ID* pExecutionId;
int Color2 = 0xFFAEB234;//AEB234
VOID InitializeGameHooks(PLDR_DATA_TABLE_ENTRY ModuleHandle) {
	PatchModuleImport(ModuleHandle, NAME_KERNEL, 407, (DWORD)XexGetProcedureAddressHook);
	PatchModuleImport(ModuleHandle, NAME_KERNEL, 408, (DWORD)XexLoadExecutableHook);
	PatchModuleImport(ModuleHandle, NAME_KERNEL, 409, (DWORD)XexLoadImageHook);
	PatchModuleImport(ModuleHandle, NAME_XAM, 401, (DWORD)XamInputGetStateHook);

	pExecutionId = (XEX_EXECUTION_ID*)RtlImageXexHeaderField(ModuleHandle->XexHeaderBase, 0x00040006);

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"hud.xex") == 0) {
		InitializeHudHooks(ModuleHandle);
	}

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"vk.xex") == 0) {
		InitializeVkHooks(ModuleHandle);
	}

	if (pExecutionId == 0) return;

	if (XamGetCurrentTitleId() == 0xFFFE07D1) {
		dashLoaded = TRUE;
		if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"dash.xex") == 0) {
			dashtext();
			SetDashUI();
			SetHUDUI();
		}
	}
	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"dash.social.lex") == 0) {

		//more tabs
		*(int*)0x9AFC4876 = Color2;
		
	}

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"Guide.MP.Purchase.xex") == 0) {
		if (mspspoof) {
			MSPSpoof();
		}
	}

	switch (pExecutionId->TitleID) {
	case BO2: {
		if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"default_mp.xex") == 0)
		{
			if (bo2bypass) {
				// TU18 Machine ID From XeX
				PatchModuleImport("default_mp.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
				PatchModuleImport("default_mp.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
				//NOP
				BYTE Data[] = { 0x60, 0x00, 0x00, 0x00 };
				// probation bypass
				memcpy((PVOID*)0x825C6070, Data, 4);
				// disable challenge log check
				memcpy((BYTE*)0x8259A65C, Data, 4);
				// disable call to protections
				memcpy((BYTE*)0x82497EB0, Data, 4);
				// cheat
				memcpy((BYTE*)0x82497F30, Data, 4);
				// write
				memcpy((BYTE*)0x82497EE0, Data, 4);
				// read
				memcpy((BYTE*)0x82497EC8, Data, 4);
				// ban 1
				memcpy((BYTE*)0x82599680, Data, 4);
				// ban 2
				memcpy((BYTE*)0x82599670, Data, 4);
				// ban 3
				memcpy((BYTE*)0x82599628, Data, 4);
				// ban 4
				memcpy((BYTE*)0x8259964C, Data, 4);
				// ban Checks
				memcpy((BYTE*)0x825996AC, Data, 4);
				// console Checks
				memcpy((BYTE*)0x825996B4, Data, 4);
				HookFunctionStart((PDWORD)0x825C5330, (PDWORD)BO2_AntiCheat_GiveProbation_Stub, (DWORD)BO2_AntiCheat_GiveProbation); // TU18
				Sleep(50);
				PatchInJump((PDWORD)0x8259A5E0, (DWORD)BO2_AntiCheat_GetChallengeResponse, false); // TU18
				INIT_BO2_CMD();
			}
			if (offhost) {
				HANDLE hThread;
				DWORD threadId;
				ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)BO2_Start, NULL, 0x2 | CREATE_SUSPENDED);
				XSetThreadProcessor(hThread, 4);
				SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
				ResumeThread(hThread);
				CloseHandle(hThread);
			}
		}
		break;
	}case AW: {
		if (awbypass) 
		{
			Sleep(500);
			PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
			PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
			HookFunctionStart((PDWORD)0x8233B018, (PDWORD)Live_GetConsoleDetailsSavestub, (DWORD)Live_GetConsoleDetailsHook);
			// hides xbdm
			*(long long*)0x822CA190 = 0x8921005060000000;
			// patch branch to not set dev flag
			*(int*)0x822CA18C = 0x48000010;
			// patch subfunction to check xbdm
			*(int*)0x822CA184 = 0x38600000;
			// return answer challenges correctly
			*(int*)0x822CA0EC = 0x3920331C;
			// prevent blacklist in console details
			*(PBYTE)(0x8233B0E4 + 0x03) = 0x00;
			INIT_AW_CMD();
		}

		break;
	}case BO3: {
		if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"default.xex") == 0)
		{
			if (bo3bypass)
			{
				PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
				PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
				Sleep(50);
				*(DWORD*)0x82665268 = 0x60000000;						// nop link register TU8
				*(DWORD*)0x825889DC = 0x60000000;						// nop challenge TU8
				*(DWORD*)0x825889E4 = 0x400;							// respond to challenge TU8
				*(__int64*)0x82332018 = 0x386000014E800020;				//TU8
				*(DWORD*)0x826B81D0 = 0x60000000;						//TU8
				
			}
			if (offhost) {
				HANDLE hThread;
				DWORD threadId;
				ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)BO3_Start, NULL, 0x2 | CREATE_SUSPENDED);
				XSetThreadProcessor(hThread, 4);
				SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
				ResumeThread(hThread);
				CloseHandle(hThread);
			}
		}
		break;
	}case GHOSTS: {
		if (ghostbypass) {
			PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
			PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
			// init bypass
			*(int*)0x826276D4 = 0x48000010;
			*(int*)0x826276CC = 0x38600000;
			*(int*)0x82627670 = 0x3920331C;
			//bypass
			*(int*)0x8262763C = 0x48000020;
			*(int*)0x82627658 = 0x48000020;
			*(int*)0x8262767C = 0x48000020;
			*(int*)0x826276D4 = 0x48000020;
			*(int*)0x8272E428 = 0x38600000;
			*(int*)0x8262A5D0 = 0x39200000;
			*(int*)0x826276DC = 0x39200003;
			*(long long*)(0x826276D8) = 0x8921005060000000;
			*(long long*)(0x8214ABBC) = 0x3800002244000002;
			*(int*)0x82627614 = 0x39200009;
			*(int*)0x82627628 = 0x38600000;
			*(int*)0x82627634 = 0x39600001;
			*(int*)0x82627650 = 0x38600002;
			*(int*)0x8262767C = 0x48000010;
			*(int*)0x826276D4 = 0x48000010;
			*(long long*)(0x82627684) = 0x8921005061490000;
			*(long long*)(0x826276DC) = 0x8921005061490000;
			*(int*)0x82265384 = 0x39200001;
			*(int*)0x8226D2B4 = 0x60000000;
			*(int*)0x82266448 = 0x60000000;
			*(int*)0x8226C874 = 0x60000000;
			*(int*)0x8226BB74 = 0x60000000;
			*(int*)0x82290494 = 0x38C00005;
			*(int*)0x822C9344 = 0x60000000;
			*(int*)0x8262A5D0 = 0x39200000;
			PatchInJump((PDWORD)0x81A74DB4, (DWORD)GhostsXeKeysGetKeyHook, false);
			PatchInJump((PDWORD)0x81A74FE4, (DWORD)XeKeysGetConsoleIDHook, false);
			INIT_GHOST_CMD();
			// cheat
			*(DWORD*)0x82627628 = 0x38600000;
			*(DWORD*)0x82627634 = 0x39600001;
			*(DWORD*)0x82627650 = 0x38600002;
		}
		if (offhost) {
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)GO_Start, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		break;
	}
	case GTAV: {
		if (gtabypass) {
			PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
			PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
			*(BYTE*)0x82CF782B = 1; //Cash Drop Bypass
			*(DWORD*)0x83288A30 = 0x48000104; //Script Bypass
			*(QWORD*)0x838B60F4 = 0x00000422F6D6AA59;
			*(DWORD*)0x82FDB57C = 0x3FC0022C;
			*(DWORD*)0x82FDB580 = 0x63DEC800;
			*(DWORD*)0x82FDB584 = 0x93DD0018;
			*(DWORD*)0x82FDB588 = 0x3C60838B;
			*(DWORD*)0x82FDB58C = 0x606360F4;
			*(DWORD*)0x82FDB590 = 0xE8630000;
			*(DWORD*)0x82FDB594 = 0x907D001C;
			*(DWORD*)0x830DD6A0 = 0x60000000; //Host kick protection
		}
		break;
	}
	case MW2: {
		PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
		PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);

		if (offhost) {
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)MW2_Start, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		break;
	}
	case MW3: {
		PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
		PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
		if (offhost) {
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)MW3_Start, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		break;
	}
	case BO1: {
		PatchModuleImport("default.xex", NAME_XAM, 64, (DWORD)NetDll_XNetXnAddrToMachineIdHook);
		PatchModuleImport("default.xex", NAME_KERNEL, 0x195, (DWORD)XexGetModuleHandleHook);
		if (offhost) {
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)BO1_Start, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		break;
	}
	case 0xFFFF0055: // Xex Menu
	case 0xC0DE9999: // Xex Menu alt
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
	case 0x1CED2911: // pcsxr
	case 0xFFED7300: // FCE360
	case 0x00FBAFBA: // DSon360
	case 0x000003D0: //3dox
	{
		break;
	}
	}
}