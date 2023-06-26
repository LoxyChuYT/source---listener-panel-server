#pragma once

typedef VOID(*myWprintf)(PWCHAR _Dest, INT64 _Count, const PWCHAR _Format, ...);
typedef VOID(*myXeCryptRc4)(const PBYTE pbKey, DWORD cbKey, PBYTE pbInpOut, DWORD cbInpOut);
typedef VOID(*XAMBUILDRESOURCELOCATOR)(HANDLE hModule, PWCHAR wModuleName, PWCHAR const cdModule, HMODULE hdRes, ULONG ddSize);
VOID InitializeHudHooks(PLDR_DATA_TABLE_ENTRY ModuleHandle);
DWORD setcolornotify();