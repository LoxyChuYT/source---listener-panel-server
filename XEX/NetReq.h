#pragma once
#include "stdafx.h"
#include "NetConnect.h"
#include "KeyVault.h"
#include "Config.h"

extern BYTE seshKey[16];
extern SERVER_GET_CUSTOM_RESPONCE cData;
extern SERVER_GET_Modules mData;
extern BOOL Expired;
extern wchar_t tokenInput[512];


	namespace svr {
	HRESULT getUpdate();
	HRESULT getSalt();
	HRESULT GET_Modules();
	VOID getVars();
	VOID presenceThread();
	HRESULT startPresence();
	HRESULT startVars();
	HRESULT ini();
}
