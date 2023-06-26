#include "stdafx.h"
#include "HudHook.h"
#include "ScnTab5.h"
#include "ScnPreferences.h"
#include "ScnGuideInfo.h"
#include "Utilities.h"

extern HANDLE hXam;
extern HANDLE dllHandle;
extern BOOL customui;

VOID XamBuildXamResourceLocatorHook(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize) {
	if (lstrcmpW(pResource, L"notify.xur") == ERROR_SUCCESS)
		swprintf(pBuffer, L"section://%X,816ED6E0#38574ed8855.xur", dllHandle); 
	else XamBuildResourceLocator(hXam, L"xam", pResource, pBuffer, dwSize);
}

HRESULT XuiSceneCreateCallbackPostRef(LPCWSTR szBasePath, LPCWSTR szScenePath, PVOID pvInitData, HXUIOBJ *phScene) {
	if (lstrcmpW(szScenePath, L"Options.xur") == 0) {
		HXUIOBJ m_hRootScene = *phScene;
		HXUIOBJ newScene;
		XuiSceneCreate(toWCHAR("section://%X,816ED6E0#", dllHandle), L"PreferencesScene.xur", NULL, &newScene);
		XuiElementAddChild(m_hRootScene, newScene);
	}
	return S_OK;
}

HRESULT XuiSceneCreateHook(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ *hScene) {
	WCHAR cmp[64];
	HRESULT ret = E_FAIL;
	swprintf(cmp, L"section://%X,816ED6E0#GuideMain.xur", dllHandle);
	if (lstrcmpW(ScenePath, L"GuideMain.xur") == 0) {
		XuiSceneCreate(NULL, cmp, InitData, hScene);
		HXUIOBJ hChild;
		XuiSceneCreate(NULL, toWCHAR("section://%X,816ED6E0#GuideInfo.xur", dllHandle), NULL, &hChild);
		XuiElementAddChild(*hScene, hChild);
	}
	else if (lstrcmpW(BasePath, cmp) == 0) {
		XuiSceneCreate(L"section://@0,hud#", ScenePath, InitData, hScene);
	}
	else {
		XuiSceneCreate(BasePath, ScenePath, InitData, hScene);
	}
	ret = XuiSceneCreateCallbackPostRef(BasePath, ScenePath, InitData, hScene);
	return ret;
}

HRESULT XuiRegisterClassHook(const XUIClass * pClass, HXUICLASS *phClass)
{
	HRESULT ret = XuiRegisterClass(pClass, phClass);
	ScnPreferences::Register();
	ScnGuideInfo::Register();
	ScnTab5::Register();
	return ret;
}

HRESULT XuiUnregisterClassHook(LPCWSTR szClassName)
{
	HRESULT ret = XuiUnregisterClass(szClassName);
	ScnPreferences::Unregister();
	ScnGuideInfo::Unregister();
	ScnTab5::Unregister();
	return ret;
}

HRESULT VkXuiSceneCreateHook(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ *hScene) {
	HRESULT ret = E_FAIL;
	WCHAR keyboardBuffer[64];
	swprintf(keyboardBuffer, L"section://%X,816ED6E0#KeyboardBase.xur", dllHandle);
	if (lstrcmpW(ScenePath, L"KeyboardBase.xur") == 0)
		ret = XuiSceneCreate(NULL, keyboardBuffer, InitData, hScene);
	else ret = XuiSceneCreate(BasePath, ScenePath, InitData, hScene);
	return ret;
}

VOID Xamhackspinner(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize) {
	if (lstrcmpW(pResource, L"loadingRing.png") == 0) {
		swprintf(pBuffer, L"section://%X,816ED6E0#loadingRing.png", dllHandle);
	}
	else XamBuildResourceLocator(hXam, L"shrdres", pResource, pBuffer, dwSize);
}

VOID XamBuildGamercardResourceLocaterHook(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize) {
	if (lstrcmpW(pResource, L"GamerCard.xur") == 0)
		swprintf(pBuffer, L"section://%X,816ED6E0#GamerCard.xur", dllHandle);
	else XamBuildResourceLocator(hXam, L"gamercrd", pResource, pBuffer, dwSize);
}

VOID InitializeHudHooks(PLDR_DATA_TABLE_ENTRY ModuleHandle)
{
	if (customui) {
		PatchModuleImport(ModuleHandle, MODULE_XAM, 0x357, (DWORD)XuiSceneCreateHook);
		PatchModuleImport(ModuleHandle, MODULE_XAM, 0x34A, (DWORD)XuiRegisterClassHook);
		PatchModuleImport(ModuleHandle, MODULE_XAM, 0x362, (DWORD)XuiUnregisterClassHook);
	}
}

VOID InitializeVkHooks(PLDR_DATA_TABLE_ENTRY ModuleHandle)
{
	if (customui) {
		PatchModuleImport(ModuleHandle, MODULE_XAM, 0x357, (DWORD)VkXuiSceneCreateHook);
	}
}

VOID freevisuals() {
	//skin
	XuiFreeVisuals(L"btn_SignIn");
	XuiFreeVisuals(L"btn_Count_achiev");
	XuiFreeVisuals(L"XuiMessageBox4");
	XuiFreeVisuals(L"XuiMessageBox3");
	XuiFreeVisuals(L"XuiMessageBox2");
	XuiFreeVisuals(L"btn_Count");
	XuiFreeVisuals(L"XuiCheckbox");
	XuiFreeVisuals(L"XuiRadioButton");
	XuiFreeVisuals(L"XuiSlider");
	XuiFreeVisuals(L"btn_RecentPlayers");
	XuiFreeVisuals(L"btn_standard_list");
	XuiFreeVisuals(L"btn_standard");
	XuiFreeVisuals(L"XuiButtonGuide");
	XuiFreeVisuals(L"prgbr_Voicemail");
	//skin2
	XuiFreeVisuals(L"btn_tileGrid");
	XuiFreeVisuals(L"btn_3Line_Icon_prgbr");
	XuiFreeVisuals(L"btn_FitnessBack");
	XuiFreeVisuals(L"xuiButtonImageCenteredMusic");
	XuiFreeVisuals(L"btn_2Line-icon");
	XuiFreeVisuals(L"btn_oneline-icon");
	XuiFreeVisuals(L"btn_Fullwidth_Icon_string_x4");
	XuiFreeVisuals(L"btn_Fullwidth_Icon_string_x3");
	XuiFreeVisuals(L"btn_Fullwidth_2Icon_2string");
	XuiFreeVisuals(L"btn_twoline-icon_RightGS");
	XuiLoadVisualFromBinary(toWCHAR("section://%X,816ED6E0#skin.xur", dllHandle), NULL);
	XuiLoadVisualFromBinary(toWCHAR("section://%X,816ED6E0#skin2.xur", dllHandle), NULL);
}

DWORD setcolornotify() {
	if (customui) {
		freevisuals();
		PatchInJump((PDWORD)ResolveFunction(NAME_XAM, 789), (DWORD)Xamhackspinner, FALSE);
		XuiLoadVisualFromBinary(toWCHAR("section://%X,816ED6E0#c8b8df7d57.xur", dllHandle), NULL);
		PatchInJump((PDWORD)ResolveFunction(NAME_XAM, 0x31F), (DWORD)XamBuildXamResourceLocatorHook, FALSE);
		PatchInJump((PDWORD)ResolveFunction(NAME_XAM, 0x31D), (DWORD)XamBuildGamercardResourceLocaterHook, FALSE);
	}
	return ERROR_SUCCESS;
}