#include "stdafx.h"
#include "ScnTab5.h"
#include "Utilities.h"
#include "NetReq.h"

extern MESSAGEBOX_RESULT result;
extern XOVERLAPPED overlapped;
extern BYTE CPUKey[];
WCHAR NameRespmsg[256];
CHAR NameRespmsgBuffer[256];
CHAR HUDMsgBuffer[256];
WCHAR HUDMsg[256];
INT nameMaxLen = 15;

//token
HRESULT RedeemToken(const char *Token)
{
	SERVER_GET_TOKEN_REDEEM_REQUEST req;
	SERVER_GET_TOKEN_REDEEM_RESPONSE resp;

	memcpy(req.SessionKey, seshKey, 0x10);
	memcpy(req.Token, Token, 0x0E);
	memcpy(req.CpuKey, CPUKey, 0x10);

	if (SendCommand(XSTL_SERVER_COMMAND_ID_TOKEN_REDEEM, &req, sizeof(SERVER_GET_TOKEN_REDEEM_REQUEST), &resp, sizeof(SERVER_GET_TOKEN_REDEEM_RESPONSE)) != ERROR_SUCCESS)
	{
		DbgPrint("RedeemToken - SendCommand() Returned E_FAIL\n");
		return E_FAIL;
	}

	switch (resp.Status)
	{
	case XSTL_STATUS_SUCCESS:
	case XSTL_STATUS_STEALTHED:
		XNotifyUI(L"OutHereLive - Token Redeemed\n Enjoy Your Time On OutHereLive!!");
		break;
	default:
		XNotifyUI(L"OutHereLive - Server Failed To Redeem Token");
		break;
	}
	return ERROR_SUCCESS;
}

HRESULT DoTokenCheck(const char *Token, DWORD confirm) {

	SERVER_GET_TOKEN_CHECK_REQUEST req;
	SERVER_GET_TOKEN_CHECK_RESPONSE resp;

	memcpy(req.SessionKey, seshKey, 0x10);
	memcpy(req.Token, Token, 0x0E);
	DbgPrint("Token: %s\n", (char*)req.Token);

	if (SendCommand(XSTL_SERVER_COMMAND_ID_TOKEN_CHECK, &req, sizeof(SERVER_GET_TOKEN_CHECK_REQUEST), &resp, sizeof(SERVER_GET_TOKEN_CHECK_RESPONSE)) != ERROR_SUCCESS)
	{
		DbgPrint("DoTokenCheck - SendCommand() Returned E_FAIL\n");
		return E_FAIL;
	}

	std::string DaysStr;
	switch (resp.Days)
	{
	case TOKEN_TYPE_DAY:
		DaysStr = "1 Day";
		break;
	case TOKEN_TYPE_3DAYS:
		DaysStr = "3 Day";
		break;
	case TOKEN_TYPE_WEEK:
		DaysStr = "1 Week";
		break;
	case TOKEN_TYPE_2WEEKS:
		DaysStr = "2 week";
		break;
	case TOKEN_TYPE_MONTH:
		DaysStr = "1 Month";
		break;
	case TOKEN_TYPE_3MONTHS:
		DaysStr = "3 Month";
		break;
	case TOKEN_TYPE_6MONTHS:
		DaysStr = "6 Month";
		break;
	case TOKEN_TYPE_1YEAR:
		DaysStr = "1 Year";
		break;
	case TOKEN_TYPE_LIFETIME:
		DaysStr = "Lifetime";
		break;
	case TOKEN_TYPE_INVALID:
		XNotifyUI(L"OutHereLive - Your Token Seems TO Be Token Invalid ");
	default:
		DbgPrint("Invalid Token\n");
		return E_FAIL;
		break;
	}
	XOVERLAPPED ovl;
	MESSAGEBOX_RESULT Result;
	ZeroMemory(&ovl, sizeof(XOVERLAPPED));
	ZeroMemory(&Result, sizeof(MESSAGEBOX_RESULT));
	LPCWSTR Btns[2] = { L"Redeem",L"Cancel" };
	std::string str = "Token Status: Valid\nToken Time:" + DaysStr + "\n\nOnce you redeem your token.\n\nOutHereLive time will only be used\nwhen you use the console\nWould you like to redeem your token?";
	std::wstring wstr = utf8_decode(str);
	while (XShowMessageBoxUI(XUSER_INDEX_ANY, L"OutHereLive - Token Options", wstr.c_str(), 2, Btns, 1, XMB_ALERTICON, &Result, &ovl) == ERROR_ACCESS_DENIED) Sleep(500);

	while (!XHasOverlappedIoCompleted(&ovl)) Sleep(500);

	switch (Result.dwButtonPressed)
	{
	case 0:
		return RedeemToken(Token);
		break;
	case 1:
		break;
	}
	return ERROR_SUCCESS;

}


int tokenMaxLen = 14;
VOID RedeemTokenBeta()
{
	DoAuth();
	XOVERLAPPED active1;
	WCHAR buffer[15];//a6e82411d6a04c
	while (XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"Enter your 14 digit token here", L"Example = AAAA-BBBB-CCCC", buffer, 15, &active1) == ERROR_ACCESS_DENIED) Sleep(500);

	while (!XHasOverlappedIoCompleted(&active1)) Sleep(500);
	if (wcslen(buffer) == 14 /*|| wcslen(buffer) == 32*/)
	{
		Sleep(1000);
		std::string str = utf8_encode(buffer);
		if (DoTokenCheck(str.c_str(), 0) != ERROR_SUCCESS)
		{
		}
	}
	else if (wcslen(buffer) > 1)
	{
	}
}

//end token
//name
HRESULT nameChange(PCHAR Nam) {
	NAME_REQUEST req;
	NAME_RESPONSE resp;

	memcpy(req.CpuKey, CPUKey, 16);
	strncpy(req.name, Nam, 20);

	if (SendCommand(XSTL_SERVER_COMMANDS_ID_CHANGE_NAME, &req, sizeof(NAME_REQUEST), &resp, sizeof(NAME_RESPONSE)) != ERROR_SUCCESS) {
		return E_FAIL;
	}

	if (resp.response == 1) {
		sprintf(NameRespmsgBuffer, "Name Changed to: %s", Nam);
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}
	else if (resp.response == 0) {
		sprintf(NameRespmsgBuffer, "Invalid Name");
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}
	else if (resp.response == 2) {
		sprintf(NameRespmsgBuffer, "Name Has Already Been Used");
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}
	else if (resp.response == 3) {
		sprintf(NameRespmsgBuffer, "(Error) Could Not Change Name");
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}
	else if (resp.response == 4) {

		sprintf(NameRespmsgBuffer, "(Error) Could Not Add Username");
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}
	else {
		sprintf(NameRespmsgBuffer, "(Error) Invalid Server Response");
		mbstowcs(NameRespmsg, NameRespmsgBuffer, strlen(NameRespmsgBuffer) + 1);
		XNotifyUI(NameRespmsg);
	}return 0;
}


VOID ChangeName() {
	DoAuth();
	sprintf(HUDMsgBuffer, "Current Name Is: %s", cData.name);
	mbstowcs(HUDMsg, HUDMsgBuffer, strlen(HUDMsgBuffer) + 1);
	XNotifyUI(HUDMsg);
	Sleep(500);
	WCHAR nameInput[512];
	CHAR name[0x100];
	XOVERLAPPED Overlapped;
	ZeroMemory(&Overlapped, sizeof(Overlapped));

	while (XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"OutHereLive Name Change", L"Enter A Name!", nameInput, 20, &Overlapped) == ERROR_ACCESS_DENIED) Sleep(500);

	while (!XHasOverlappedIoCompleted(&Overlapped))
		Sleep(100);
	if (wcslen(nameInput) != 0) {
		wcstombs((PCHAR)name, nameInput, nameMaxLen);
		if (strlen((PCHAR)name) != 21) {
			Sleep(500);
		}
		nameChange((PCHAR)name);
	}

}

//endname
VOID Reboot() {
	HalReturnToFirmware(HalRebootRoutine);
}
VOID Shutdwn() {
	HalReturnToFirmware(HalPowerDownRoutine);
}

DWORD ScnTab5::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	GetChildById(L"btnName", &btnName);
	GetChildById(L"btnReboot", &btnReboot);
	GetChildById(L"btnShut", &btnShut);
	GetChildById(L"btnToken", &btnToken);
	GetChildById(L"btnKv", &btnKv);
	return ERROR_SUCCESS;
}

DWORD ScnTab5::OnInitt(HXUIOBJ hObjPressed, BOOL& bHandled)
{
	if (!Expired) {
		if (hObjPressed == btnName)
		{
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)ChangeName, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		 if (hObjPressed == btnReboot)
		{
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)Reboot, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		 if (hObjPressed == btnShut)
		{
			HANDLE hThread;
			DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)Shutdwn, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		 if (hObjPressed == btnKv)
		 {
			 if (FileExists(PATH_KEYVAULT_HDD)) {
				 DbgPrint("Kv Loaded From: %s", PATH_KEYVAULT_HDD);
				 if (SetKeyVault(PATH_KEYVAULT_HDD) != ERROR_SUCCESS) {
					 DbgPrint("%s Load Failed", PATH_KEYVAULT_HDD);
					 HalReturnToFirmware(HalPowerDownRoutine);
				 }
			 }
		 }
	}
	if (hObjPressed == btnToken)
	{
		HANDLE hThread;
		DWORD threadId;
		ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)RedeemTokenBeta, NULL, 0x2 | CREATE_SUSPENDED);
		XSetThreadProcessor(hThread, 4);
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
		return ERROR_SUCCESS;
}
DWORD ScnTab5::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}
DWORD ScnTab5::InitializeChildren() {
	return ERROR_SUCCESS;
}