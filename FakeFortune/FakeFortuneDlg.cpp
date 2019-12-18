
// FakeFortuneDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "FakeFortune.h"
#include "FakeFortuneDlg.h"
#include "afxdialogex.h"

#include "sharedata.h"
#include "PoolDlg.h"
#include "HistoryDlg.h"
#include "CustomDlg.h"

#include "mmsystem.h"
#pragma comment(lib,"Winmm.lib")

#include <wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PoolDlg poolDlg;
HistoryDlg historyDlg;
CustomDlg customDlg;

CStdioFile Logfile;
int dlgInitialize = 0;

// Keep improving random function
class RandomSequence
{
	HCRYPTPROV hProvider;
public:
	RandomSequence(void) : hProvider(NULL) {
		if (FALSE == CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, 0)) {
			// failed, should we try to create a default provider?
			if (NTE_BAD_KEYSET == GetLastError()) {
				if (FALSE == CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
					// ensure the provider is NULL so we could use a backup plan
					hProvider = NULL;
				}
			}
		}
	}

	~RandomSequence(void) {
		if (NULL != hProvider) {
			CryptReleaseContext(hProvider, 0U);
		}
	}

	BOOL generate(BYTE* buf, DWORD len) {
		if (NULL != hProvider) {
			return CryptGenRandom(hProvider, len, buf);
		}
		return FALSE;
	}
};

DWORD CFakeFortuneDlg::GetRandom()
{
	DWORD res;
	RandomSequence random;
	random.generate((BYTE*)&res, sizeof(DWORD));

	return res;
}

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFakeFortuneDlg 對話方塊



CFakeFortuneDlg::CFakeFortuneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FAKEFORTUNE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFakeFortuneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MYPICTURE, m_Picture);
	DDX_Control(pDX, IDC_FORETEXT, m_ForeText);
	DDX_Control(pDX, IDC_FORETEXT2, m_ForeText2);
	DDX_Control(pDX, IDC_BUTTON_SHOW_POOL, m_poolBtn);
	DDX_Control(pDX, IDC_BUTTON_SHOW_HISTORY, m_historyBtn);
	DDX_Control(pDX, IDC_BUTTON_SHOW_CUSTOM, m_customBtn);
	DDX_Control(pDX, IDC_BUTTON_TYPE1_DRAW, m_type1Btn);
	DDX_Control(pDX, IDC_BUTTON_TYPE2_DRAW, m_type2Btn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_pauseBtn);
}

BEGIN_MESSAGE_MAP(CFakeFortuneDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_POOL, &CFakeFortuneDlg::OnBnClickedButtonShowPool)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_HISTORY, &CFakeFortuneDlg::OnBnClickedButtonShowHistory)
	ON_MESSAGE(WM_MY_MSG, OnMyMSG)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TYPE1_DRAW, &CFakeFortuneDlg::OnBnClickedButtonType1Draw)
	ON_WM_CLOSE()
//	ON_BN_CLICKED(IDC_STOP, &CFakeFortuneDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON_TYPE2_DRAW, &CFakeFortuneDlg::OnBnClickedButtonType2Draw)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_CUSTOM, &CFakeFortuneDlg::OnBnClickedButtonShowCustom)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CFakeFortuneDlg::OnBnClickedButtonPause)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFakeFortuneDlg 訊息處理常式

BOOL CFakeFortuneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	poolDlg.Create(IDD_POOL_DIALOG, NULL);
	historyDlg.Create(IDD_HISTORY_DIALOG, NULL);
	customDlg.Create(IDD_DIALOG_CUSTOM, NULL);

	if (PromptLoadData()) {
		SetTimer(INIT_UPDATE_TIMER_ID, 100, NULL);
	}

	//
	CString filename = TEXT("LOG_") + CTime::GetCurrentTime().Format("%Y%m%d_%H%M%S") + TEXT(".txt");
	Logfile.Open(filename, CFile::modeReadWrite | CFile::modeCreate);
	CString log = TEXT("File ") + filename + TEXT(" created\n");
	Logfile.WriteString(log);

	if (m_Picture.Load(_T(RESULT_BACKGROUND))) {
		m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
		m_Picture.Stop();
		m_Picture.Draw();
	}

	m_Font.CreatePointFont(RESEULT_TEXT_FONT_SIZE, TEXT(RESEULT_TEXT_FONT_NAME));
	m_ForeText.SetFont(&m_Font, 1);
	m_ForeText.m_color = RESEULT_TEXT_FONT_COLOR;

	m_Font2.CreatePointFont(RESEULT_TEXT_FONT2_SIZE, TEXT(RESEULT_TEXT_FONT2_NAME));
	m_ForeText2.SetFont(&m_Font2, 1);
	m_ForeText2.m_color = RESEULT_TEXT_FONT2_COLOR;

	EnablePauseButton(0);

	//////////////////////////////////
	GetDlgItem(IDC_BUTTON_SHOW_CUSTOM)->ShowWindow(0);
	debugFlag = 0;
	dlgInitialize = 1;

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

CString UTF8ToUnicode(char* UTF8)
{

	DWORD dwUnicodeLen;
	LPWSTR pwText;
	CString strUnicode;
	dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, NULL, 0);
	pwText = new WCHAR[dwUnicodeLen];
	if (!pwText)
	{
		return strUnicode;
	}
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, pwText, dwUnicodeLen);
	strUnicode.Format(_T("%s"), pwText);
	delete[]pwText;
	return strUnicode;
}

int CFakeFortuneDlg::PromptLoadData()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = TEXT("Comma-Separated Values (*.csv)\0*.csv\0All Files (*.*)\0*.*\0\0");
	dlg.m_ofn.lpstrTitle = TEXT("Load Data Base");

	CString filename;
	INT_PTR res = dlg.DoModal();
	TRACE(_T("res is %u\n"), res);
	if (res == IDOK) {
		filename = dlg.GetPathName();
	}

	CStdioFile file;
	if (!file.Open(filename, CFile::modeRead)) {
		return 0;
	}

	CString temp;
	while (1) {
		if (!file.ReadString(temp))
			break;
		//Translate
		char *pStr = (char*)temp.GetBuffer(temp.GetLength()); //取得str對象的原始字符串
		int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需緩存的多少
		wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申請緩存空間
		MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize * sizeof(wchar_t));//轉碼
		temp = CString(pBuffer);
		free(pBuffer); //釋放緩存

		// Parse string
		std::vector<CString> result = split(temp, TEXT(","));
		if (result.size() >= 2) {
			class EmployeeRecord record;
			record.id = _ttoi(result[0]);
			record.name = result[1];

			gShareData.GlobalSet.insert(record);
		}
	}
	
	file.Close();
	return 1;
}

void CFakeFortuneDlg::UpdatePoolWindow()
{
	// HWND
	::PostMessage(poolDlg.GetSafeHwnd(), WM_MY_MSG, WPARAM_UPDATE_GUI, 0);
}

void CFakeFortuneDlg::UpdateHistoryWindow()
{
	//
	::PostMessage(historyDlg.GetSafeHwnd(), WM_MY_MSG, WPARAM_UPDATE_GUI, 0);
}

void CFakeFortuneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CFakeFortuneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CFakeFortuneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFakeFortuneDlg::OnBnClickedButtonShowPool()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	poolDlg.ShowWindow(SW_SHOW);
	poolDlg.UpdateWindow();
}


void CFakeFortuneDlg::OnBnClickedButtonShowHistory()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	historyDlg.ShowWindow(SW_SHOW);
	historyDlg.UpdateWindow();
}

LRESULT CFakeFortuneDlg::OnMyMSG(WPARAM wPararm, LPARAM lParam)
{
	switch (wPararm) {
	case WPARAM_UPDATE_GUI:
		switch (lParam) {
		default:
			break;
		}
		break;
	case WPARAM_TEXT_UP:
		{
			RECT ctrlRect;
			m_ForeText.GetWindowRect(&ctrlRect);
			ScreenToClient(&ctrlRect);
			ctrlRect.top -= 1;
			ctrlRect.bottom -= 1;

			m_Picture.Load(_T(RESULT_BACKGROUND));
			m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
			m_Picture.Draw();
			m_ForeText.MoveWindow(&ctrlRect, 0);
			CString str;
			m_ForeText.GetWindowText(str);
			m_ForeText.SetWindowText(str);
		}
		break;
	case WPARAM_TEXT_DOWN:
		{
			RECT ctrlRect;
			m_ForeText.GetWindowRect(&ctrlRect);
			ScreenToClient(&ctrlRect);
			ctrlRect.top += 1;
			ctrlRect.bottom += 1;
			
			m_Picture.Load(_T(RESULT_BACKGROUND));
			m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
			m_Picture.Draw();
			m_ForeText.MoveWindow(&ctrlRect, 0);
			CString str;
			m_ForeText.GetWindowText(str);
			m_ForeText.SetWindowText(str);
		}
		break;
	case WPARAM_TEXT_LEFT:
		{
			RECT ctrlRect;
			m_ForeText.GetWindowRect(&ctrlRect);
			ScreenToClient(&ctrlRect);
			ctrlRect.left -= 1;
			ctrlRect.left -= 1;
			
			m_Picture.Load(_T(RESULT_BACKGROUND));
			m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
			m_Picture.Draw();
			m_ForeText.MoveWindow(&ctrlRect, 0);
			CString str;
			m_ForeText.GetWindowText(str);
			m_ForeText.SetWindowText(str);
		}
		break;
	case WPARAM_TEXT_RIGHT:
		{
			RECT ctrlRect;
			m_ForeText.GetWindowRect(&ctrlRect);
			ScreenToClient(&ctrlRect);
			ctrlRect.left += 1;
			ctrlRect.left += 1;
			
			m_Picture.Load(_T(RESULT_BACKGROUND));
			m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
			m_Picture.Draw();
			m_ForeText.MoveWindow(&ctrlRect, 0);
			CString str;
			m_ForeText.GetWindowText(str);
			m_ForeText.SetWindowText(str);
		}
		break;
	default:
		break;
	}

	return 0;
}

void CFakeFortuneDlg::EnablePauseButton(int en)
{
	CButton *btn = (CButton *)GetDlgItem(IDC_BUTTON_PAUSE);
	btn->EnableWindow(en);
}

void CFakeFortuneDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//

	CDialogEx::OnTimer(nIDEvent);

	if (INIT_UPDATE_TIMER_ID == nIDEvent) {
		TRACE(TEXT("Update Pool\n"));
		UpdatePoolWindow();
		KillTimer(nIDEvent);
	}
	else if (DRAW_ANIMATION_TIMER_ID == nIDEvent) {
		if (gShareData.ShowType == 1) {
			m_Picture.Stop();
			m_Picture.Load(_T(RESULT_BACKGROUND));
			m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
			m_Picture.Draw();
			m_ForeText.SetWindowText(gShareData.NextShowValue);
			m_ForeText2.SetWindowText(gShareData.NextShowName);
			PlaySound(TEXT(TYPE1_DRAW_SOUND), 0, SND_FILENAME | SND_ASYNC);
			KillTimer(nIDEvent);
			EnableDrawButton(1);
			RedrawButtons();
		}
		else if (gShareData.ShowType == 2) {
			m_Picture.Stop();
			gShareData.ShowCount++;
			int len = gShareData.NextShowValue.GetLength();

			if (gShareData.ShowCount > len) {
				m_ForeText2.SetWindowText(gShareData.NextShowName);
				PlaySound(TEXT(TYPE1_DRAW_SOUND), 0, SND_FILENAME | SND_ASYNC);
				KillTimer(nIDEvent);
				EnableDrawButton(1);
				EnablePauseButton(0);
				RedrawButtons();
				return;
			}

			if (1== gShareData.ShowCount)
			{
				KillTimer(nIDEvent);
				SetTimer(DRAW_ANIMATION_TIMER_ID, TYPE2_SHOW_INTERVAL_MS, NULL);
				m_Picture.Load(_T(RESULT_BACKGROUND));
				m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
				m_Picture.Draw();
				EnablePauseButton(1);
			}

			m_ForeText.SetWindowText(gShareData.NextShowValue.Mid(0, gShareData.ShowCount));
			PlaySound(TEXT(TYPE2_DRAW_SOUND), 0, SND_FILENAME | SND_ASYNC);
		}
	}
}

void CFakeFortuneDlg::RemoveNthFromPool(int idx)
{
	std::set<class EmployeeRecord >::iterator it;
	it = gShareData.GlobalSet.begin();
	for (int i = 0; i < idx; i++) {
		++it;
	}

	CString time = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString id_str;
	id_str.Format(TEXT("%d"), it->id);
	CString log = time + TEXT(",") + id_str + TEXT(",") + it->name + TEXT("\n");
	Logfile.WriteString(log);

	HistoryRecord record;
	record.time = time;
	record.emplyee.id = it->id;
	record.emplyee.name = it->name;
	gShareData.HistoryRecord.push_back(record);
	
	gShareData.GlobalSet.erase(it);
	
	UpdatePoolWindow();
	UpdateHistoryWindow();
}

void CFakeFortuneDlg::TypedDisplayNth(int idx, int type)
{
	std::set<class EmployeeRecord >::iterator it;
	it = gShareData.GlobalSet.begin();
	for (int i = 0; i < idx; i++) {
		++it;
	}

	//TRACE(TEXT("show type 1 emplyee %d win\n"), it->id);

	CString str;
	str.Format(TEXT("%d"), it->id);
	gShareData.ShowType = type;
	gShareData.ShowCount = 0;
	gShareData.NextShowValue = str;
	gShareData.NextShowName = it->name;

	m_Picture.Load(_T(ANIMATION_BACKGROUND));
	m_Picture.SetBkColor(ANIMATION_BACKGROUND_COLOR);
	m_Picture.Draw();
	m_ForeText2.SetWindowText(TEXT(""));
	SetTimer(DRAW_ANIMATION_TIMER_ID, ANIMATION_WAIT_MS, NULL);
}

void CFakeFortuneDlg::OnClose()
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	Logfile.Close();
	CDialogEx::OnClose();
}


void CFakeFortuneDlg::OnBnClickedButtonType1Draw()
{
	if (debugFlag) {
		// TODO: 在此加入控制項告知處理常式程式碼
		int i = 40;
		while (i--) {
			int pool_size = gShareData.GlobalSet.size();
			if (pool_size <= 0) {
				break;
			}
			DWORD draw = GetRandom() % pool_size;
			RemoveNthFromPool(draw);
		}
	}
	else {
		// TODO: 在此加入控制項告知處理常式程式碼
		int pool_size = gShareData.GlobalSet.size();
		if (pool_size <= 0) {
			MessageBox(TEXT("沒了啦!"), TEXT("訊息"));
			return;
		}
		DWORD draw = GetRandom() % pool_size;
		//TRACE(TEXT("Pool Size %d draw %d\n"), pool_size, draw);
		TypedDisplayNth(draw, 1);
		RemoveNthFromPool(draw);

		///
		EnableDrawButton(0);
	}
}

void CFakeFortuneDlg::OnBnClickedButtonType2Draw()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int pool_size = gShareData.GlobalSet.size();
	if (pool_size <= 0) {
		MessageBox(TEXT("沒了啦!"), TEXT("訊息"));
		return;
	}
	int draw = GetRandom() % pool_size;
	//TRACE(TEXT("Pool Size %d draw %d\n"), pool_size, draw);
	TypedDisplayNth(draw, 2);
	RemoveNthFromPool(draw);

	///
	EnableDrawButton(0);

	///
}

void CFakeFortuneDlg::EnableDrawButton(int en)
{
	CButton *btn;
	btn = (CButton*)this->GetDlgItem(IDC_BUTTON_TYPE1_DRAW);
	btn->EnableWindow(en);
	btn = (CButton*)this->GetDlgItem(IDC_BUTTON_TYPE2_DRAW);
	btn->EnableWindow(en);
}


void CFakeFortuneDlg::OnBnClickedButtonShowCustom()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	customDlg.ShowWindow(SW_SHOW);
	customDlg.UpdateWindow();
}

void CFakeFortuneDlg::MoveFromBottomRight(CWnd *wnd, int right, int bottom)
{
	RECT dlgRect;
	this->GetWindowRect(&dlgRect); //dlg's screen rect
	int dlgHeight = dlgRect.bottom - dlgRect.top;
	int dlgWidth = dlgRect.right - dlgRect.left;

	RECT cBtnRect;
	wnd->GetWindowRect(&cBtnRect);
	int cBtnHeight = cBtnRect.bottom - cBtnRect.top;
	int cBtnWidth = cBtnRect.right - cBtnRect.left;
	ScreenToClient(&cBtnRect);
	cBtnRect.right = dlgWidth - right;
	cBtnRect.left = cBtnRect.right - cBtnWidth;
	cBtnRect.bottom = dlgHeight - bottom;
	cBtnRect.top = cBtnRect.bottom - cBtnHeight;
	wnd->MoveWindow(&cBtnRect);
}

void CFakeFortuneDlg::ResizeCanvas(int width, int height)
{
	RECT cRect;
	m_Picture.GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	cRect.right = cRect.left + width;
	cRect.bottom = cRect.top + height;

	m_Picture.MoveWindow(&cRect);
}

void CFakeFortuneDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	TRACE(TEXT("Size changed\n"));
	if (!dlgInitialize)
		return;

	//////////////////////////////////////////////////

	MoveFromBottomRight(&m_poolBtn, 40, 420);
	MoveFromBottomRight(&m_historyBtn, 40, 350);
	MoveFromBottomRight(&m_pauseBtn, 40, 280);
	MoveFromBottomRight(&m_type1Btn, 40, 210);
	MoveFromBottomRight(&m_type2Btn, 40, 140);
	MoveFromBottomRight(&m_cancelBtn, 40, 70);

	//////////////////////////////////////////////////

	RECT dlgRect;
	this->GetWindowRect(&dlgRect); //dlg's screen rect
	int dlgHeight = dlgRect.bottom - dlgRect.top;
	int dlgWidth = dlgRect.right - dlgRect.left;

	//canvas 720 * 720
	RECT canvasRect;
	canvasRect.top = (dlgHeight / 2 - 360);
	canvasRect.bottom = canvasRect.top + 720;
	canvasRect.left = dlgWidth / 2 - 380;
	canvasRect.right = canvasRect.left + 720;
	m_Picture.MoveWindow(&canvasRect);

	//Text 650 * 300
	RECT textRect;
	textRect.left = canvasRect.left + 63;
	textRect.right = textRect.left + 650;
	textRect.top = canvasRect.top + 253;
	textRect.bottom = textRect.top + 300;
	m_ForeText.MoveWindow(&textRect);

	//Text 300 * 200
	textRect.left = textRect.left + 110;
	textRect.right = textRect.left + 500;
	textRect.top = textRect.top + 160;
	textRect.bottom = textRect.top + 200;
	m_ForeText2.MoveWindow(&textRect);

	//Redraw
	m_Picture.Load(_T(RESULT_BACKGROUND));
	m_Picture.SetBkColor(RESULT_BACKGROUND_COLOR);
	m_Picture.Draw();
	CString str;
	m_ForeText.GetWindowText(str);
	m_ForeText.SetWindowText(str);
	m_ForeText2.GetWindowText(str);
	m_ForeText2.SetWindowText(str);

	// 
	RedrawButtons();
}

void CFakeFortuneDlg::RedrawButtons()
{
	m_poolBtn.RedrawWindow();
	m_historyBtn.RedrawWindow();
	m_customBtn.RedrawWindow();
	m_type1Btn.RedrawWindow();
	m_type2Btn.RedrawWindow();
	m_cancelBtn.RedrawWindow();
	m_pauseBtn.RedrawWindow();
}

void CFakeFortuneDlg::OnBnClickedButtonPause()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (gShareData.Paused) {
		gShareData.Paused = 0;
		SetTimer(DRAW_ANIMATION_TIMER_ID, TYPE2_SHOW_INTERVAL_MS, NULL);
	}
	else {
		gShareData.Paused = 1;
		KillTimer(DRAW_ANIMATION_TIMER_ID);
	}
}


HBRUSH CFakeFortuneDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此變更 DC 的任何屬性
	if (nCtlColor == CTLCOLOR_DLG) {
		HBRUSH hbr1;
		// 這裡放你要的顏色 COLORREF m_curColor = RGB(255,0,0);
		COLORREF m_curColor = RGB(0, 135, 220);
		hbr1 = CreateSolidBrush(m_curColor);
		return hbr1;
	}
	// TODO:  如果預設值並非想要的，則傳回不同的筆刷
	return hbr;
}


BOOL CFakeFortuneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	if (pMsg->message == WM_KEYUP) {
		TRACE(TEXT("Released %d\n", pMsg->wParam));
		m_type1Btn.SetWindowText(TEXT("Small\nPrize"));
		debugFlag = 0;
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN) {
		TRACE(TEXT("Pressed %d\n", pMsg->wParam));
		if (debugFlag == 0) {
			debugFlag = 1;
			m_type1Btn.SetWindowText(TEXT("TEST 40 Draw"));
		}
		return TRUE;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}
