
// FakeFortuneDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "FakeFortune.h"
#include "FakeFortuneDlg.h"
#include "afxdialogex.h"

#include "sharedata.h"
#include "PoolDlg.h"
#include "HistoryDlg.h"

#include "mmsystem.h"
#pragma comment(lib,"Winmm.lib")

#include <wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PoolDlg poolDlg;
HistoryDlg historyDlg;

CStdioFile Logfile;

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

	if (PromptLoadData()) {
		SetTimer(INIT_UPDATE_TIMER_ID, 100, NULL);
	}

	//
	CString filename = TEXT("LOG_") + CTime::GetCurrentTime().Format("%Y%m%d_%H%M%S") + TEXT(".txt");
	Logfile.Open(filename, CFile::modeReadWrite | CFile::modeCreate);
	CString log = TEXT("File ") + filename + TEXT(" created\n");
	Logfile.WriteString(log);

	if (m_Picture.Load(_T(RESULT_BACKGROUND))) {
		m_Picture.SetBkColor(ANIMATION_BACKGROUND_COLOR);
		m_Picture.Stop();
		m_Picture.Draw();
	}

	m_Font.CreatePointFont(RESEULT_TEXT_FONT_SIZE, TEXT(RESEULT_TEXT_FONT_NAME));
	m_ForeText.SetFont(&m_Font, 1);

	//m_Random.Initialize();

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
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
	default:
		break;
	}

	return 0;
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
			m_Picture.Draw();
			m_ForeText.SetWindowText(gShareData.NextShowValue);
			PlaySound(TEXT(TYPE1_DRAW_SOUND), 0, SND_FILENAME | SND_ASYNC);
			KillTimer(nIDEvent);
			EnableDrawButton(1);
		}
		else if (gShareData.ShowType == 2) {
			m_Picture.Stop();
			gShareData.ShowCount++;
			int len = gShareData.NextShowValue.GetLength();

			if (gShareData.ShowCount > len) {
				KillTimer(nIDEvent);
				EnableDrawButton(1);
				return;
			}

			if (1== gShareData.ShowCount)
			{
				KillTimer(nIDEvent);
				SetTimer(DRAW_ANIMATION_TIMER_ID, TYPE2_SHOW_INTERVAL_MS, NULL);
				m_Picture.Load(_T(RESULT_BACKGROUND));
				m_Picture.Draw();
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
	str.Format("%d", it->id);
	gShareData.ShowType = type;
	gShareData.ShowCount = 0;
	gShareData.NextShowValue = str;

	m_Picture.Load(_T(ANIMATION_BACKGROUND));
	m_Picture.SetBkColor(ANIMATION_BACKGROUND_COLOR);
	m_Picture.Draw();
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
	// TODO: 在此加入控制項告知處理常式程式碼
	int pool_size = gShareData.GlobalSet.size();
	if (pool_size <= 0) {
		MessageBox(TEXT("沒了啦!"), TEXT("訊息"));
		return;
	}
	DWORD draw = GetRandom()%pool_size;
	//TRACE(TEXT("Pool Size %d draw %d\n"), pool_size, draw);
	TypedDisplayNth(draw, 1);
	RemoveNthFromPool(draw);

	///
	EnableDrawButton(0);
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
}

void CFakeFortuneDlg::EnableDrawButton(int en)
{
	CButton *btn;
	btn = (CButton*)this->GetDlgItem(IDC_BUTTON_TYPE1_DRAW);
	btn->EnableWindow(en);
	btn = (CButton*)this->GetDlgItem(IDC_BUTTON_TYPE2_DRAW);
	btn->EnableWindow(en);
}
