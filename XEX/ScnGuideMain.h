#pragma once

#include "stdafx.h"
#include <xtl.h>
#include <xui.h>    // XUI Runtime
#include <xuiapp.h> // XUI Class Library

class ScnGuideMain : CXuiSceneImpl
{
public:
	ScnGuideMain() {}
	~ScnGuideMain() {}

	static ScnGuideMain& getInstance()
	{
		static ScnGuideMain singleton;
		return singleton;
	}

	CXuiControl Header,
		ROL,
		imgControllerBattery,
		imgHeadsetBattery,
		btnY,
		btnX,
		btnB,
		Tabscene;

	HXUIOBJ _ScnGuideMain,
		ScnGuideMain0,
		ScnGuideMain1;
	//XUI msg map

	XUI_IMPLEMENT_CLASS(ScnGuideMain, L"ScnGuideMain", XUI_CLASS_SCENE)
		XUI_BEGIN_MSG_MAP()

		XUI_ON_XM_INIT(OnInit)
		//XUI_ON_XM_MY_MESSAGE(OnMyMessage)
		XUI_ON_XM_ENTER_TAB(OnEnterTab)

		XUI_END_MSG_MAP()

		HRESULT OnEnterTab(BOOL& bHandled);
	HRESULT OnInit(XUIMessageInit *pInitData, BOOL& bHandled);

	HRESULT InitializeChildren();
};