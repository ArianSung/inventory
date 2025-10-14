
// InventoryManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "InventoryManager.h"
#include "InventoryManagerDlg.h"
#include "afxdialogex.h"
#include "CAddProductDlg.h"

#include "COrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInventoryManagerDlg 대화 상자


// 생성자
CInventoryManagerDlg::CInventoryManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INVENTORYMANAGER_DIALOG, pParent)
	, m_nCurrentTab(0)
	, m_nDangerThreshold(10)   // 위험 기준 기본값 10
	, m_nWarningThreshold(30)  // 주의 기준 기본값 30// 첫 번째 탭 선택
	, m_nTimerID(0)                 // 타이머 ID 초기화
	, m_nRefreshInterval(30)        // 기본 30초
	, m_bAutoRefresh(TRUE)          // 자동 새로고침 활성화
	, m_pDBManager(nullptr)
	, m_bDBConnected(FALSE)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 소멸자
CInventoryManagerDlg::~CInventoryManagerDlg()
{
	// ========================================
	// 1. 타이머 종료
	// ========================================
	if (m_nTimerID != 0)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}

	// ========================================
	// 2. DB 연결 해제
	// ========================================
	DisconnectDatabase();

	// ========================================
	// 3. DB 관리자 싱글톤 삭제
	// ========================================
	CDBManager::DestroyInstance();
	m_pDBManager = nullptr;
}

void CInventoryManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Control(pDX, IDC_LIST_INVENTORY, m_listInventory);
}

BEGIN_MESSAGE_MAP(CInventoryManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LOG, &CInventoryManagerDlg::OnBnClickedButtonClearLog)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CInventoryManagerDlg::OnBnClickedButtonRefresh)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CInventoryManagerDlg::OnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON_ORDER, &CInventoryManagerDlg::OnBnClickedButtonOrder)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INVENTORY, &CInventoryManagerDlg::OnDblclkListInventory)
	ON_BN_CLICKED(IDC_BUTTON2, &CInventoryManagerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInventoryManagerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CInventoryManagerDlg 메시지 처리기

BOOL CInventoryManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// ========================================
	// 초기화 작업
	// ========================================

	UpdateData(FALSE);

	// 탭 초기화
	InitTabs();

	InitInventoryList();

	// 환영 로그 추가
	AddLog(_T("✅ 프로그램 시작"));
	AddLog(_T("💡 The Most 재고 관리 시스템"));

	// ========================================
	// MySQL 라이브러리 버전 확인
	// ========================================
	CString strMySQLVersion;
	strMySQLVersion.Format(_T("📚 MySQL Client 버전: %s"),
		CString(mysql_get_client_info()));
	AddLog(strMySQLVersion);

	// ========================================
	// 데이터베이스 연결 시도
	// ========================================
	AddLog(_T("🔌 데이터베이스 연결 시도 중..."));
	ConnectDatabase();

	// ========================================
	// 재고 데이터 로드
	// ========================================
	if (m_bDBConnected)
	{
		LoadInventoryData();

		// 리스트 업데이트 (추가!)
		UpdateInventoryList();
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CInventoryManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CInventoryManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ========================================
// 로그 추가 함수
// ========================================
void CInventoryManagerDlg::AddLog(CString strLog)
{
	// 현재 시간 가져오기
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("[%H:%M:%S]"));

	CString strExistingLog;
	m_editLog.GetWindowText(strExistingLog);

	// 새 로그를 아래에 추가
	CString strNewLog;
	if (strExistingLog.IsEmpty())
		strNewLog.Format(_T("%s %s"), strTime, strLog);
	else
		strNewLog.Format(_T("%s\r\n%s %s"), strExistingLog, strTime, strLog);

	m_editLog.SetWindowText(strNewLog);

	// 마지막 줄로 스크롤
	int nLen = m_editLog.GetWindowTextLength();
	m_editLog.SetSel(nLen, nLen);
	m_editLog.SendMessage(EM_SCROLLCARET, 0, 0);
}

// ========================================
// 로그 지우기 함수
// ========================================
void CInventoryManagerDlg::ClearLog()
{
	m_editLog.SetWindowText(_T(""));
	AddLog(_T("로그를 지웠습니다."));
}

// ========================================
// 탭 초기화 함수
// ========================================
void CInventoryManagerDlg::InitTabs()
{
	// 기존 탭 모두 삭제
	m_tabMain.DeleteAllItems();

	// 탭 추가
	m_tabMain.InsertItem(0, _T("재고현황"));
	m_tabMain.InsertItem(1, _T("통계"));
	m_tabMain.InsertItem(2, _T("설정"));

	// 첫 번째 탭 선택
	m_tabMain.SetCurSel(0);
	m_nCurrentTab = 0;
}


void CInventoryManagerDlg::OnBnClickedButtonClearLog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 로그 지우기
	ClearLog();

}

void CInventoryManagerDlg::OnBnClickedButtonRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	AddLog(_T("🔄 수동 새로고침 실행"));

	// 연결 테스트
	if (TestConnection())
	{
		// 재고 데이터 새로고침
		RefreshInventoryData();

		AddLog(_T("✅ 새로고침 완료"));
	}
	else
	{
		// 재연결 시도
		AddLog(_T("🔌 재연결 시도 중..."));
		ConnectDatabase();

		if (m_bDBConnected)
		{
			LoadInventoryData();
		}
	}
}

void CInventoryManagerDlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 현재 선택된 탭 인덱스 가져오기
	int nSelectedTab = m_tabMain.GetCurSel();

	// 이전 탭과 다르면
	if (nSelectedTab != m_nCurrentTab)
	{
		m_nCurrentTab = nSelectedTab;

		// 탭별 로그 메시지
		CString strTabName;
		switch (nSelectedTab)
		{
		case 0:
			strTabName = _T("재고현황");
			break;
		case 1:
			strTabName = _T("통계");
			break;
		case 2:
			strTabName = _T("설정");
			break;
		default:
			strTabName = _T("알 수 없음");
			break;
		}

		CString strLog;
		strLog.Format(_T("📂 [%s] 탭으로 이동"), strTabName);
		AddLog(strLog);

		// TODO: 나중에 탭별로 다른 화면 표시
		// ShowTabContent(nSelectedTab);
	}

	*pResult = 0;
}

// ========================================
// 데이터베이스 연결
// ========================================
void CInventoryManagerDlg::ConnectDatabase()
{
	// DB 관리자 싱글톤 인스턴스 가져오기
	m_pDBManager = CDBManager::GetInstance();

	if (m_pDBManager == nullptr)
	{
		AddLog(_T("❌ DB 관리자 초기화 실패"));
		m_bDBConnected = FALSE;
		return;
	}

	// 연결 설정
	DB_CONFIG config;
	config.strHost = _T("192.168.0.92");        // ← MySQL 서버 주소
	config.nPort = 3306;                        // ← MySQL 포트
	config.strDatabase = _T("themost_db");      // ← 데이터베이스 이름
	config.strUser = _T("mfcuser");             // ← MySQL 사용자
	config.strPassword = _T("Moble1234");		// ← MySQL 비밀번호 (실제 비밀번호로!)

	// 연결 시도
	BOOL bResult = m_pDBManager->Connect(config);

	if (bResult)
	{
		// 연결 성공
		AddLog(_T("✅ 데이터베이스 연결 성공!"));

		CString strInfo;
		strInfo.Format(_T("📊 DB: %s@%s:%d"),
			config.strDatabase, config.strHost, config.nPort);
		AddLog(strInfo);

		m_bDBConnected = TRUE;
	}
	else
	{
		// 연결 실패
		CString strError;
		strError.Format(_T("❌ DB 연결 실패: %s"),
			m_pDBManager->GetLastError());
		AddLog(strError);

		m_bDBConnected = FALSE;

		// 에러 메시지 박스
		MessageBox(strError, _T("데이터베이스 연결 오류"), MB_OK | MB_ICONERROR);
	}
}

// ========================================
// 데이터베이스 연결 해제
// ========================================
void CInventoryManagerDlg::DisconnectDatabase()
{
	if (m_pDBManager != nullptr && m_bDBConnected)
	{
		m_pDBManager->Disconnect();
		AddLog(_T("🔌 데이터베이스 연결 해제"));
		m_bDBConnected = FALSE;
	}
}

// ========================================
// 연결 테스트
// ========================================
BOOL CInventoryManagerDlg::TestConnection()
{
	if (!m_bDBConnected || m_pDBManager == nullptr)
	{
		AddLog(_T("⚠️ 데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}

	// 간단한 쿼리로 연결 테스트
	CString strQuery = _T("SELECT 1");
	BOOL bResult = m_pDBManager->ExecuteSelect(strQuery);

	if (bResult)
	{
		AddLog(_T("✅ 연결 테스트 성공"));
		m_pDBManager->FreeResult();
		return TRUE;
	}
	else
	{
		CString strError;
		strError.Format(_T("❌ 연결 테스트 실패: %s"),
			m_pDBManager->GetLastError());
		AddLog(strError);
		return FALSE;
	}
}

// ========================================
// 재고 데이터 로드
// ========================================
void CInventoryManagerDlg::LoadInventoryData()
{
	// DB 연결 확인
	if (!m_bDBConnected || m_pDBManager == nullptr)
	{
		AddLog(_T("⚠️ 데이터베이스에 연결되지 않았습니다."));
		return;
	}

	AddLog(_T("📊 재고 데이터 로드 중..."));

	// 재고 목록 가져오기
	BOOL bResult = m_pDBManager->GetInventoryList(m_vecInventory);

	// ========== 디버그 로그 추가 ==========
	CString strDebug;
	strDebug.Format(_T("🔍 [DEBUG] GetInventoryList 결과: %s"),
		bResult ? _T("TRUE") : _T("FALSE"));
	AddLog(strDebug);

	strDebug.Format(_T("🔍 [DEBUG] 벡터 크기: %d"), (int)m_vecInventory.size());
	AddLog(strDebug);
	// =====================================

	if (bResult)
	{
		// 성공
		CString strLog;
		strLog.Format(_T("✅ 재고 데이터 로드 완료 (%d건)"), (int)m_vecInventory.size());
		AddLog(strLog);

		// 통계 정보
		int nDanger = 0;   // 위험 (10개 미만)
		int nWarning = 0;  // 주의 (10~29개)
		int nNormal = 0;   // 정상 (30개 이상)

		for (size_t i = 0; i < m_vecInventory.size(); i++)
		{
			if (m_vecInventory[i].nStock == 0)
			{
				// 품절은 위험에 포함
				nDanger++;
			}
			else if (m_vecInventory[i].nStock < m_nDangerThreshold)
			{
				nDanger++;
			}
			else if (m_vecInventory[i].nStock < m_nWarningThreshold)
			{
				nWarning++;
			}
			else
			{
				nNormal++;
			}
		}

		// 통계 로그
		CString strStats;
		strStats.Format(_T("📈 정상: %d건 / 주의: %d건 / 위험: %d건"),
			nNormal, nWarning, nDanger);
		AddLog(strStats);

		if (nDanger > 0)
		{
			CString strAlert;
			strAlert.Format(_T("⚠️ 긴급 발주 필요: %d건!"), nDanger);
			AddLog(strAlert);
		}
	}
	else
	{
		// 실패
		CString strError;
		strError.Format(_T("❌ 재고 데이터 로드 실패: %s"),
			m_pDBManager->GetLastError());
		AddLog(strError);
	}
}

// ========================================
// 재고 데이터 새로고침
// ========================================
void CInventoryManagerDlg::RefreshInventoryData()
{
	// 기존 데이터 클리어
	m_vecInventory.clear();
	//m_listInventory.DeleteAllItems(); //  리스트 컨트롤을 직접 비우는 코드 추가
	// 다시 로드
	LoadInventoryData();

	// 리스트 업데이트
	UpdateInventoryList();
	// TODO: 나중에 리스트 뷰 업데이트
	// UpdateInventoryListView();
}

// ========================================
// 재고 리스트 초기화
// ========================================
void CInventoryManagerDlg::InitInventoryList()
{
	AddLog(_T("🔍 InitInventoryList 시작"));

	// ========================================
	// 1. 리스트 핸들 확인
	// ========================================
	if (m_listInventory.GetSafeHwnd() == nullptr)
	{
		AddLog(_T("❌ 리스트 컨트롤 핸들이 없습니다!"));
		return;
	}

	// ========================================
	// 2. 기존 데이터 및 컬럼 삭제
	// ========================================
	m_listInventory.DeleteAllItems();

	int nColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
	for (int i = nColCount - 1; i >= 0; i--)
	{
		m_listInventory.DeleteColumn(i);
	}

	// ========================================
	// 3. 확장 스타일 설정
	// ========================================
	DWORD dwStyle = m_listInventory.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;   // 전체 행 선택
	dwStyle |= LVS_EX_GRIDLINES;       // 그리드 라인
	m_listInventory.SetExtendedStyle(dwStyle);

	// ========================================
	// 4. 컬럼 추가
	// ========================================
	int nCol = 0;
	nCol = m_listInventory.InsertColumn(0, _T("상태"), LVCFMT_CENTER, 60);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 0 추가 성공") : _T("❌ 컬럼 0 추가 실패"));

	nCol = m_listInventory.InsertColumn(1, _T("품번"), LVCFMT_LEFT, 150);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 1 추가 성공") : _T("❌ 컬럼 1 추가 실패"));

	nCol = m_listInventory.InsertColumn(2, _T("상품명"), LVCFMT_LEFT, 200);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 2 추가 성공") : _T("❌ 컬럼 2 추가 실패"));

	nCol = m_listInventory.InsertColumn(3, _T("브랜드"), LVCFMT_LEFT, 100);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 3 추가 성공") : _T("❌ 컬럼 3 추가 실패"));

	nCol = m_listInventory.InsertColumn(4, _T("색상"), LVCFMT_CENTER, 80);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 4 추가 성공") : _T("❌ 컬럼 4 추가 실패"));

	nCol = m_listInventory.InsertColumn(5, _T("사이즈"), LVCFMT_CENTER, 80);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 5 추가 성공") : _T("❌ 컬럼 5 추가 실패"));

	nCol = m_listInventory.InsertColumn(6, _T("재고"), LVCFMT_RIGHT, 80);
	AddLog(nCol >= 0 ? _T("✅ 컬럼 6 추가 성공") : _T("❌ 컬럼 6 추가 실패"));

	// ========================================
	// 5. 최종 확인
	// ========================================
	int nFinalColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
	CString strDebug;
	strDebug.Format(_T("📋 최종 컬럼 개수: %d"), nFinalColCount);
	AddLog(strDebug);

	if (nFinalColCount == 7)
	{
		AddLog(_T("✅ 재고 리스트 초기화 완료"));
	}
	else
	{
		AddLog(_T("❌ 재고 리스트 초기화 실패 (컬럼 개수 불일치)"));
	}
}

// ========================================
// 재고 리스트 업데이트 (수정 버전!)
// ========================================
void CInventoryManagerDlg::UpdateInventoryList()
{
	AddLog(_T("🔍 UpdateInventoryList 시작"));

	// ========================================
	// 1. 리스트 핸들 확인
	// ========================================
	if (m_listInventory.GetSafeHwnd() == nullptr)
	{
		AddLog(_T("❌ 리스트 컨트롤 핸들이 없습니다!"));
		return;
	}

	// ========================================
	// 2. 컬럼 개수 확인
	// ========================================
	int nColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
	if (nColCount != 7)
	{
		CString strError;
		strError.Format(_T("❌ 컬럼 개수 오류 (현재: %d, 필요: 7)"), nColCount);
		AddLog(strError);

		// 다시 초기화 시도
		AddLog(_T("🔄 리스트 재초기화 시도..."));
		InitInventoryList();
		return;
	}

	// ========================================
	// 3. 데이터 확인
	// ========================================
	CString strDebug;
	strDebug.Format(_T("🔍 업데이트할 데이터: %d건"), (int)m_vecInventory.size());
	AddLog(strDebug);

	if (m_vecInventory.size() == 0)
	{
		AddLog(_T("⚠️ 표시할 재고 데이터가 없습니다."));
		return;
	}

	// ========================================
	// 4. 리스트 업데이트 전 준비
	// ========================================
	m_listInventory.SetRedraw(FALSE);  // ✨ 다시 그리기 중지 (성능 향상)

	// ========================================
	// 5. 기존 항목 삭제
	// ========================================
	m_listInventory.DeleteAllItems();

	// ========================================
	// 6. 데이터 추가
	// ========================================
	int nAddedCount = 0;
	for (size_t i = 0; i < m_vecInventory.size(); i++)
	{
		const INVENTORY_ITEM& item = m_vecInventory[i];

		// 상태
		CString strStatus;
		if (item.nStock == 0)
			strStatus = _T("품절");
		else if (item.nStock < m_nDangerThreshold)
			strStatus = _T("위험");
		else if (item.nStock < m_nWarningThreshold)
			strStatus = _T("주의");
		else
			strStatus = _T("정상");

		// 행 추가 (첫 번째 컬럼만)
		int nIndex = m_listInventory.InsertItem((int)i, strStatus);

		if (nIndex == -1)
		{
			CString strError;
			strError.Format(_T("❌ 항목 추가 실패 (인덱스: %d)"), (int)i);
			AddLog(strError);
			continue;
		}

		// 나머지 컬럼 설정
		m_listInventory.SetItemText(nIndex, 1, item.strOptionCode);
		m_listInventory.SetItemText(nIndex, 2, item.strProductName);
		m_listInventory.SetItemText(nIndex, 3, item.strBrandName);
		m_listInventory.SetItemText(nIndex, 4, item.strColorName);
		m_listInventory.SetItemText(nIndex, 5, item.strSizeName);

		CString strStock;
		strStock.Format(_T("%d"), item.nStock);
		m_listInventory.SetItemText(nIndex, 6, strStock);

		m_listInventory.SetItemData(nIndex, (DWORD_PTR)item.nOptionID);

		nAddedCount++;

		// 처음 5개만 로그 (너무 많으면 느림)
		if (i < 5)
		{
			CString strItemLog;
			strItemLog.Format(_T("  ✓ [%d] %s - %s"),
				nIndex, item.strOptionCode, item.strProductName);
			AddLog(strItemLog);
		}
	}

	// ========================================
	// 7. 다시 그리기 활성화 (중요!)
	// ========================================
	m_listInventory.SetRedraw(TRUE);  // ✨ 다시 그리기 재개

	// ========================================
	// 8. 강제 갱신 (이게 핵심!)
	// ========================================
	m_listInventory.Invalidate(TRUE);  // ✨ TRUE = 배경도 지우고 다시 그림
	m_listInventory.UpdateWindow();    // ✨ 즉시 다시 그리기

	// ✨ 추가: 메시지 큐 처리
	MSG msg;
	while (PeekMessage(&msg, m_listInventory.GetSafeHwnd(), 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ========================================
	// 9. 완료
	// ========================================
	CString strLog;
	strLog.Format(_T("✅ 리스트 업데이트 완료 (%d건)"), nAddedCount);
	AddLog(strLog);
}

void CInventoryManagerDlg::OnBnClickedButtonOrder()
{
	// ========================================
	// 1. 선택된 항목 확인
	// ========================================
	int nSelected = m_listInventory.GetNextItem(-1, LVNI_SELECTED);

	if (nSelected == -1)
	{
		AfxMessageBox(_T("발주할 품목을 선택해주세요."));
		AddLog(_T("⚠️ 발주: 품목 선택 안 됨"));
		return;
	}

	// ========================================
	// 2. 선택된 품목 정보 가져오기
	// ========================================
	int nOptionID = (int)m_listInventory.GetItemData(nSelected);

	// 디버그 로그
	CString strDebugLog;
	strDebugLog.Format(_T("선택된 품목 - Option ID: %d, 인덱스: %d"), nOptionID, nSelected);
	AddLog(strDebugLog);

	// 벡터에서 해당 품목 찾기
	INVENTORY_ITEM* pItem = nullptr;
	for (size_t i = 0; i < m_vecInventory.size(); i++)
	{
		if (m_vecInventory[i].nOptionID == nOptionID)
		{
			pItem = &m_vecInventory[i];
			break;
		}
	}

	if (pItem == nullptr)
	{
		AfxMessageBox(_T("품목 정보를 찾을 수 없습니다."));
		AddLog(_T("❌ 발주: 품목 정보 없음"));
		return;
	}

	// 디버그 로그
	strDebugLog.Format(_T("품목 정보 - 품번: %s, 상품명: %s, 재고: %d"),
		pItem->strOptionCode, pItem->strProductName, pItem->nStock);
	AddLog(strDebugLog);

	// ========================================
	// 3. 발주 대화상자 띄우기
	// ========================================
	COrderDlg dlg;
	dlg.m_nOptionID = pItem->nOptionID;
	dlg.m_strOptionCode = pItem->strOptionCode;
	dlg.m_strProductName = pItem->strProductName;
	dlg.m_nCurrentStock = pItem->nStock;

	INT_PTR nResult = dlg.DoModal();

	if (nResult == IDOK)
	{
		// ========================================
		// 4. 발주 실행
		// ========================================
		CString strLog;
		strLog.Format(_T("📦 발주 시도: %s (수량: %d)"),
			pItem->strOptionCode, dlg.m_nOrderQuantity);
		AddLog(strLog);

		// DB 연결 확인
		if (!m_bDBConnected || m_pDBManager == nullptr)
		{
			AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
			AddLog(_T("❌ 발주 실패: DB 연결 없음"));
			return;
		}

		// 발주 실행
		BOOL bResult = m_pDBManager->AddStock(nOptionID, dlg.m_nOrderQuantity);

		if (bResult)
		{
			// 성공
			strLog.Format(_T("✅ 발주 성공: %s (%d개, %d → %d)"),
				pItem->strOptionCode,
				dlg.m_nOrderQuantity,
				pItem->nStock,
				pItem->nStock + dlg.m_nOrderQuantity);
			AddLog(strLog);

			AfxMessageBox(_T("발주가 완료되었습니다."));

			// 재고 데이터 새로고침
			RefreshInventoryData();
		}
		else
		{
			// 실패
			CString strError;
			strError.Format(_T("❌ 발주 실패: %s"),
				m_pDBManager->GetLastError());
			AddLog(strError);

			AfxMessageBox(strError);
		}
	}
	else
	{
		// 취소
		AddLog(_T("🚫 발주 취소됨"));
	}
}

// ========================================
// 창 닫기 처리
// ========================================
void CInventoryManagerDlg::OnClose()
{
	AddLog(_T("🚪 프로그램 종료 요청"));

	// ========================================
	// 1. 열려있는 모달 대화상자 찾아서 닫기
	// ========================================
	CWnd* pWnd = GetWindow(GW_ENABLEDPOPUP);

	if (pWnd != nullptr && pWnd != this)
	{
		AddLog(_T("⚠️ 열린 대화상자를 먼저 닫습니다..."));

		// ✨ 대화상자에 WM_CLOSE 메시지 전송
		pWnd->SendMessage(WM_CLOSE);

		// ✨ 메시지 처리 대기 (중요!)
		MSG msg;
		int nCount = 0;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) && nCount < 100)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			nCount++;
		}

		AddLog(_T("✅ 대화상자 닫기 완료"));
	}

	// ========================================
	// 2. DB 연결 해제
	// ========================================
	DisconnectDatabase();

	// ========================================
	// 3. 종료 로그
	// ========================================
	AddLog(_T("👋 프로그램 종료 중..."));

	// ========================================
	// 4. 기본 처리 (대화상자 닫기)
	// ========================================
	CDialogEx::OnClose();
}

void CInventoryManagerDlg::OnDblclkListInventory(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// ========================================
	// 1. 클릭된 항목 확인
	// ========================================
	int nItem = pNMItemActivate->iItem;

	if (nItem == -1)
	{
		// 빈 공간 더블클릭
		AddLog(_T("⚠️ 재고 수정: 항목이 선택되지 않음"));
		*pResult = 0;
		return;
	}

	// ========================================
	// 2. 선택된 항목 정보 가져오기
	// ========================================
	int nOptionID = (int)m_listInventory.GetItemData(nItem);

	// 디버그 로그
	CString strDebugLog;
	strDebugLog.Format(_T("🖱️ 더블클릭 - 항목 인덱스: %d, Option ID: %d"), nItem, nOptionID);
	AddLog(strDebugLog);

	// 벡터에서 해당 품목 찾기
	INVENTORY_ITEM* pItem = nullptr;
	for (size_t i = 0; i < m_vecInventory.size(); i++)
	{
		if (m_vecInventory[i].nOptionID == nOptionID)
		{
			pItem = &m_vecInventory[i];
			break;
		}
	}

	if (pItem == nullptr)
	{
		AfxMessageBox(_T("품목 정보를 찾을 수 없습니다."));
		AddLog(_T("❌ 재고 수정: 품목 정보 없음"));
		*pResult = 0;
		return;
	}

	// 디버그 로그
	strDebugLog.Format(_T("📦 선택된 품목 - 품번: %s, 상품명: %s, 재고: %d"),
		pItem->strOptionCode, pItem->strProductName, pItem->nStock);
	AddLog(strDebugLog);

	// ========================================
	// 3. 재고 수정 대화상자 띄우기
	// ========================================
	CEditStockDlg dlg;
	dlg.m_nOptionID = pItem->nOptionID;
	dlg.m_strOptionCode = pItem->strOptionCode;
	dlg.m_strProductName = pItem->strProductName;
	dlg.m_nCurrentStock = pItem->nStock;

	INT_PTR nResult = dlg.DoModal();

	if (nResult == IDOK)
	{
		// ========================================
		// 4. 재고 수정 실행
		// ========================================
		CString strLog;
		strLog.Format(_T("✏️ 재고 수정 시도: %s (%d → %d)"),
			pItem->strOptionCode, pItem->nStock, dlg.m_nNewStock);
		AddLog(strLog);

		// DB 연결 확인
		if (!m_bDBConnected || m_pDBManager == nullptr)
		{
			AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
			AddLog(_T("❌ 재고 수정 실패: DB 연결 없음"));
			*pResult = 0;
			return;
		}

		// 재고 업데이트
		BOOL bResult = m_pDBManager->UpdateStock(nOptionID, dlg.m_nNewStock);

		if (bResult)
		{
			// 성공
			strLog.Format(_T("✅ 재고 수정 성공: %s (%d → %d, 변경: %+d)"),
				pItem->strOptionCode,
				pItem->nStock,
				dlg.m_nNewStock,
				dlg.m_nNewStock - pItem->nStock);
			AddLog(strLog);

			AfxMessageBox(_T("재고가 수정되었습니다."));

			// 메시지 큐 정리
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// 재고 데이터 새로고침
			RefreshInventoryData();

			// 다시 한번 메시지 큐 정리
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 실패
			CString strError;
			strError.Format(_T("❌ 재고 수정 실패: %s"),
				m_pDBManager->GetLastError());
			AddLog(strError);

			AfxMessageBox(strError);
		}
	}
	else
	{
		// 취소
		AddLog(_T("🚫 재고 수정 취소됨"));
	}

	*pResult = 0;
}

void CInventoryManagerDlg::OnBnClickedButton2()
{
	//// 1. 리스트 컨트롤에서 선택된 항목의 인덱스를 가져옵니다.
	//int nSelectedItem = m_listInventory.GetNextItem(-1, LVNI_SELECTED);

	//if (nSelectedItem == -1)
	//{
	//	AfxMessageBox(_T("삭제할 품목을 먼저 선택해주세요."));
	//	AddLog(_T("⚠️ 삭제: 품목이 선택되지 않음"));
	//	return;
	//}

	//// 2. 선택된 항목의 고유 ID (OptionID)와 품번을 가져옵니다.
	//// GetItemData를 통해 저장해 둔 nOptionID를 가져오는 것이 가장 정확합니다.
	//int nOptionID = (int)m_listInventory.GetItemData(nSelectedItem);
	//CString strProductCode = m_listInventory.GetItemText(nSelectedItem, 1); // 품번은 1번 열

	//// 3. 사용자에게 삭제 여부를 다시 한번 확인합니다.
	//CString strConfirmMsg;
	//strConfirmMsg.Format(_T("품번 '%s'을(를) 정말로 삭제하시겠습니까?\n이 작업은 되돌릴 수 없습니다."), strProductCode);
	//if (AfxMessageBox(strConfirmMsg, MB_YESNO | MB_ICONWARNING) != IDYES)
	//{
	//	AddLog(_T("🚫 삭제가 사용자에 의해 취소됨"));
	//	return;
	//}

	//// 4. 데이터베이스에서 해당 레코드 삭제
	//// DB 연결 상태를 확인합니다.
	//if (!m_bDBConnected || m_pDBManager == nullptr)
	//{
	//	AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
	//	AddLog(_T("❌ 삭제 실패: DB 연결 없음"));
	//	return;
	//}

	//CString strLog;
	//strLog.Format(_T("🗑️ 삭제 시도: %s (OptionID: %d)"), strProductCode, nOptionID);
	//AddLog(strLog);

	//// CDBManager를 통해 품목 삭제 함수를 호출합니다 (이 함수는 CDBManager에 추가해야 함).
	//BOOL bResult = m_pDBManager->DeleteInventoryItem(nOptionID);

	//if (bResult)
	//{
	//	// 5. 성공 시 화면 새로고침
	//	AfxMessageBox(_T("선택한 품목이 성공적으로 삭제되었습니다."));
	//	AddLog(_T("✅ 삭제 성공!"));

	//	// 전체 데이터를 다시 로드하여 화면을 갱신합니다.
	//	RefreshInventoryData();
	//}
	//else
	//{
	//	// 6. 실패 시 오류 메시지 표시
	//	CString strError;
	//	strError.Format(_T("❌ 삭제 실패: %s"), m_pDBManager->GetLastError());
	//	AddLog(strError);
	//	AfxMessageBox(strError);
	//}

	////////////////////////////////
	int nSelectedItem = m_listInventory.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		AfxMessageBox(_T("삭제할 품목을 먼저 선택해주세요."));
		AddLog(_T("⚠️ 삭제: 품목이 선택되지 않음"));
		return;
	}

	int nOptionID = (int)m_listInventory.GetItemData(nSelectedItem);
	CString strProductCode = m_listInventory.GetItemText(nSelectedItem, 1);

	// ▼▼▼ 진단 코드 추가 ▼▼▼
	CString strDebugMsg;
	strDebugMsg.Format(_T("선택된 품목의 Option ID는 [ %d ] 입니다.\n이 ID로 DB에서 삭제를 시도합니다."), nOptionID);
	AfxMessageBox(strDebugMsg);
	// ▲▲▲ 여기까지 추가 ▲▲▲

	CString strConfirmMsg;
	strConfirmMsg.Format(_T("품번 '%s'을(를) 정말로 삭제하시겠습니까?\n이 작업은 되돌릴 수 없습니다."), strProductCode);
	if (AfxMessageBox(strConfirmMsg, MB_YESNO | MB_ICONWARNING) != IDYES)
	{
		AddLog(_T("🚫 삭제가 사용자에 의해 취소됨"));
		return;
	}

	if (!m_bDBConnected || m_pDBManager == nullptr)
	{
		AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
		AddLog(_T("❌ 삭제 실패: DB 연결 없음"));
		return;
	}

	CString strLog;
	strLog.Format(_T("🗑️ 삭제 시도: %s (OptionID: %d)"), strProductCode, nOptionID);
	AddLog(strLog);

	BOOL bResult = m_pDBManager->DeleteInventoryItem(nOptionID);

	if (bResult)
	{
		AfxMessageBox(_T("선택한 품목이 성공적으로 삭제되었습니다."));
		AddLog(_T("✅ 삭제 성공!"));
		RefreshInventoryData();
	}
	else
	{
		// 🔴 DBManager가 전달한 상세 오류 메시지를 표시합니다.
		CString strError;
		strError.Format(_T("❌ 삭제 실패: %s"), m_pDBManager->GetLastError());
		AddLog(strError);
		AfxMessageBox(strError);
	}
}

void CInventoryManagerDlg::OnBnClickedButton3()
{
	CAddProductDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// 상품 추가에 성공하면 목록을 새로고침합니다.
		AddLog(_T("✨ 새 상품이 추가되었습니다. 목록을 새로고침합니다."));
		RefreshInventoryData();
	}
}
