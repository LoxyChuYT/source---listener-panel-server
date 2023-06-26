#include "stdafx.h"
#include "ScnPreferences.h"
#include "INIReader.h"
#include "NetReq.h"
#include "HvPeekPoke.h"
#include "Utilities.h"

extern BYTE kvDigest[XECRYPT_SHA_DIGEST_SIZE];
extern BOOL freemode;
extern BYTE m_SMCMessage[0x10];
extern BYTE m_SMCReturn[0x10];
#define REQUEST_TEMP            0x07

typedef enum {
	CPU,
	GPU,
	MEM,
	BRD
} TEMP_INDEX;

FLOAT GetTemperature(TEMP_INDEX sTempIndex, BOOL Celsius) {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_TEMP;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);

	FLOAT Temp = (FLOAT)((m_SMCReturn[sTempIndex * 0x02 + 0x01] | (m_SMCReturn[sTempIndex * 0x02 + 0x02] << 0x08)) / 256.0);

	Temp = (9.0f / 5.0f) * Temp + 32.0f;

	return Temp;
}

HRESULT ScnPreferences::OnInit(XUIMessageInit *pInitData, BOOL& bHandled)
{
	return ERROR_SUCCESS;
}

HRESULT ScnPreferences::OnRender(XUIMessageRender *pRenderData, BOOL& bHandled)
{
	InitializeChildren();

	//Plugins
	INIReader reader("HDD:\\launch.ini");
	if (reader.ParseError() < 0) DbgPrint("Failed to read launch.ini");
	txt_LabelPlugins.SetText(toWCHAR("%s's Plugins", cData.name));

	///////////////////////////////////////////////////////////////////////
	//One

	std::string Plugin1 = reader.Get("Plugins", "plugin1", "UNKNOWN");
	wstring widePlugin1;
	for (size_t i = 0; i < Plugin1.length(); ++i)
		widePlugin1 += wchar_t(Plugin1[i]);

	const wchar_t* Plugin1Conv = widePlugin1.c_str();
	txt_TextPlugin1.SetText(Plugin1Conv);

	///////////////////////////////////////////////////////////////////////
	//Two

	std::string Plugin2 = reader.Get("Plugins", "plugin2", "UNKNOWN");
	wstring widePlugin2;
	for (size_t i = 0; i < Plugin2.length(); ++i)
		widePlugin2 += wchar_t(Plugin2[i]);

	const wchar_t* Plugin2Conv = widePlugin2.c_str();
	txt_TextPlugin2.SetText(Plugin2Conv);

	///////////////////////////////////////////////////////////////////////
	//Three

	std::string Plugin3 = reader.Get("Plugins", "plugin3", "UNKNOWN");
	wstring widePlugin3;
	for (size_t i = 0; i < Plugin3.length(); ++i)
		widePlugin3 += wchar_t(Plugin3[i]);

	const wchar_t* Plugin3Conv = widePlugin3.c_str();
	txt_TextPlugin3.SetText(Plugin3Conv);

	///////////////////////////////////////////////////////////////////////
	//Four

	std::string Plugin4 = reader.Get("Plugins", "plugin4", "UNKNOWN");
	wstring widePlugin4;
	for (size_t i = 0; i < Plugin4.length(); ++i)
		widePlugin4 += wchar_t(Plugin4[i]);

	const wchar_t* Plugin4Conv = widePlugin4.c_str();
	txt_TextPlugin4.SetText(Plugin4Conv);

	///////////////////////////////////////////////////////////////////////
	//Fice

	std::string Plugin5 = reader.Get("Plugins", "plugin5", "UNKNOWN");
	wstring widePlugin5;
	for (size_t i = 0; i < Plugin5.length(); ++i)
		widePlugin5 += wchar_t(Plugin5[i]);

	const wchar_t* Plugin5Conv = widePlugin5.c_str();
	txt_TextPlugin5.SetText(Plugin5Conv);




	// -- Time Left
	wchar_t Text[112];
	if (freemode) {
		swprintf(Text, L"Free Mode");
	}
	else if (Expired) {
		swprintf(Text, L"Expired");
	}
	else if (cData.days >= 500) {

		swprintf(Text, L"Lifetime");
	}

	else {
		swprintf(Text, L"%iDays %iHrs %iMin", cData.days, cData.hours, cData.minutes);
	}
	Data3.SetText(Text);

	// -- KV Hash
	wchar_t KVHash11[112];
	swprintf(KVHash11, L"%02X%02X%02X%02X", kvDigest[0], kvDigest[1], kvDigest[2], kvDigest[3]);
	Data1.SetText(KVHash11);


	// -- CPU Key
	wchar_t CPUKey[112];
	BYTE ConsoleKey[0x10];
	HvPeekBytes(0x20, ConsoleKey, 0x10);
	swprintf(CPUKey, L" %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", ConsoleKey[0], ConsoleKey[1], ConsoleKey[2], ConsoleKey[3], ConsoleKey[4], ConsoleKey[5], ConsoleKey[6], ConsoleKey[7], ConsoleKey[8],
		ConsoleKey[9], ConsoleKey[10], ConsoleKey[11], ConsoleKey[12], ConsoleKey[13], ConsoleKey[14], ConsoleKey[15], ConsoleKey[16]);
	Data2.SetText(CPUKey);


	//Client Info 
	txt_LabelPlugins1.SetText(toWCHAR("%s's Temps", cData.name));

	wchar_t CPUTempText[10];
	swprintf(CPUTempText, L"%0.1f °F", GetTemperature(CPU, TRUE));
	txt_Cpu.SetText(CPUTempText);

	wchar_t GPUTempText[10];
	swprintf(GPUTempText, L"%0.1f °F", GetTemperature(GPU, TRUE));
	txt_Gpu.SetText(GPUTempText);

	wchar_t MEMTempText[10];
	swprintf(MEMTempText, L"%0.1f °F", GetTemperature(MEM, TRUE));
	txt_Mem.SetText(MEMTempText);

	wchar_t BRDTempText[10];
	swprintf(BRDTempText, L"%0.1f °F", GetTemperature(BRD, TRUE));
	txt_Mobo.SetText(BRDTempText);

	wchar_t kerneltext[10];
	swprintf(kerneltext, L"%i", XboxKrnlVersion->Build);
	Data5.SetText(kerneltext);
	return ERROR_SUCCESS;
}
HRESULT ScnPreferences::InitializeChildren()
{	
	GetChildById(L"labelHeading", &labelHeading); // labelHeading
	GetChildById(L"Data1", &Data1); // CPUKey
	GetChildById(L"Data2", &Data2); // Reserved Days
	GetChildById(L"Data3", &Data3); // Time Left
	GetChildById(L"Data4", &Data4); // Connection Status
	GetChildById(L"Subject5", &Subject5); // Kerneltitle
	GetChildById(L"Data5", &Data5); // Kernel
	GetChildById(L"Title", &Title); // Title
	GetChildById(L"txt_LabelPlugins", &txt_LabelPlugins); // labelHeading
	GetChildById(L"txt_TextPlugin1", &txt_TextPlugin1); // txt_TextPlugin1
	GetChildById(L"txt_TextPlugin2", &txt_TextPlugin2); // txt_TextPlugin2
	GetChildById(L"txt_TextPlugin3", &txt_TextPlugin3); // txt_TextPlugin3
	GetChildById(L"txt_TextPlugin4", &txt_TextPlugin4); // txt_TextPlugin4
	GetChildById(L"txt_TextPlugin5", &txt_TextPlugin5); // txt_TextPlugin5
	GetChildById(L"txt_LabelPlugins1", &txt_LabelPlugins1); // labelHeading
	GetChildById(L"txt_Cpu", &txt_Cpu); // txt_TextPlugin5
	GetChildById(L"txt_Gpu", &txt_Gpu); // txt_TextPlugin5
	GetChildById(L"txt_Mem", &txt_Mem); // txt_TextPlugin5
	GetChildById(L"txt_Mobo", &txt_Mobo); // txt_TextPlugin5
	return ERROR_SUCCESS;
}