#pragma once


// PoolDlg 對話方塊

class PoolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PoolDlg)

public:
	PoolDlg(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~PoolDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POOL_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	afx_msg LRESULT OnMyMSG(WPARAM wPararm, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateListContent();
};
