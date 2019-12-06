// CustomDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "FakeFortune.h"
#include "CustomDlg.h"
#include "afxdialogex.h"
#include "sharedata.h"

// CustomDlg 對話方塊

IMPLEMENT_DYNAMIC(CustomDlg, CDialogEx)

CustomDlg::CustomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CUSTOM, pParent)
{
	TRACE(TEXT("custom created\n"));
}

CustomDlg::~CustomDlg()
{
}

void CustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_UD, m_SpinUd);
	DDX_Control(pDX, IDC_SPIN_LR, m_SpinLr);
}


BEGIN_MESSAGE_MAP(CustomDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UD, &CustomDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LR, &CustomDlg::OnDeltaposSpinLr)
	ON_MESSAGE(WM_MY_MSG, OnMyMSG)
END_MESSAGE_MAP()


// CustomDlg 訊息處理常式

LRESULT CustomDlg::OnMyMSG(WPARAM wPararm, LPARAM lParam)
{
	switch (wPararm) {
	default:
		break;
	}

	return 0;
}

void CustomDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;

	if (pNMUpDown->iDelta == -1) {
		::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_MY_MSG, WPARAM_TEXT_UP, 0);
	}
	else if (pNMUpDown->iDelta == 1) {
		::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_MY_MSG, WPARAM_TEXT_DOWN, 0);
	}
}


void CustomDlg::OnDeltaposSpinLr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;

	if (pNMUpDown->iDelta == -1) {
		::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_MY_MSG, WPARAM_TEXT_RIGHT, 0);
	}
	else if (pNMUpDown->iDelta == 1) {
		::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_MY_MSG, WPARAM_TEXT_LEFT, 0);
	}
}
