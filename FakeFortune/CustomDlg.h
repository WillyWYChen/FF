#pragma once


// CustomDlg 對話方塊

class CustomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CustomDlg)

public:
	CustomDlg(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~CustomDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CUSTOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
protected:
//	CSpinButtonCtrl m_Spin;
public:
	CSpinButtonCtrl m_SpinUd;
	CSpinButtonCtrl m_SpinLr;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinLr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnMyMSG(WPARAM wPararm, LPARAM lParam);
};
