#pragma once
#include "stdafx.h"
#include <xtl.h>
#include <xui.h>
#include <xuiapp.h>

class ScnGuideInfo : CXuiSceneImpl
{
public:
	ScnGuideInfo() {}
	~ScnGuideInfo() {}

	static ScnGuideInfo& getInstance()
	{
		static ScnGuideInfo singleton;
		return singleton;
	}
	
	CXuiTextElement txt_Label,txt_KvSerialInfo, txt_TimeRemaining, txt_TextKV, txt_Kvstatus, txt_Kvdays;

		XUI_IMPLEMENT_CLASS(ScnGuideInfo, L"ScnGuideInfo", XUI_CLASS_SCENE)
		XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_RENDER(OnRender)
		XUI_END_MSG_MAP()

		DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	HRESULT OnRender(XUIMessageRender *pRenderData, BOOL& bHandled);
	DWORD InitializeChildren();
};