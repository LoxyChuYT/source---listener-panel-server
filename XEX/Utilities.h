
#define __UTILITIES_H_

#pragma once
#include "stdafx.h"
#include "Config.h"
#include "stdafx.h"
#include <xtl.h>
#include <stdio.h>
#include <xkelib.h>

PWCHAR toWCHAR(PCHAR Text, ...);
typedef HRESULT (*pDmSetMemory)(LPVOID lpbAddr, DWORD cb, LPCVOID lpbBuf, LPDWORD pcbRet);

class MemoryBuffer
{
public:

	MemoryBuffer( DWORD dwSize = 512 )
	{
		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;

		if( ( dwSize < UINT_MAX ) && ( dwSize != 0 ) )
		{
			m_pBuffer = ( BYTE* )malloc( dwSize + 1 );    // one more char, in case when using string funcions
			if( m_pBuffer )
			{
				m_dwBufferSize = dwSize;
				m_pBuffer[0] = 0;
			}
		}
	};

	~MemoryBuffer()
	{
		if( m_pBuffer )
			free( m_pBuffer );

		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;
	};

    // Add chunk of memory to buffer
    BOOL    Add( const void* p, DWORD dwSize )
    {
        if( CheckSize( dwSize ) )
        {
            memcpy( m_pBuffer + m_dwDataLength, p, dwSize );
            m_dwDataLength += dwSize;
            *( m_pBuffer + m_dwDataLength ) = 0;    // fill end zero
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    };

    // Get the data in buffer
    BYTE* GetData() const
    {
        return m_pBuffer;
    };

    // Get the length of data in buffer
    DWORD   GetDataLength() const
    {
        return m_dwDataLength;
    };

    // Rewind the data pointer to the begining
    void    Rewind()
    {
        m_dwDataLength = 0; m_pBuffer[ 0 ] = 0;
    };

    // Automatically adjust increase buffer size if necessary
    BOOL    CheckSize( DWORD dwSize )
    {
        if( m_dwBufferSize >= ( m_dwDataLength + dwSize ) )
        {
            return TRUE;    // Enough space
        }
        else
        {
            // Try to double it
            DWORD dwNewSize = max( m_dwDataLength + dwSize, m_dwBufferSize * 2 );
            BYTE* pNewBuffer = ( UCHAR* )realloc( m_pBuffer, dwNewSize + 1 );        // one more char
            if( pNewBuffer )
            {
                m_pBuffer = pNewBuffer;
                m_dwBufferSize = dwNewSize;
                return TRUE;
            }
            else
            {
                // Failed
                return FALSE;
            }
        }
    }

	private:

	BYTE* m_pBuffer;

    DWORD m_dwDataLength;

    DWORD m_dwBufferSize;
	
};

VOID Cputxtfile(const CHAR* strFormat, ...);
#ifdef _DEBUG
VOID DbgPrint(const CHAR* strFormat, ...);
#else
#define DbgPrint
#endif
VOID XNotifyUI(PWCHAR pwszStringParam);
VOID XNotifyDoQueueUI(PWCHAR pwszStringParam);

//findMem
bool GetSectionInfo(CONST PCHAR SectionName, PDWORD Address, PDWORD Length);
bool DataCompare(PBYTE pbData, PBYTE pbMask, PCHAR szMask);
DWORD findPattern(PCHAR SectionName, PCHAR pbMask, PCHAR szMask, char label[50] = NULL);
DWORD ReadHighLow(DWORD Address, DWORD HighAdditive, DWORD LowAdditive, char label[50] = NULL);

DWORD getDeviceSize(const std::string &device);
HRESULT SetMemory(VOID* Destination, VOID* Source, DWORD Length);
PBYTE GetFuseCpukey();
HRESULT CreateSymbolicLink(CHAR* szDrive, CHAR* szDeviceName, BOOL System);
HRESULT DeleteSymbolicLink(CHAR* szDrive, BOOL System);
PWCHAR charToWChar(__in LPCSTR c_buffer);
DWORD resolveFunctionA(PCHAR modname, DWORD ord);
BOOL XeKeysPkcs1Verify(const BYTE* pbHash, const BYTE* pbSig, XECRYPT_RSA* pRsa);
DWORD makeBranch(DWORD branchAddr, DWORD destination, BOOL linked);
DWORD applyPatchData(PVOID buffer);
BYTE *GetFile(CHAR *szPath, PDWORD pdwOutSize);
VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination);
VOID PatchInJump(DWORD* Address, DWORD Destination, BOOL Linked = FALSE);
VOID PatchInBranch(DWORD* Address, DWORD Destination, BOOL Linked);
FARPROC ResolveFunction(CHAR* ModuleName, DWORD Ordinal);
DWORD PatchModuleImport(CHAR* Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
BOOL IsBufferEmpty(BYTE* Buffer, DWORD Length);
QWORD HvGetFuseLine(BYTE fuseIndex);
BOOL CReadFile(const CHAR * FileName, MemoryBuffer &pBuffer);
BOOL CWriteFile(const CHAR* FilePath, const VOID* Data, DWORD Size);
BOOL FileExists(LPCSTR lpFileName);
int toWCHAR(char* input, WCHAR* output);
BOOL IsTrayOpen();
std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
BOOL banned();
VOID DoAuth();
void PrepareBuffers();
VOID SetDashUI();
VOID SetHUDUI();