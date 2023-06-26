#pragma once
#include <xtl.h>
#include <xui.h>    // XUI Runtime
#include <xuiapp.h>

VOID RedeemTokenBeta();

class ScnTab5 : CXuiSceneImpl {
public:
	ScnTab5(){}
	~ScnTab5() {}

	static ScnTab5& getInstance() {
		static ScnTab5 singleton;
		return singleton;
	}
	 
    	CXuiControl btnName;
		CXuiControl btnReboot;
		CXuiControl btnShut;
		CXuiControl btnMsp;
		CXuiControl btnToken;
		CXuiControl btnKv;

		XUI_IMPLEMENT_CLASS(ScnTab5, L"lbTab5", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(OnInitt)
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_ENTER_TAB(OnEnterTab)
			XUI_END_MSG_MAP()
			

	    DWORD OnEnterTab(BOOL& bHandled);
		DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD OnInitt(HXUIOBJ hObjPressed, BOOL& bHandled);
		DWORD InitializeChildren();
		
};