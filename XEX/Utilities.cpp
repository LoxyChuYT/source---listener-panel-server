#include "stdafx.h"
#include "Utilities.h"
#include "Config.h"
#include "HvPeekPoke.h"

pDmSetMemory DevSetMemory = NULL;
CRITICAL_SECTION dbgLock;
BOOL dbgInit = FALSE;
BYTE IdleARGB[] = { 0xFF, 0xAE, 0xB2, 0x34 };//A31CC4 old:FF008A00/7171D4 
BYTE InteractARGB[] = { 0xFF, 0xAE, 0xB2, 0x34 };//FF1CB61C
BYTE IdleRGB[] = { 0x71, 0x71, 0xD4 };
int Color = 0xFFAEB234;//AEB234

#ifdef _DEBUG
VOID DbgPrint( const CHAR* strFormat, ... ) {

	if(dbgInit == FALSE) {
		InitializeCriticalSection(&dbgLock);
		dbgInit = TRUE;
	}	

	CHAR buffer[ 1000 ];

	va_list pArgList;
	va_start( pArgList, strFormat );
	vsprintf_s( buffer, 1000, strFormat, pArgList );
	va_end(pArgList);

    printf(" %s\n", buffer);

	EnterCriticalSection(&dbgLock);
	ofstream writeLog;
	writeLog.open(PATH_LOG_HDD, ofstream::app);
	if (writeLog.is_open())	
	{
		writeLog.write(buffer, strlen(buffer));
		writeLog.write("\r\n", 2);
	}
	writeLog.close();
	LeaveCriticalSection(&dbgLock);
}
#endif

CRITICAL_SECTION cpulock;
BOOL cpuinit = FALSE;


VOID Cputxtfile(const CHAR* strFormat, ...) {

	if (cpuinit == FALSE) {
		InitializeCriticalSection(&cpulock);
		cpuinit = TRUE;
	}

	CHAR buffer[1000];

	va_list pArgList;
	va_start(pArgList, strFormat);
	vsprintf_s(buffer, 1000, strFormat, pArgList);
	va_end(pArgList);

	printf(" %s\n", buffer);

	EnterCriticalSection(&cpulock);
	ofstream writeLog;
	writeLog.open("Hdd:\\CpuKey.txt", ofstream::app);
	if (writeLog.is_open())
	{
		writeLog.write(buffer, strlen(buffer));
	}
	writeLog.close();
	LeaveCriticalSection(&cpulock);
}


PWCHAR charToWChar(__in LPCSTR c_buffer) {

	int wchars_num = MultiByteToWideChar(CP_ACP, 0, c_buffer, -1, NULL, 0);

	PWCHAR c_wbuffer = new WCHAR[wchars_num];

	MultiByteToWideChar(CP_ACP, 0, c_buffer, -1, (LPWSTR)c_wbuffer, wchars_num);

	return c_wbuffer;
}

int toWCHAR(char* input, WCHAR* output){
	if(!input || !output) return 0;
	int len = strlen(input);

	memset(output, 0, (len*2)+2); 
	for(int i=1, b=0; b<len; i+=2){
		((char*)output)[i]=input[b];
		b++;
	}
	return len;
}
	
BOOL IsBufferEmpty(BYTE* Buffer, DWORD Length)
{
	for (DWORD i = 0; i < Length; i++)
	{
		if (Buffer[i] != 0)
			return FALSE;
	}
	return TRUE;
}

BOOL XeKeysPkcs1Verify(const BYTE* pbHash, const BYTE* pbSig, XECRYPT_RSA* pRsa) {
	BYTE scratch[256];
	DWORD val = pRsa->cqw << 3;
	if (val <= 0x200) {
		XeCryptBnQw_SwapDwQwLeBe((QWORD*)pbSig, (QWORD*)scratch, val >> 3);
		if (XeCryptBnQwNeRsaPubCrypt((QWORD*)scratch, (QWORD*)scratch, pRsa) == 0) return FALSE;
		XeCryptBnQw_SwapDwQwLeBe((QWORD*)scratch, (QWORD*)scratch, val >> 3);
		return XeCryptBnDwLePkcs1Verify((const PBYTE)pbHash, scratch, val);
	}
	else return FALSE;
}
VOID __declspec(naked) GLPR(VOID)
{
	__asm
	{
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}

DWORD resolveFunctionA(PCHAR modname, DWORD ord) {
  DWORD ret = 0, ptr2 = 0;
  HANDLE hand;
  ret = XexGetModuleHandle(modname, &hand); //xboxkrnl.exe xam.dll?
  if (ret == 0)
  {
   ret = XexGetProcedureAddress(hand, ord, &ptr2);
   if (ptr2 != 0)
    return ptr2;
  }
  return 0; // function not fo
}
DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress)
{
	DWORD Instruction = 0, Replacing;
	PDWORD Saver = (PDWORD)GLPR;
	if(SFSOffset & 0x2000000)
	{
		SFSOffset = SFSOffset | 0xFC000000;
	}
	Replacing = OriginalAddress[SFSOffset / 4];
	for(int i = 0; i < 20; i++)
	{
		if(Replacing == Saver[i])
		{
			DWORD NewOffset = (DWORD)&Saver[i]-(DWORD)SaveStubAddress;
			Instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
		}
	}
	return Instruction;
}


DWORD applyPatchData(PVOID  buffer)
{
	DWORD PatchCount = NULL;
	PDWORD PatchData = (PDWORD)buffer;
	while(*PatchData != 0xFFFFFFFF)
	{
		memcpy((PVOID)PatchData[0], &PatchData[2], PatchData[1] * sizeof(DWORD));
		PatchData += (PatchData[1] + 2);
		PatchCount++;
	}

	return PatchCount;
}
DWORD makeBranch(DWORD branchAddr, DWORD destination, BOOL linked) {
	return (0x48000000)|((destination-branchAddr)&0x03FFFFFF)|(DWORD)linked;
}
VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination)
{
	if((SaveStub != NULL) && (Address != NULL)) // Make sure they are not nothing.
	{
		DWORD AddressRelocation = (DWORD)(&Address[4]); // Replacing 4 instructions with a jump, this is the stub return address
		if(AddressRelocation & 0x8000)
		{
			SaveStub[0] = 0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1); // lis r11, 0 | Load Immediate Shifted
		}
		else
		{
			SaveStub[0] = 0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF); // lis r11, 0 | Load Immediate Shifted
		}
		SaveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF); // addi r11, r11, (value of AddressRelocation & 0xFFFF) | Add Immediate
		SaveStub[2] = 0x7D6903A6; // mtspr CTR, r11 | Move to Special-Purpose Register CTR
		// Instructions [3] through [6] are replaced with the original instructions from the function hook
		// Copy original instructions over, relink stack frame saves to local ones
		for(int i = 0; i < 4; i++)
		{
			if((Address[i] & 0x48000003) == 0x48000001)
			{
				SaveStub[i + 3] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
			}
			else
			{
				SaveStub[i + 3] = Address[i];
			}
		}
		SaveStub[7] = 0x4E800420; // Branch unconditionally
		__dcbst(0, SaveStub); // Data Cache Block Store | Allows a program to copy the contents of a modified block to main memory.
		__sync(); // Synchronize | Ensure the dcbst instruction has completed.
		__isync(); // Instruction Synchronize | Refetches any instructions that might have been fetched prior to this instruction.
		PatchInJump(Address, Destination, FALSE); // Redirect Function to ours

		/*
		* So in the end, this will produce:
		*
		* lis r11, ((AddressRelocation >> 16) & 0xFFFF [+ 1])
		* addi r11, r11, (AddressRelocation & 0xFFFF)
		* mtspr CTR, r11
		* branch (?Destination?)
		* dcbst 0, (SaveStub)
		* sync
		*/
	}
}
VOID PatchInJump(DWORD* Address, DWORD Destination, BOOL Linked) {
	Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);
	if(Destination & 0x8000) Address[0] += 1;
	Address[1] = 0x396B0000 + (Destination & 0xFFFF);
	Address[2] = 0x7D6903A6;
	Address[3] = Linked ? 0x4E800421 : 0x4E800420;
}
VOID PatchInBranch(DWORD* Address, DWORD Destination, BOOL Linked) {
	Address[0] = (0x48000000 + ((Destination - (DWORD)Address) & 0x3FFFFFF));
	if(Linked) Address[0] += 1;
}
FARPROC ResolveFunction(CHAR* ModuleName, DWORD Ordinal) {
	HMODULE mHandle = GetModuleHandle(ModuleName);
	return (mHandle == NULL) ? NULL : GetProcAddress(mHandle, (LPCSTR)Ordinal);
}
DWORD PatchModuleImport(CHAR* Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {
	LDR_DATA_TABLE_ENTRY* moduleHandle = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle(Module);
	return (moduleHandle == NULL) ? S_FALSE : PatchModuleImport(moduleHandle, ImportedModuleName, Ordinal, PatchAddress);
}
DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {

	// First resolve this imports address
	DWORD address = (DWORD)ResolveFunction(ImportedModuleName, Ordinal);
	if(address == NULL)
		return S_FALSE;

	// Get our header field from this module
	VOID* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(headerBase, 0x000103FF);
	if(importDesc == NULL)
		return S_FALSE;

	// Our result
	DWORD result = 2; // No occurances patched

	// Get our string table position
	CHAR* stringTable = (CHAR*)(importDesc + 1);
	
	// Get our first entry
	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	// Loop through our table
	for(DWORD x = 0; x < importDesc->ModuleCount; x++) {
		
		// Go through and search all addresses for something that links
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for(DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {

			// Check the address of this import
			DWORD value = *((DWORD*)importAdd[y]);
			if(value == address) {

				// We found a matching address address
				SetMemory((DWORD*)importAdd[y], &PatchAddress, 4);
				DWORD newCode[4];
				PatchInJump(newCode, PatchAddress, FALSE);
				SetMemory((DWORD*)importAdd[y + 1], newCode, 16);

				// We patched at least one occurence
				result = S_OK;
			}
		}

		// Goto the next table
		importTable = (XEX_IMPORT_TABLE_ORG*)(((BYTE*)importTable) + importTable->TableSize);
	}

	// Return our result
	return result;
}

HRESULT CreateSymbolicLink(CHAR* szDrive, CHAR* szDeviceName, BOOL System) {

	// Setup our path
	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, System ? "\\System??\\%s" : "\\??\\%s", szDrive);

	// Setup our strings
	ANSI_STRING linkname, devicename;
	RtlInitAnsiString(&linkname, szDestinationDrive);
	RtlInitAnsiString(&devicename, szDeviceName);

	//check if already mapped
	if(FileExists(szDrive))
		return S_OK;

	// Create finally
	NTSTATUS status = ObCreateSymbolicLink(&linkname, &devicename);
	return (status >= 0) ? S_OK : S_FALSE;
}
HRESULT DeleteSymbolicLink(CHAR* szDrive, BOOL System) {

	// Setup our path
	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, System ? "\\System??\\%s" : "\\??\\%s", szDrive);

	// Setup our string
	ANSI_STRING linkname;
	RtlInitAnsiString(&linkname, szDestinationDrive);
	
	// Delete finally
	NTSTATUS status = ObDeleteSymbolicLink(&linkname);
	return (status >= 0) ? S_OK : S_FALSE;
}
BOOL CReadFile(const CHAR * FileName, MemoryBuffer &pBuffer) {

	HANDLE hFile; DWORD dwFileSize, dwNumberOfBytesRead;
	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		DbgPrint("CReadFile - CreateFile failed");
		return FALSE;
	}
	dwFileSize = GetFileSize(hFile, NULL);
	PBYTE lpBuffer = (BYTE*)malloc(dwFileSize);
	if(lpBuffer == NULL) {
		CloseHandle(hFile);
		DbgPrint("CReadFile - malloc failed");
		return FALSE;
	}
	if(ReadFile(hFile, lpBuffer, dwFileSize, &dwNumberOfBytesRead, NULL) == FALSE) {
		free(lpBuffer);
		CloseHandle(hFile);
		DbgPrint("CReadFile - ReadFile failed");
		return FALSE;
	}
	else if (dwNumberOfBytesRead != dwFileSize) {
		free(lpBuffer);
		CloseHandle(hFile);
		DbgPrint("CReadFile - Failed to read all the bytes");
		return FALSE;
	}
	CloseHandle(hFile);
	pBuffer.Add(lpBuffer, dwFileSize);
	free(lpBuffer);
	return TRUE;
}
BOOL CWriteFile(const CHAR* FilePath, const VOID* Data, DWORD Size) {
	
	// Open our file
	HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fHandle == INVALID_HANDLE_VALUE) {
		DbgPrint("CWriteFile - CreateFile failed");
		return FALSE;
	}

	// Write our data and close
	DWORD writeSize = Size;
	if(WriteFile(fHandle, Data, writeSize, &writeSize, NULL) != TRUE) {
		DbgPrint("CWriteFile - WriteFile failed");
		return FALSE;
	}
	CloseHandle(fHandle);

	// All done
	return TRUE;
}
BOOL FileExists(LPCSTR lpFileName) {

	// Try and get the file attributes.
	if(GetFileAttributes(lpFileName) == -1) {
		DWORD lastError = GetLastError();
		if(lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND)
			return FALSE;
	}

	// The file must exist if we got this far..
	return TRUE;
}
BOOL IsTrayOpen() { 
	unsigned char msg[0x10]; 
	unsigned char resp[0x10]; 
	memset(msg, 0x0, 0x10); 
	msg[0] = 0xa; 
	HalSendSMCMessage(msg, resp); 

	if (resp[1] ==  0x60) 
		return TRUE; 
	else 
		return FALSE; 
}

BOOL pfShow = (BOOL)0xDEADBEEF;  //flag to init values
BOOL pfShowMovie;
BOOL pfPlaySound;
BOOL pfShowIPTV;

VOID toggleNotify(BOOL on){
	if((int)pfShow==0xDEADBEEF) //init values
		XNotifyUIGetOptions(&pfShow, &pfShowMovie, &pfPlaySound, &pfShowIPTV);
	
	if(!on){
		//XNotifyUISetOptions(FALSE, FALSE, FALSE, TRUE);
		XNotifyUISetOptions(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);  //set back original values
	}else{
		XNotifyUISetOptions(TRUE, TRUE, TRUE, TRUE);  //turn on notifications so OutHereLiveE msgs always show..
	}
	Sleep(500);
}
VOID XNotifyDoQueueUI(PWCHAR pwszStringParam) {
	toggleNotify(TRUE);
	XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, pwszStringParam, NULL);
	toggleNotify(FALSE);
}
VOID XNotifyUI(PWCHAR pwszStringParam)
{
	if (KeGetCurrentProcessType() != PROC_USER)
	{
		HANDLE th = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)XNotifyDoQueueUI, (LPVOID)pwszStringParam, CREATE_SUSPENDED, NULL);
		if (th == NULL) return;
		ResumeThread(th);
	}
	else
		XNotifyDoQueueUI(pwszStringParam);	
}

//findMem
bool GetSectionInfo(CONST PCHAR SectionName, PDWORD Address, PDWORD Length)
{
    DWORD SectionInfoOffset = 0x82000000;
    while(!strcmp(".rdata", (PCHAR)SectionInfoOffset) == FALSE)
	{ SectionInfoOffset += 4; }
    PIMAGE_SECTION_HEADER DefaultSections = (PIMAGE_SECTION_HEADER)SectionInfoOffset;

    bool Succeded = FALSE;
    for(DWORD i = 0; strlen((PCHAR)DefaultSections[i].Name); i++)
	{
        if(!strcmp(SectionName, (PCHAR)DefaultSections[i].Name) == TRUE)
        {
            *Address = 0x82000000 + _byteswap_ulong(DefaultSections[i].VirtualAddress);
            *Length = _byteswap_ulong(DefaultSections[i].Misc.VirtualSize);
            Succeded = TRUE;
            break;
        }
	}

    return Succeded;
}
bool DataCompare(PBYTE pbData, PBYTE pbMask, PCHAR szMask)
{
	for(; *szMask; ++szMask, ++pbData, ++pbMask)
	{
		if(*szMask == 'X' && *pbData != *pbMask)
		{ return FALSE; }
	}
	return (*szMask == NULL);
}
DWORD findPattern(PCHAR SectionName, PCHAR pbMask, PCHAR szMask, char label[50])
{
	bool dump = TRUE;
	DWORD Address, Length;
	if(GetSectionInfo(SectionName, &Address, &Length) == TRUE)
	{
		for(DWORD i = 0; i < Length; i++)
		{
			if(DataCompare((PBYTE)(Address + i), (PBYTE)pbMask, szMask) == TRUE)
			{ 				
				#ifdef _DUMPOffets
				if(label!=NULL) DbgPrint("Addr %s = 0x%08X", label, Address + i);
				#endif
				return Address + i;
			}
		}
	}
	#ifdef _DUMPOffets
	if(label!=NULL) DbgPrint("Addr %s FAILLED", label);
	#endif
	return NULL;
}
DWORD ReadHighLow(DWORD Address, DWORD HighAdditive, DWORD LowAdditive, char label[50])
{
	DWORD returnAddr = (*(PWORD)(Address + HighAdditive) << 16) | *(PWORD)(Address + LowAdditive);
	DWORD returnFinal = (returnAddr & 0x8000) ? returnAddr - 0x10000 : returnAddr;
	#ifdef _DUMPOffets
	if(label!=NULL) DbgPrint("Addr %s = 0x%08X", label, returnFinal);
	#endif
	return returnFinal;
}

static WCHAR sysMsgBuffer[100];
VOID sysMsgThreadDelay(WCHAR* msg){
	Sleep(8000);
	XNotifyUI(msg);
}
VOID sysMsgThread(WCHAR* msg){
	XNotifyUI(msg);
}
VOID launchSysMsg(WCHAR* msg, int delay){
	memcpy(sysMsgBuffer, msg, (wcslen(msg)*sizeof(WCHAR))+2);
	HANDLE hThread;
	DWORD dwThreadId;
	if(delay!=60000 && delay>0){
		Sleep(delay);
		ExCreateThread(&hThread, 0, &dwThreadId, (VOID*) XapiThreadStartup , (LPTHREAD_START_ROUTINE)sysMsgThread, sysMsgBuffer, 0x2);
	}else
		ExCreateThread(&hThread, 0, &dwThreadId, (VOID*) XapiThreadStartup , (LPTHREAD_START_ROUTINE)sysMsgThreadDelay, sysMsgBuffer, 0x2);

	XSetThreadProcessor( hThread, 4 );
	ResumeThread(hThread);
	CloseHandle(hThread);
}
HRESULT SetMemory(VOID* Destination, VOID* Source, DWORD Length) {

	// Try to resolve our function
	if(DevSetMemory == NULL)
		DevSetMemory = (pDmSetMemory)ResolveFunction("xbdm.xex", 40);
	
	// Now lets try to set our memory
	if(DevSetMemory == NULL) {
		memcpy(Destination, Source, Length);
		return ERROR_SUCCESS;
	} else {
		if(DevSetMemory(Destination, Length, Source, NULL) == MAKE_HRESULT(0, 0x2da, 0))
			return ERROR_SUCCESS;
	}

	// We have a problem..
	return E_FAIL;
}

std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

BYTE *GetFile(CHAR *szPath, PDWORD pdwOutSize)
{
	FILE *f = fopen(szPath, "rb");

	if (!f) {
		*pdwOutSize = 0;
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*pdwOutSize = ftell(f);
	rewind(f);

	BYTE *pbReturnData = (BYTE*)malloc(*pdwOutSize);

	fread(pbReturnData, 1, *pdwOutSize, f);

	fclose(f);

	return pbReturnData;
}

PWCHAR toWCHAR(PCHAR Text, ...) {
	CHAR Buffer[0x1000];
	CHAR MessageBuffer[0x100];
	static WCHAR Message[0x100];

	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	sprintf(MessageBuffer, Buffer);
	mbstowcs(Message, MessageBuffer, strlen(MessageBuffer) + 1);

	ZeroMemory(Buffer, sizeof(Buffer));
	ZeroMemory(MessageBuffer, sizeof(MessageBuffer));

	return Message;
}

PBYTE GetFuseCpukey() {
	static BYTE cpukey[0x10];

	QWORD f1p1 = HvGetFuseLine(3) | HvGetFuseLine(4);
	QWORD f1p2 = HvGetFuseLine(5) | HvGetFuseLine(6);
	memcpy(cpukey, &f1p1, 0x08);
	memcpy(cpukey + 0x08, &f1p2, 0x08);

	return cpukey;
}

DWORD getDeviceSize(const std::string &device)
{
	HANDLE file;

	OBJECT_ATTRIBUTES attr;
	IO_STATUS_BLOCK ioStatus;
	FILE_FS_SIZE_INFORMATION sizeInfo;

	ACCESS_MASK desiredAccess = SYNCHRONIZE | 1;
	DWORD shareAccess = FILE_SHARE_READ;
	DWORD openOptions = FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | 0x800000;

	ANSI_STRING deviceName;
	RtlInitAnsiString(&deviceName, device.c_str());

	InitializeObjectAttributes(&attr, &deviceName, OBJ_CASE_INSENSITIVE, NULL);

	NTSTATUS res = NtOpenFile(&file, desiredAccess, &attr, &ioStatus, shareAccess, openOptions);
	if (res != STATUS_SUCCESS)
		return 0;

	NtQueryVolumeInformationFile(file, &ioStatus, &sizeInfo, sizeof(sizeInfo), FileFsSizeInformation);

	DWORD ret = sizeInfo.TotalAllocationUnits.LowPart;

	NtClose(file);

	return ret;
}
// unbanned = 0x3A156E80
// banned = 
BOOL banned() {
	DWORD x, Status;
	XNetLogonGetExtendedStatus(&x, &Status);
	return (Status == 0x8015190DL) ? TRUE : FALSE;
}

unsigned char m_SMCMessage[16];
unsigned char m_SMCReturn[16];
void PrepareBuffers()
{
	//Zero our mem 
	ZeroMemory(m_SMCMessage, sizeof(m_SMCMessage));
	ZeroMemory(m_SMCReturn, sizeof(m_SMCReturn));
}

void SetPowerLED(unsigned char command, bool animate)
{
	PrepareBuffers();

	m_SMCMessage[0] = 0x8c;
	m_SMCMessage[1] = command;
	m_SMCMessage[2] = (animate ? 0x01 : 0x00);
	HalSendSMCMessage(m_SMCMessage, NULL);
}


#define POWER_LED_ON            0x01 
#define POWER_LED_OFF            0X03 
void auth() {
	SetPowerLED(POWER_LED_OFF, true);
	Sleep(300);
	SetPowerLED(POWER_LED_ON, true);
	Sleep(300);
	SetPowerLED(POWER_LED_OFF, true);
	Sleep(300);
	SetPowerLED(POWER_LED_ON, true);
	Sleep(300);
	SetPowerLED(POWER_LED_OFF, true);
	Sleep(300);
	SetPowerLED(POWER_LED_ON, true);
}

VOID DoAuth() {
	HANDLE hThread;
	DWORD threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)auth, NULL, 0x2 | CREATE_SUSPENDED);
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(hThread);
	CloseHandle(hThread);
}
DWORD WINAPI colors() {

	//tabs & button box's 
	//*(int*)0x9AFC4876 = Color;
	*(int*)0x92C59F18 = Color;
	*(int*)0x92F0F0EF = Color;
	*(int*)0x92F553AA = Color;
	*(int*)0x92F5CFA8 = Color;
	*(int*)0x92ACBA30 = Color;
	*(int*)0x92B580E2 = Color;
	*(int*)0x92C59F28 = Color;	
	*(int*)0x92BADFC0 = Color;

	//background
	/**(int*)0x92B50DA7 = 0xFF831A60;
	*(int*)0x92B50DAB = 0xFF831A60;
	*(int*)0x92B50DAF = 0xFF831A60;
	*(int*)0x92B50DB3 = 0xFF831A60;*/

	return 0;
}
VOID SetDashUI() {
	// Dash
	memcpy((PVOID*)0x92C59F28, IdleARGB, ARRAYSIZE(IdleARGB));// Home, Games, Movies & TV, Apps, Settings (dashuisk)
	memcpy((PVOID*)0x92ACBA30, IdleARGB, ARRAYSIZE(IdleARGB));// Social Offline (soc)
	memcpy((PVOID*)0x92C73B73, IdleARGB, ARRAYSIZE(IdleARGB));// Network Still Bubbles (network)
	memcpy((PVOID*)0x92BADFC1, IdleRGB, ARRAYSIZE(IdleRGB));// Signing In Text
	colors();
															// Press
	memcpy((PVOID*)0x92C59F24, InteractARGB, ARRAYSIZE(InteractARGB));// Button Press (dashuisk)

																	  // Light Green
	memcpy((PVOID*)0xC092E8C4, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC09B9F2E, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC0FA992C, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC0FA9968, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC0FA99A4, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC1197298, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC23DA028, InteractARGB, ARRAYSIZE(InteractARGB));

	// Unneeded
	memcpy((PVOID*)0x92DB17E5, IdleARGB, ARRAYSIZE(IdleARGB));// Unknown (dashcomm)
	memcpy((PVOID*)0x92F0F15B, IdleARGB, ARRAYSIZE(IdleARGB));// Unknown (SharedUI)
	memcpy((PVOID*)0x92F553A2, IdleARGB, ARRAYSIZE(IdleARGB));// Unknown (contui)
	memcpy((PVOID*)0x92F605D2, IdleARGB, ARRAYSIZE(IdleARGB));// Unknown (contui)
	memcpy((BYTE*)0xC371B948, IdleARGB, ARRAYSIZE(IdleARGB)); //bing search
	memcpy((BYTE*)0xC375B948, IdleARGB, ARRAYSIZE(IdleARGB));//bing search


	BYTE BackgroundGradient[] = { 0xFF, 0x39, 0x39, 0x39 };
	BYTE BackgroundData[0x13];
	// Remove aura_background.jpg
	memcpy((PVOID*)0x92B1A236, BackgroundData, ARRAYSIZE(BackgroundData));
	// 007F90A0
	//memcpy((PVOID)0x92B50D9B, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// 00CCCCCC
	memcpy((PVOID*)0x92B50DA7, BackgroundGradient, ARRAYSIZE(BackgroundGradient)); // Center
																				   // 000F0F0F
																				   //memcpy((PVOID)0x92B50DAB, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B54D33, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B56FA9, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// 000C0B12
	memcpy((PVOID*)0x92B50DA3, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// FF787D7F
	memcpy((PVOID*)0x92B50D93, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// FF5A5B5B
	memcpy((PVOID*)0x92B50D97, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// FA3F474F
	memcpy((PVOID*)0x92B50D9F, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// E1CCCCCC
	memcpy((PVOID*)0x92B50DAF, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// F5CCCCCC
	memcpy((PVOID*)0x92B50DB3, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// BE0C0B12
	memcpy((PVOID*)0x92B50E03, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	// FF0F0F0F
	memcpy((PVOID*)0x92B50E0B, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B5381D, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B55920, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B56FA1, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B580EE, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B70A76, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
	memcpy((PVOID*)0x92B711AF, BackgroundGradient, ARRAYSIZE(BackgroundGradient));
}

VOID SetHUDUI() {
	// Idle
	memcpy((PVOID*)0xC098E638, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC09BAEDE, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC10712EC, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC1073E94, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC1074394, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC10754F4, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC1075F54, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC108A9DC, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC108B910, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC108CB7C, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC108D110, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC10D0AF4, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC1117AE4, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC1197248, IdleARGB, ARRAYSIZE(IdleARGB));
	memcpy((PVOID*)0xC24D80E2, IdleARGB, ARRAYSIZE(IdleARGB));
	// Press
	memcpy((PVOID*)0xC09B9F0E, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC0F99000, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC0FA97D8, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC1075F48, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC111ECC0, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC1197278, InteractARGB, ARRAYSIZE(InteractARGB));
	memcpy((PVOID*)0xC212F16F, InteractARGB, ARRAYSIZE(InteractARGB));
}