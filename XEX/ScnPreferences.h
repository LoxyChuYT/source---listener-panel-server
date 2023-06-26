#pragma once
#include <xtl.h>
#include <xui.h>    // XUI Runtime
#include <xuiapp.h>

class ScnPreferences : CXuiSceneImpl
{
public:
	ScnPreferences() {}
	~ScnPreferences() {}

	static ScnPreferences& getInstance()
	{
		static ScnPreferences singleton;
		return singleton;
	}

	CXuiTextElement Data1, Data2, Data3, Data4, Data5, Subject5, Title, txt_TextPlugin1, txt_TextPlugin2, txt_TextPlugin3, txt_TextPlugin4, txt_TextPlugin5,
		txt_Cpu, txt_Gpu, txt_Mem, txt_Mobo, labelHeading, txt_LabelPlugins, txt_LabelPlugins1;

	XUI_IMPLEMENT_CLASS(ScnPreferences, L"ScnPreferences", XUI_CLASS_SCENE)
		XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_RENDER(OnRender)
		XUI_END_MSG_MAP()

	HRESULT OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	HRESULT OnRender(XUIMessageRender *pRenderData, BOOL& bHandled);

	HRESULT InitializeChildren();
};