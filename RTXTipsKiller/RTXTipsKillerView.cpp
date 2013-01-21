#include "resource.h"
#include "RTXTipsKillerView.h"

RTXTipsKillerView* RTXTipsKillerView::ms_pInstance = NULL;
RTXTipsKillerView::RTXTipsKillerView(void) :
	m_hWnd(NULL)
{
}

RTXTipsKillerView::~RTXTipsKillerView(void)
{
}

RTXTipsKillerView* RTXTipsKillerView::GetView(HINSTANCE hInstance)
{
	if (ms_pInstance == NULL)
	{
		ms_pInstance = new RTXTipsKillerView();
		HWND hWnd = CreateDialogParam(
			hInstance,
			MAKEINTRESOURCE(IDD_RTX_TIPS_KILLER),
			NULL,
			RTXTipsKillerView::OnEvent,
			0);
	}

	return ms_pInstance;
}
	
void RTXTipsKillerView::Show(int nCmdShow)
{
	ShowWindow(m_hWnd, nCmdShow);
}

HWND RTXTipsKillerView::GetSafeHwnd() const
{
	return m_hWnd;
}

BOOL RTXTipsKillerView::HandleViewMessage(MSG* msg)
{
	return IsDialogMessage(m_hWnd, msg);
}

INT_PTR CALLBACK RTXTipsKillerView::OnEvent(
	HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		OnInitDialog(hDlg);
		return (INT_PTR)TRUE;

	case WM_CLOSE:
		DestroyWindow(hDlg);
		return (INT_PTR)TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		ms_pInstance->OnCommand(LOWORD(wParam));
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}

void RTXTipsKillerView::OnInitDialog(HWND hDlg)
{
	ms_pInstance->m_hWnd = hDlg;
	ms_pInstance->SetIcon();
	ms_pInstance->InspectTipsWindowClass();
}

void RTXTipsKillerView::SetIcon(void)
{
	HICON hIcon = LoadIcon(
		GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDI_RTX_TIPS_KILLER));
	if (hIcon != NULL)
	{
		SendMessage(
			ms_pInstance->m_hWnd,
			WM_SETICON,
			ICON_SMALL,
			(LPARAM)hIcon);

		SendMessage(
			ms_pInstance->m_hWnd,
			WM_SETICON,
			ICON_BIG,
			(LPARAM)hIcon);
	}
}

void RTXTipsKillerView::OnCommand(WORD wCommandId)
{
	switch (wCommandId)
	{
	case IDC_BTN_INSPECT:
		if (!InspectTipsWindowClass())
		{
			ShowNoTipsFoundDialog();
		}
		break;
	case IDC_BTN_CLOSE_TIPS:
		CloseTipsWindow();
		break;
	}
}

bool RTXTipsKillerView::InspectTipsWindowClass(void)
{
	HWND hTipsWindow = GetTipsWindow();
	if (hTipsWindow != NULL)
	{
		TCHAR szClassName[200] = {0};
		GetClassName(hTipsWindow, szClassName, sizeof(szClassName));
		SetDlgItemText(m_hWnd, IDC_STATIC_CLASS_NAME, szClassName);
		EnableCloseTipsButton();
		return true;
	}

	SetDlgItemText(m_hWnd, IDC_STATIC_CLASS_NAME, _T("-"));
	DisableCloseTipsButton();
	return false;
}

HWND RTXTipsKillerView::GetTipsWindow()
{
	RECT rcWorkArea = {0};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

	POINT stPoint = {0};
	stPoint.x = rcWorkArea.right - 130;
	stPoint.y = rcWorkArea.bottom - 80;

	HWND hTipsWindow = ::WindowFromPoint(stPoint);
	while (::GetParent(hTipsWindow) != NULL)
	{
		hTipsWindow = ::GetParent(hTipsWindow);
	}

	DWORD dwStyle = (DWORD)GetWindowLong(hTipsWindow, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(hTipsWindow, GWL_EXSTYLE);

	if (dwStyle == 0x94000000 && dwExStyle == 0x000088)
	{
		return hTipsWindow;
	}

	return NULL;
}

void RTXTipsKillerView::EnableCloseTipsButton()
{
	EnableWindow(GetDlgItem(m_hWnd, IDC_BTN_CLOSE_TIPS), TRUE);
}

void RTXTipsKillerView::DisableCloseTipsButton()
{
	EnableWindow(GetDlgItem(m_hWnd, IDC_BTN_CLOSE_TIPS), FALSE);
}

void RTXTipsKillerView::CloseTipsWindow(void)
{
	TCHAR szClassName[200] = {0};
	GetDlgItemText(m_hWnd, IDC_STATIC_CLASS_NAME, szClassName, sizeof(szClassName));

	if (szClassName[0] == _T('\0'))
	{
		ShowNoTipsFoundDialog();
		return;
	}

	int iNumberOfClosed = 0;

	HWND hTipsWindow;
	while ((hTipsWindow = FindWindow(szClassName, NULL)) != NULL)
	{
		SendMessage(hTipsWindow, WM_CLOSE, 0, 0);
		iNumberOfClosed++;
	}

	if (iNumberOfClosed == 0)
	{
		ShowNoTipsFoundDialog();
	}
	else
	{
		ShowTipsClosedDialog(iNumberOfClosed);
	}
}

void RTXTipsKillerView::ShowNoTipsFoundDialog()
{
	MessageBox(m_hWnd, _T("未能发现 RTX 弹窗"), NULL, MB_ICONINFORMATION);
}

void RTXTipsKillerView::ShowTipsClosedDialog(int iNumberOfClosed)
{
	TCHAR szMessage[100] = {0};	
	_stprintf_s(szMessage, _T( "成功关闭 %d 个 RTX 弹窗"), iNumberOfClosed);
	MessageBox(m_hWnd, szMessage, _T("信息"), MB_ICONINFORMATION);
}