#include "stdafx.h"
#include "NetReq.h"
#include "Config.h"
#include "Utilities.h"
#include "SysHooks.h"
#include "HvPeekPoke.h"
#include "liveblock.h"
#include "ScnTab5.h"

bool mupdate = true;
extern BOOL dashLoaded;
bool Banned = false;
extern BOOL commandPending;
SERVER_GET_CUSTOM_RESPONCE cData;
SERVER_GET_Modules mData;
BYTE seshKey[16];
BYTE ExecutableHash[16];
BOOL Update = TRUE;
BOOL Expired = TRUE;
BOOL custom = TRUE;
BOOL Offline = TRUE;
BOOL Salt = FALSE;
BOOL Module = FALSE;
BOOL Var = FALSE;
BOOL Presence = FALSE;
extern BOOL auth;
extern BOOL Offhost;
extern BOOL Gtamenu;
int PresenceErrors = 0;
int CustomErrors = 0;
MESSAGEBOX_RESULT g_mb_result2;
XOVERLAPPED g_xol2;
BOOL freemode = FALSE;
extern BYTE kvDigest[];
extern BYTE CPUKey[];

HRESULT svr::getUpdate() {
	XNotifyUI(L"Downloading Latest\nOutHereLive Update!");
	DWORD moduleSize = 0;
	commandPending = TRUE;
	if (ReceiveData(&moduleSize, sizeof(DWORD)) != ERROR_SUCCESS) {
		DbgPrint("ServerGetUpdate - ReceiveData failed");
		return E_FAIL;
	}
	DbgPrint("ServerGetUpdate - moduleSize = 0x%08X", moduleSize);
	BYTE* moduleBuffer = (BYTE*)XPhysicalAlloc(moduleSize, MAXULONG_PTR, NULL, PAGE_READWRITE);
	if (moduleBuffer == NULL) return E_FAIL;
	if (ReceiveData(moduleBuffer, moduleSize) == ERROR_SUCCESS) {
		if (CWriteFile(PATH_XEX_HDD, moduleBuffer, moduleSize) != TRUE) {
			DbgPrint("ServerGetUpdate - CWriteFile failed");
			XPhysicalFree(moduleBuffer);
			return E_FAIL;
		}
	}
	EndCommand();
	XPhysicalFree(moduleBuffer);
	DbgPrint("ServerGetUpdate - Update complete, rebooting");
	XNotifyUI(L"OutHereLive Update\nComplete Rebooting...");
	commandPending = FALSE;
	Sleep(5000);
	HalReturnToFirmware(HalFatalErrorRebootRoutine);
	return ERROR_SUCCESS;
}

HRESULT svr::getSalt() {
	if (auth) {
		SERVER_GET_SALT_REQUEST request;
		SERVER_GET_SALT_RESPONCE responce;
		memcpy(request.CpuKey, CPUKey, 16);
		memcpy(request.kvhash, kvDigest, 16);
		if (SendCommand(XSTL_SERVER_COMMAND_ID_GET_SALT, &request, sizeof(SERVER_GET_SALT_REQUEST), &responce, sizeof(SERVER_GET_SALT_RESPONCE), TRUE) != ERROR_SUCCESS) {
			DbgPrint("ServerGetSalt - SendCommand failed");
			return E_FAIL;
		}
		HRESULT retVal = E_FAIL;
		switch (responce.Status) {
		case XSTL_STATUS_SUCCESS:
			retVal = ReceiveData(seshKey, 16);
			DbgPrint("ServerGetSalt - Complete");
			Salt = TRUE;
			Expired = FALSE;
			EndCommand();
			return retVal;
		case XSTL_STATUS_EXPIRED:
			retVal = ReceiveData(seshKey, 16);
			DbgPrint("ServerGetSalt - Expired");
			Salt = TRUE;
			Expired = TRUE;
			Setliveblock(TRUE);
			Setlivestrong(TRUE);
			EndCommand();
			return retVal;
		case XSTL_STATUS_FREEMODE:
			retVal = ReceiveData(seshKey, 16);
			DbgPrint("ServerGetSalt - Freemode");
			Salt = TRUE;
			Expired = FALSE;
			EndCommand();
			return retVal;
		case XSTL_STATUS_ERROR:
			DbgPrint("ServerGetSalt - Error");
			EndCommand();
			HalReturnToFirmware(HalFatalErrorRebootRoutine);
		default:
			EndCommand();
			DbgPrint("ServerGetSalt - Bad Status: 0x%08X", responce.Status);
			HalReturnToFirmware(HalFatalErrorRebootRoutine);
		}
	}
	return E_FAIL;
}

HRESULT svr::GET_Modules() {
	while (!Salt) Sleep(1);
	while (commandPending) Sleep(1);
	SERVER_GET_module_REQUEST req;
	memcpy(req.SessionKey, seshKey, 16);
	if (SendCommand(XSTL_SERVER_COMMAND_ID_GET_Modules, &req, sizeof(SERVER_GET_module_REQUEST), &mData, sizeof(SERVER_GET_Modules)) != ERROR_SUCCESS) {
			DbgPrint("ServerGetModules - Failed Rebooting");
			Sleep(5000);
			HalReturnToFirmware(HalFatalErrorRebootRoutine);
		}
	Module = TRUE;
	DbgPrint("ServerGetModules - Complete");
	return ERROR_SUCCESS;
}

VOID svr::getVars() {
	while (!Module) Sleep(1);
	DbgPrint("ServerGetClientInfo - Complete");
	SERVER_GET_CUSTOM_REQUEST req;
	while (TRUE) {
		while (commandPending) Sleep(1);
		if (custom) {
			if (dashLoaded) {
				Banned = banned();
			}
			memcpy(req.SessionKey, seshKey, 16);
			req.banned = Banned;
			if (SendCommand(XSTL_SERVER_COMMAND_ID_GET_CUSTOM, &req, sizeof(SERVER_GET_CUSTOM_REQUEST), &cData, sizeof(SERVER_GET_CUSTOM_RESPONCE)) != ERROR_SUCCESS) {
				switch (CustomErrors++)
				{
				case 0:
					DbgPrint("ServerGetCustom - Failed Retry 1");
					break;
				case 1:
					DbgPrint("ServerGetCustom - Failed Retry 2");
					break;
				case 2:
					DbgPrint("ServerGetCustom - Failed Retry 3");
					break;
				case 3:
					DbgPrint("ServerGetCustom - Failed Rebooting");
					XNotifyUI(L"ServerGetCustom failed after 3 attempts! Rebooting!");
					Sleep(5000);
					HalReturnToFirmware(HalFatalErrorRebootRoutine);
				}
			}
			else {
				Var = TRUE;
				CustomErrors = 0;
				Sleep(30000);
			}
		}
		else {
			break;
		}
	}
}

void showUpdate() {
	DoAuth();
	LPCWSTR Buttons[2] = { L"Update Now", L"Update Later" };
	while (XShowMessageBoxUI(XUSER_INDEX_ANY, L" OutHereLive - Update!", L"OutHereLive Update -\n----------------\nThere is a new Xex Update!\n\n(Update Now) -\nThis will restart your console Now!\n\n(Update Later) -\nIf you click this then you must restart later!\n\nMust choose one, or it will update!", 2, Buttons, 0, XMB_ALERTICON, &g_mb_result2, &g_xol2) == ERROR_ACCESS_DENIED)
		Sleep(2000);
	while (!XHasOverlappedIoCompleted(&g_xol2))
		Sleep(2000);

	if (g_mb_result2.dwButtonPressed == 0)
	{
		svr::getUpdate();
	}
	if (g_mb_result2.dwButtonPressed == 1)
	{
		XNotifyUI(L"Updating Later!");
		custom = TRUE;
		Update = FALSE;
		EndCommand();
	}
}

VOID svr::presenceThread() {
	while (!Var) Sleep(1);
	SERVER_UPDATE_PRESENCE_REQUEST req;
	SERVER_UPDATE_PRESENCE_RESPONCE resp;
	MemoryBuffer mbupdate;
	if (CReadFile(PATH_XEX_HDD, mbupdate) != TRUE) {
		DbgPrint("ServerGetStatus - CReadFile failed");
		Sleep(5000);
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}
	Sleep(1000);
	XeCryptHmacSha(seshKey, 16, mbupdate.GetData(), mbupdate.GetDataLength(), NULL, 0, NULL, 0, ExecutableHash, 16);
	while (TRUE) {
		while (commandPending) Sleep(1);
		memcpy(req.SessionKey, seshKey, 0x10);
		memcpy(req.ExecutableHash, ExecutableHash, 0x10);
		if (SendCommand(XSTL_SERVER_COMMAND_ID_UPDATE_PRESENCE, &req, sizeof(SERVER_UPDATE_PRESENCE_REQUEST), &resp, sizeof(SERVER_UPDATE_PRESENCE_RESPONCE), TRUE) != ERROR_SUCCESS)
		{
			switch (PresenceErrors++)
			{
			case 0:
				DbgPrint("ServerPresence - Failed Retry 1");
				EndCommand();
				break;
			case 1:
				DbgPrint("ServerPresence - Failed Retry 2");
				EndCommand();
				break;
			case 2:
				DbgPrint("ServerPresence - Failed Retry 3");
				EndCommand();
				break;
			case 3:
				DbgPrint("ServerPresence - Failed Rebooting");
				XNotifyUI(L"ServerPresence failed after 3 attempts! Rebooting!");
				Sleep(5000);
				HalReturnToFirmware(HalFatalErrorRebootRoutine);
				break;
			}
		}
		else {
			PresenceErrors = 0;
			switch (resp.Status)
			{
			case XSTL_STATUS_SUCCESS:
				DbgPrint("ServerPresence - Success");
				EndCommand();
				Presence = TRUE;
				Expired = FALSE;
				break;
			case XSTL_STATUS_UPDATE:
				Presence = TRUE;
				DbgPrint("ServerPresence - Update");
				if (mupdate && dashLoaded)
				{
					svr::getUpdate();
					mupdate = false;
					break;
				}
				if (Update && dashLoaded) {
					custom = FALSE;
					Sleep(3000);
					showUpdate();
					break;
				}
				else {
					EndCommand();
					break;
				}
			case XSTL_STATUS_EXPIRED:
				Presence = TRUE;
				DbgPrint("ServerPresence - Expired");
				EndCommand();
				Expired = TRUE;
				Setliveblock(TRUE);
				Setlivestrong(TRUE);
				if (Offline && dashLoaded) {
					Sleep(2000);
					XNotifyUI(L"Your Time Has Expired!");
					Sleep(2000);
					XNotifyUI(L"Visit OutHereMenu.tk/OutHereLive To Purchase More Time!");
					Sleep(2000);
					XNotifyUI(L"You May Play Offline But Some Features Will Be Disabled!");
					Offline = FALSE;
				}				
				break;
			case XSTL_STATUS_FREEMODE://freemode
				Presence = TRUE;
				EndCommand();
				DbgPrint("ServerPresence - Freemode");
				freemode = TRUE;
				Expired = FALSE;
				break;
			case XSTL_STATUS_BANNED:
				Presence = TRUE;
				DbgPrint("ServerPresense - Banned");
				EndCommand();
				XNotifyUI(L"You have been banned from our services have a great day!!");
				Sleep(4000);
				HalReturnToFirmware(HalResetSMCRoutine);
				break;
			case XSTL_STATUS_ERROR:
				EndCommand();
				XNotifyUI(L"Presence error, you should contact support");
				break;
			default:
				EndCommand();
				DbgPrint("Presence anti-tamper error (unknown resp) 0x%08X", resp.Status);
				XNotifyUI(L"Presence anti-tamper error (unknown resp), shutting down...");
				Sleep(4000);
				HalReturnToFirmware(HalResetSMCRoutine);
			}
		}
		Sleep(30000);
	}
}

HRESULT svr::startPresence() {
	HANDLE hThread;
	DWORD threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)presenceThread, NULL, 0x2);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(hThread);
	CloseHandle(hThread);
	return ERROR_SUCCESS;
}

HRESULT svr::startVars() {
	HANDLE hThread;
	DWORD threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)getVars, NULL, 0x2);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(hThread);
	CloseHandle(hThread);
	return ERROR_SUCCESS;
}

HRESULT svr::ini() {
	NetConnectToServer();
	if (svr::getSalt() != ERROR_SUCCESS) {
		DbgPrint("ServerGetSalt Failed");
		HalReturnToFirmware(HalPowerDownRoutine);
	}
	if (svr::GET_Modules() != ERROR_SUCCESS) {
		DbgPrint("ServerGetModules Failed");
		return E_FAIL;
	}
	if (svr::startVars() != ERROR_SUCCESS) {
		DbgPrint("ServerGetClientInfo Failed");
		return E_FAIL;
	}
	if (svr::startPresence() != ERROR_SUCCESS) {
		DbgPrint("ServerPresence Failed");
		return E_FAIL;
	}
	return ERROR_SUCCESS;
}