
// FakeFortuneDlg.h: 標頭檔
//

#pragma once
#include "PictureEx.h"
#include "TransparentStatic.h"
#include "TransparentStatic2.h"
//#include "TrueRandom.h"

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
	void EnablePauseButton(int en);

	void MoveFromBottomRight(CWnd *wnd, int right, int bottom);

	void ResizeCanvas(int width, int height);

	void DoHitSpaceBar();

	DWORD GetRandom();

	void RedrawButtons();
public:
	afx_msg void OnBnClickedButtonShowPool();
	afx_msg void OnBnClickedButtonShowHistory();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonType1Draw();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonType2Draw();

protected:
	CFont m_Font;
	CFont m_Font2;
	//CTrueRandom m_Random;
	int debugFlag;
public:
	afx_msg void OnBnClickedButtonShowCustom();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CPictureEx m_Picture;
	CTransparentStatic m_ForeText;
	CTransparentStatic m_ForeText2;
	CButton m_poolBtn;
	CButton m_historyBtn;
	CButton m_customBtn;
	CButton m_type1Btn;
	CButton m_type2Btn;
	CButton m_cancelBtn;
	afx_msg void OnBnClickedButtonPause();
	CButton m_pauseBtn;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_RadioSmall;
	CButton m_RadioBig;
};
