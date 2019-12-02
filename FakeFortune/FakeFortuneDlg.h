
// FakeFortuneDlg.h: 標頭檔
//

#pragma once
#include "PictureEx.h"
#include "TransparentStatic.h"
#include "TransparentStatic2.h"
#include "TrueRandom.h"

// CFakeFortuneDlg 對話方塊
class CFakeFortuneDlg : public CDialogEx
{
// 建構
public:
	CFakeFortuneDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FAKEFORTUNE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;
	CPictureEx m_Picture;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMyMSG(WPARAM wPararm, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	int PromptLoadData();
	void UpdatePoolWindow();
	void UpdateHistoryWindow();
	void RemoveNthFromPool(int idx);

	void TypedDisplayNth(int idx, int type = 1);
	void EnableDrawButton(int en);

	DWORD GetRandom();
public:
	afx_msg void OnBnClickedButtonShowPool();
	afx_msg void OnBnClickedButtonShowHistory();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonType1Draw();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonType2Draw();

protected:
	CFont m_Font;
	CTransparentStatic m_ForeText;
	CTrueRandom m_Random;	
};
