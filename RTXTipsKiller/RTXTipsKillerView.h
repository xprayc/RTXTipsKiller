#pragma once

#include "stdafx.h"

class RTXTipsKillerView
{
public:
	RTXTipsKillerView(void);
	~RTXTipsKillerView(void);

public:
	static RTXTipsKillerView* GetView(HINSTANCE hInstance);

	void Show(int nCmdShow);
	HWND GetSafeHwnd(void) const;
	BOOL HandleViewMessage(MSG* message);

private:
	static INT_PTR CALLBACK OnEvent(
		HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void OnInitDialog(HWND hDlg);
	void SetIcon(void);
	void OnCommand(WORD wCommandId);
	bool InspectTipsWindowClass();
	HWND GetTipsWindow();
	void EnableCloseTipsButton();
	void DisableCloseTipsButton();
	void CloseTipsWindow();
	void ShowNoTipsFoundDialog();
	void ShowTipsClosedDialog(int iNumberOfClosed);

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	static RTXTipsKillerView* ms_pInstance;
};

static INT_PTR CALLBACK ViewEventHandler(
		HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
