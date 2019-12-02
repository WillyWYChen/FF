// HistoryDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "FakeFortune.h"
#include "HistoryDlg.h"
#include "afxdialogex.h"
#include "sharedata.h"

// HistoryDlg 對話方塊

IMPLEMENT_DYNAMIC(HistoryDlg, CDialogEx)

HistoryDlg::HistoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTORY_DIALOG, pParent)
{

}

HistoryDlg::~HistoryDlg()
{
}

void HistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HistoryDlg, CDialogEx)
	ON_MESSAGE(WM_MY_MSG, OnMyMSG)
END_MESSAGE_MAP()


// HistoryDlg 訊息處理常式
BOOL HistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Add column to BatchList
	CListCtrl *list = (CListCtrl *)this->GetDlgItem(IDC_LIST_HISTORY);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	list->InsertColumn(
		0,              // Rank/order of item 
		TEXT("時間"),          // Caption for this header 
		LVCFMT_LEFT,    // Relative position of items under header 
		120);           // Width of items under header

	list->InsertColumn(1, TEXT("工號"), LVCFMT_LEFT, 60);
	list->InsertColumn(2, TEXT("姓名"), LVCFMT_LEFT, 60);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

LRESULT HistoryDlg::OnMyMSG(WPARAM wPararm, LPARAM lParam)
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

void HistoryDlg::UpdateListContent()
{
	CListCtrl *list = (CListCtrl *)(this->GetDlgItem(IDC_LIST_HISTORY));

	list->DeleteAllItems();

	for (int i = 0; i < gShareData.HistoryRecord.size(); i++) {
		int Count = list->GetItemCount();
		int nItem;
		CString id_str;
		id_str.Format("%d", gShareData.HistoryRecord[i].emplyee.id);	
		nItem = list->InsertItem(Count, gShareData.HistoryRecord[i].time);
		list->SetItemText(nItem, 1, id_str);
		list->SetItemText(nItem, 2, gShareData.HistoryRecord[i].emplyee.name);
	}
}