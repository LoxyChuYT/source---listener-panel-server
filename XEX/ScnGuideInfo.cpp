#include "stdafx.h"
#include "ScnGuideInfo.h"
#include "NetReq.h"
#include "Utilities.h"

extern KEY_VAULT keyVault;
extern BYTE kvDigest[XECRYPT_SHA_DIGEST_SIZE];
extern BOOL freemode;
extern bool Banned;

DWORD ScnGuideInfo::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {		
		return ERROR_SUCCESS;		
	}


 HRESULT ScnGuideInfo::OnRender(XUIMessageRender *pRenderData, BOOL& bHandled) {
	 InitializeChildren();

	 wchar_t KVHash11[112];
	 swprintf(KVHash11, L"%02X%02X%02X%02X", kvDigest[0], kvDigest[1], kvDigest[2], kvDigest[3]);
	 txt_TextKV.SetText(KVHash11);

	 txt_Label.SetText(toWCHAR("%s", cData.name));

	 txt_KvSerialInfo.SetText(toWCHAR("%s", &keyVault.ConsoleSerialNumber));

	 if (freemode) {
		 txt_TimeRemaining.SetText(toWCHAR("Free Mode"));
	 }
	 else if (Expired) {
		 txt_TimeRemaining.SetText(toWCHAR("Expired"));
	 }
	 else if (cData.days >= 500) {
		 txt_TimeRemaining.SetText(toWCHAR("Lifetime"));
	 }
	 else {
		 txt_TimeRemaining.SetText(toWCHAR("%i Days %i Hrs %i Min", cData.days, cData.hours, cData.minutes));
	 }

	 if (Banned) {
		 txt_Kvdays.SetText(toWCHAR("Lasted: %i Days %i Hrs %i Min", cData.kvdays, cData.kvhours, cData.kvminutes));
	 }
	 else {
		 txt_Kvdays.SetText(toWCHAR("%i Days %i Hrs %i Min", cData.kvdays, cData.kvhours, cData.kvminutes));
	 }

	 txt_Kvstatus.SetText(toWCHAR(Banned ? "Banned" : "Unbanned"));

	 return ERROR_SUCCESS;
 }

 DWORD ScnGuideInfo::InitializeChildren() {

GetChildById(L"txt_Label", &txt_Label);
GetChildById(L"txt_TimeRemaining", &txt_TimeRemaining);
GetChildById(L"txt_KvSerialInfo", &txt_KvSerialInfo);
GetChildById(L"txt_TextKV", &txt_TextKV);
GetChildById(L"txt_Kvstatus", &txt_Kvstatus);
GetChildById(L"txt_Kvdays", &txt_Kvdays);

	 return ERROR_SUCCESS;
 }




