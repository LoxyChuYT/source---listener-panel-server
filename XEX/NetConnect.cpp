#include "stdafx.h" 
#include "NetConnect.h"
#include "Utilities.h"

BOOL commandPending = FALSE;
#define XSTL_SALT_KEY_XOR_VALUE 0x7A
BYTE XSTL_SALT_KEY[] = { 0x22, 0x38, 0x36, 0x29, 0x2E, 0x3F, 0x3B, 0x36, 0x2E, 0x32};
BOOL  connected = FALSE;
DWORD lastSocketError;
CRITICAL_SECTION serverLock;
SOCKET hSocket = INVALID_SOCKET;
BYTE sessionKey[0x10];
BYTE tmpBuffer[0x4020];
#define SEND_RECV_SIZE 1024*2
int NetErrors = 0;
// Server connection info
BOOL netcheck = TRUE;
BOOL netnotify = TRUE;
BYTE FUCK_OFF_SKIDS[4] = { 0, 0, 0, 0 }; //vps encrypt your shit! Or someone will crack your stealth!
//#define FUCK_OFF_SKIDS_XOR_VALUE 0x83 
//#define XSTL_SERV_PORT_XOR_VALUE 0x74 
WORD XSTL_SERV_PORT = 5000;//dev
//WORD XSTL_SERV_PORT = 0x1EE4;//release


//Use code below if you dont want to XOR
VOID NetConnectToServer()
{
for (DWORD x = 0; x < sizeof(XSTL_SALT_KEY); x++)
XSTL_SALT_KEY[x] ^= XSTL_SALT_KEY_XOR_VALUE;
XeCryptSha(XSTL_SALT_KEY, sizeof(XSTL_SALT_KEY), NULL, NULL, NULL, NULL, sessionKey, 0x10);
InitializeCriticalSection(&serverLock);

}

//VOID NetConnectToServer() 
/*{
	// XOR the IP.
	for (DWORD i = 0; i < sizeof(FUCK_OFF_SKIDS); i++)
		FUCK_OFF_SKIDS[i] ^= FUCK_OFF_SKIDS_XOR_VALUE;
	// XOR the port.
	XSTL_SERV_PORT ^= XSTL_SERV_PORT_XOR_VALUE;
	// XOR Salt
	for(DWORD x = 0; x < sizeof(XSTL_SALT_KEY); x++)
		XSTL_SALT_KEY[x] ^= XSTL_SALT_KEY_XOR_VALUE;
	XeCryptSha(XSTL_SALT_KEY, sizeof(XSTL_SALT_KEY), NULL, NULL, NULL, NULL, sessionKey, 0x10);
	InitializeCriticalSection(&serverLock);
}*/

VOID EndCommand() {
	if (hSocket != INVALID_SOCKET && connected) {
		NetDll_closesocket(XNCALLER_SYSAPP, hSocket);
		connected = FALSE;
	}
}

HRESULT InitCommand() {
Start:
	XNetStartupParams xnsp;
	memset(&xnsp, 0, sizeof(xnsp));
	xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
	xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
	if((lastSocketError = NetDll_XNetStartup(XNCALLER_SYSAPP, &xnsp)) != 0) {
		DbgPrint("NetConnect: XNetStartup error 0x%08X\n", lastSocketError);
		Sleep(10000);
		goto Start;
	}
	DWORD dwStatus = XNetGetEthernetLinkStatus();
	int m_bIsActive = dwStatus != 0;

	if (!m_bIsActive)
	{
		if (netcheck) {
			DbgPrint("NetConnect: No Ethernet Link Active retrying\n");
			Sleep(10000);
			netcheck = FALSE;
			goto Start;
		}
		else {
			Sleep(10000);
			DbgPrint("NetConnect: No Ethernet Link Active\n");
			if (netnotify) {
				XNotifyUI(L"OutHereLive Auth Failed!\nPlease Connect To Inet!");
				netnotify = FALSE;
			}
			goto Start;
		}
	}
	WSADATA WsaData;
	if((lastSocketError = NetDll_WSAStartupEx(XNCALLER_SYSAPP, MAKEWORD(2, 2), &WsaData, 2)) != 0) {
		DbgPrint("NetConnect: WSAStartup error 0x%08X\n", lastSocketError);
		Sleep(10000);
		goto Start;
	}
	if((hSocket = NetDll_socket(XNCALLER_SYSAPP, AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		lastSocketError = WSAGetLastError();
		DbgPrint("NetConnect: socket error %d\n", lastSocketError);
		Sleep(10000);
		goto Start;
	}

	BOOL bSockOpt = TRUE;
	if(NetDll_setsockopt(XNCALLER_SYSAPP, hSocket, SOL_SOCKET, 0x5801, (PCSTR)&bSockOpt, sizeof(BOOL)) != 0) {
		lastSocketError = WSAGetLastError();
		DbgPrint("NetConnect: setsockopt error %d\n", lastSocketError);
		Sleep(10000);
		goto Start;
	}
	int sendRecvSize = SEND_RECV_SIZE;
	NetDll_setsockopt(XNCALLER_SYSAPP, hSocket, SOL_SOCKET, SO_SNDBUF, (PCSTR)&sendRecvSize, sizeof(int));
	NetDll_setsockopt(XNCALLER_SYSAPP, hSocket, SOL_SOCKET, SO_RCVBUF, (PCSTR)&sendRecvSize, sizeof(int));
	sockaddr_in httpServerAdd;
	httpServerAdd.sin_family = AF_INET;
	httpServerAdd.sin_port = htons(XSTL_SERV_PORT);
	httpServerAdd.sin_addr.S_un.S_un_b.s_b1 = FUCK_OFF_SKIDS[0];
	httpServerAdd.sin_addr.S_un.S_un_b.s_b2 = FUCK_OFF_SKIDS[1];
	httpServerAdd.sin_addr.S_un.S_un_b.s_b3 = FUCK_OFF_SKIDS[2];
	httpServerAdd.sin_addr.S_un.S_un_b.s_b4 = FUCK_OFF_SKIDS[3];
	if(NetDll_connect(XNCALLER_SYSAPP, hSocket, (struct sockaddr*)&httpServerAdd, sizeof(httpServerAdd)) == SOCKET_ERROR) {
		lastSocketError = WSAGetLastError();
		switch (NetErrors++)
		{
		case 0:
			DbgPrint("NetConnect: connect error 1: %d\n", lastSocketError);
			goto Start;
			break;
		case 1:
			DbgPrint("NetConnect: connect error 2: %d\n", lastSocketError);
			goto Start;
			break;
		case 2:
			DbgPrint("NetConnect: connect error 3: %d\n", lastSocketError);
			goto Start;
			break;
		case 3:
			DbgPrint("NetConnect: connect error 4: %d\n", lastSocketError);
			if (netnotify) {
				XNotifyUI(L"OutHereLive Auth Failed!\nCould Not Connect To Server!");

				netnotify = FALSE;
			}
			NetErrors = 3;
			goto Start;
			break;
			//Sleep(5000);
			//HalReturnToFirmware(HalFatalErrorRebootRoutine);
		}
	}
	netcheck = TRUE;
	netnotify = TRUE;
	connected = TRUE;
	NetErrors = 0;
	return ERROR_SUCCESS;
}
HRESULT SendCommand(DWORD CommandId, VOID* CommandData, DWORD DataLen) {
Start:
	if(!connected){
		if(InitCommand() != ERROR_SUCCESS) {
			goto Start;
		}
	}
	memcpy(tmpBuffer, &CommandId, sizeof(DWORD));
	memcpy(tmpBuffer + 4, &DataLen, sizeof(DWORD));
	XeCryptRc4(sessionKey, 0x10, (BYTE*)CommandData, DataLen);
	memcpy(tmpBuffer + 8, CommandData, DataLen);
	DWORD bytesLeft = DataLen + 8;
	CHAR* curPos = (CHAR*)tmpBuffer;
	while(bytesLeft > 0) {
		DWORD sendSize = min(SEND_RECV_SIZE, bytesLeft);
		DWORD cbSent = NetDll_send(XNCALLER_SYSAPP, hSocket, curPos, sendSize, NULL);
		if(cbSent == SOCKET_ERROR) {
			lastSocketError = WSAGetLastError();
			DbgPrint("NetConnect: send error %d\n", lastSocketError);
			return E_FAIL;
		}
		bytesLeft -= cbSent;
		curPos += cbSent;
	}
	return ERROR_SUCCESS;
}

HRESULT ReceiveData(VOID* Buffer, DWORD BytesExpected) {
	if(!connected) return E_FAIL;
	DWORD bytesLeft = BytesExpected;
	DWORD bytesRecieved = 0;
	while(bytesLeft > 0) {
		DWORD recvSize = min(SEND_RECV_SIZE, bytesLeft);
		DWORD cbRecv = NetDll_recv(XNCALLER_SYSAPP, hSocket, (CHAR*)Buffer + bytesRecieved, recvSize, NULL);
		if(cbRecv == SOCKET_ERROR) {
			lastSocketError = WSAGetLastError();
			DbgPrint("NetConnect: recv error %d\n", lastSocketError);
			return E_FAIL;
		}
		if(cbRecv == 0) { DbgPrint("NetConnect: recv cbRecv = 0"); break; }
		bytesLeft -= cbRecv;
		bytesRecieved += cbRecv;
	}
	if(bytesRecieved != BytesExpected) return E_FAIL;
	XeCryptRc4(sessionKey, 0x10, (BYTE*)Buffer, bytesRecieved);
	return ERROR_SUCCESS;
}

HRESULT SendCommand(DWORD CommandId, VOID* CommandData, DWORD CommandLength, VOID* Responce, DWORD ResponceLength, BOOL KeepOpen) {
	commandPending = TRUE;
	EnterCriticalSection(&serverLock);
	HRESULT returnValue = ERROR_SUCCESS;
	if(InitCommand() != ERROR_SUCCESS) {
		returnValue = E_FAIL;
		goto Finish;
	}
	if(SendCommand(CommandId, CommandData, CommandLength) != ERROR_SUCCESS) {
		returnValue = E_FAIL;
		goto Finish;
	}
	if(ReceiveData(Responce, ResponceLength) != ERROR_SUCCESS) {
		returnValue = E_FAIL;
		goto Finish;
	}
	if(KeepOpen == FALSE) {
		EndCommand();
	}
Finish:
	LeaveCriticalSection(&serverLock);
	commandPending = FALSE;
	return returnValue;
}