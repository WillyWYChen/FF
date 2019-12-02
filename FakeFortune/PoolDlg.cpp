// PoolDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "FakeFortune.h"
#include "PoolDlg.h"
#include "afxdialogex.h"
#include "sharedata.h"

// PoolDlg 對話方塊

IMPLEMENT_DYNAMIC(PoolDlg, CDialogEx)

PoolDlg::PoolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POOL_DIALOG, pParent)
{

}

PoolDlg::~PoolDlg()
{
}

void PoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PoolDlg, CDialogEx)
	ON_MESSAGE(WM_MY_MSG, OnMyMSG)
END_MESSAGE_MAP()


// PoolDlg 訊息處理常式
BOOL PoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Add column to BatchList
	CListCtrl *list = (CListCtrl *)this->GetDlgItem(IDC_LIST_POOL);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	list->InsertColumn(
		0,              // Rank/order of item 
		TEXT("工號"),          // Caption for this header 
		LVCFMT_LEFT,    // Relative position of items under header 
		60);           // Width of items under header

	list->InsertColumn(1, TEXT("姓名"), LVCFMT_LEFT, 60);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

LRESULT PoolDlg::OnMyMSG(WPARAM wPararm, LPARAM lParam)
{
	switch (wPararm) {
	case WPARAM_UPDATE_GUI:
		switch (lParam) {
		default:
			UpdateListContent();
			break;
		}
		break;
	default:
		break;
	}

	return 0;
}

void PoolDlg::UpdateListContent()
{
	CListCtrl *list = (CListCtrl *)(this->GetDlgItem(IDC_LIST_POOL));

	list->DeleteAllItems();

	for (std::set<struct EmployeeRecord >::iterator it = gShareData.GlobalSet.begin(); it != gShareData.GlobalSet.end(); ++it) {
		int Count = list->GetItemCount();
		int nItem;
		CString id_str;
		id_str.Format("%d", it->id);
		
		nItem = list->InsertItem(Count, id_str);
		list->SetItemText(nItem, 1, it->name);
	}
}
