#include "stdafx.h"
#include "ScnGuideMain.h"
#include "Utilities.h"

HRESULT ScnGuideMain::OnInit(XUIMessageInit *pInitData, BOOL& bHandled)
{
	InitializeChildren();
	return ERROR_SUCCESS;
}

HRESULT ScnGuideMain::OnEnterTab(BOOL& bHandled)
{
	return ERROR_SUCCESS;
}

HRESULT ScnGuideMain::InitializeChildren()
{
	return ERROR_SUCCESS;
}