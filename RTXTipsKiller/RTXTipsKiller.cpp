#include "stdafx.h"
#include "RTXTipsKiller.h"
#include "RTXTipsKillerView.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int APIENTRY _tWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	RTXTipsKillerView* pView = RTXTipsKillerView::GetView(hInstance);
	pView->Show(nCmdShow);

	BOOL ret;
	MSG msg;

	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (ret == -1)
		{
			return ret;
		}

		if (!pView->HandleViewMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}