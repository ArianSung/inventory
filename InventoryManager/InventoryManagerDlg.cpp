// InventoryManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "InventoryManager.h"
#include "InventoryManagerDlg.h"
#include "afxdialogex.h"
#include "CAddProductDlg.h"
#include "COrderDlg.h"
#include "CStatsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// === [소문자 변환 헬퍼] ==================================
static CString ToLower(const CString& s) {
    CString t = s;
    t.MakeLower();
    return t;
}
// =========================================================


// CInventoryManagerDlg 대화 상자

// 생성자
CInventoryManagerDlg::CInventoryManagerDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_INVENTORYMANAGER_DIALOG, pParent)
    , m_nCurrentTab(0)
    , m_nDangerThreshold(10)    // 위험 기준 기본값 10
    , m_nWarningThreshold(30)   // 주의 기준 기본값 30
    , m_nTimerID(0)             // 타이머 ID 초기화
    , m_nRefreshInterval(30)    // 기본 30초
    , m_bAutoRefresh(TRUE)      // 자동 새로고침 활성화
    , m_pDBManager(nullptr)
    , m_bDBConnected(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 소멸자
CInventoryManagerDlg::~CInventoryManagerDlg()
{
    if (m_nTimerID != 0) { KillTimer(m_nTimerID); m_nTimerID = 0; }
    DisconnectDatabase();
    CDBManager::DestroyInstance();
    m_pDBManager = nullptr;
}

void CInventoryManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
    DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
    DDX_Control(pDX, IDC_LIST_INVENTORY, m_listInventory);
    DDX_Control(pDX, IDC_COMBO_FILTER_BRAND, m_comboFilterBrand);
    DDX_Control(pDX, IDC_COMBO_FILTER_CATEGORY, m_comboFilterCategory);
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
    ON_BN_CLICKED(IDC_BTN_SEARCH, &CInventoryManagerDlg::OnBnClickedBtnSearch) // 검색 버튼
    ON_CBN_SELCHANGE(IDC_COMBO_FILTER_BRAND, &CInventoryManagerDlg::OnSelchangeComboFilter)
    ON_CBN_SELCHANGE(IDC_COMBO_FILTER_CATEGORY, &CInventoryManagerDlg::OnSelchangeComboFilter)
END_MESSAGE_MAP()


// CInventoryManagerDlg 메시지 처리기

BOOL CInventoryManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 아이콘 설정
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    UpdateData(FALSE);

    // 검색 컨트롤 연결
    m_editSearch.SubclassDlgItem(IDC_EDIT_SEARCH, this);
    m_btnSearch.SubclassDlgItem(IDC_BTN_SEARCH, this);

    // 탭/리스트 초기화
    InitTabs();
    InitInventoryList();

    // 환영 로그
    AddLog(_T("✅ 프로그램 시작"));
    AddLog(_T("💡 The Most 재고 관리 시스템"));

    // MySQL 라이브러리 버전 확인 (유니코드 안전)
    CString strMySQLVersion;
    strMySQLVersion.Format(_T("📚 MySQL Client 버전: %S"), mysql_get_client_info());
    AddLog(strMySQLVersion);

    // DB 연결
    AddLog(_T("🔌 데이터베이스 연결 시도 중..."));
    ConnectDatabase();

    // 재고 데이터 로드/표시/캐시
    if (m_bDBConnected)
    {
        // 콤보 박스 초기화 (첫 항목으로 "전체" 추가)
        m_comboFilterBrand.InsertString(0, _T("전체 브랜드"));
        m_comboFilterBrand.SetCurSel(0); // 0번째 항목("전체 브랜드")을 기본으로 선택

        m_comboFilterCategory.InsertString(0, _T("전체 카테고리"));
        m_comboFilterCategory.SetCurSel(0);

        // DB에서 브랜드 목록 가져와서 콤보 박스에 추가하기
        std::vector<CString> vecBrands;
        if (m_pDBManager->GetBrandList(vecBrands)) // DB매니저에게 브랜드 목록 요청
        {
            for (const auto& brand : vecBrands) // 가져온 목록을 하나씩
            {
                m_comboFilterBrand.AddString(brand); // 콤보 박스에 추가
            }
        }

        // DB에서 카테고리 목록 가져와서 콤보 박스에 추가하기
        std::vector<CString> vecCategories;
        if (m_pDBManager->GetCategoryList(vecCategories)) // DB매니저에게 카테고리 목록 요청
        {
            for (const auto& category : vecCategories) // 가져온 목록을 하나씩
            {
                m_comboFilterCategory.AddString(category); // 콤보 박스에 추가
            }
        }

        LoadInventoryData();
        UpdateInventoryList();
        SnapshotDisplayToCache();
    }
    else
    {
        AddLog(_T("❌ DB 연결 실패. 오프라인 모드로 동작합니다."));
    }

    if (!m_pStatsDlg) {
        m_pStatsDlg = new CStatsDlg();
        m_pStatsDlg->Create(IDD_STATS_DIALOG, this);           // 자식으로 만들기
        m_pStatsDlg->InitDB(m_pDBManager, m_bDBConnected);     // DB 주입

        // 플레이스홀더 위치/크기에 맞춰서 배치
        CRect rc;
        GetDlgItem(IDC_PLACE_STATS)->GetWindowRect(&rc);
        ScreenToClient(&rc);
        m_pStatsDlg->SetWindowPos(nullptr, rc.left, rc.top, rc.Width(), rc.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);

        m_pStatsDlg->ShowWindow(SW_HIDE);  // 시작은 숨김
    }
    // [ADD] 통계 호스트(Static) 처음엔 숨김 + 클릭 못 하게
    if (CWnd* pHost = GetDlgItem(IDC_PLACE_STATS)) {
        pHost->ShowWindow(SW_HIDE);
        pHost->EnableWindow(FALSE);
        pHost->SetWindowText(_T("")); // (선택) "Static" 글자 지우기
    }


    // [EDIT] 현재 탭에 맞춰 표시/숨김
    ShowTabPage(m_tabMain.GetCurSel());

    return TRUE;
}

// 그림 처리
void CInventoryManagerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect; GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CInventoryManagerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 로그 추가
void CInventoryManagerDlg::AddLog(CString strLog)
{
    CTime time = CTime::GetCurrentTime();
    CString strTime = time.Format(_T("[%H:%M:%S]"));

    CString strExistingLog; m_editLog.GetWindowText(strExistingLog);

    CString strNewLog;
    if (strExistingLog.IsEmpty())
        strNewLog.Format(_T("%s %s"), strTime, strLog);
    else
        strNewLog.Format(_T("%s\r\n%s %s"), strExistingLog, strTime, strLog);

    m_editLog.SetWindowText(strNewLog);

    int nLen = m_editLog.GetWindowTextLength();
    m_editLog.SetSel(nLen, nLen);
    m_editLog.SendMessage(EM_SCROLLCARET, 0, 0);
}

// 로그 지우기
void CInventoryManagerDlg::ClearLog()
{
    m_editLog.SetWindowText(_T(""));
    AddLog(_T("로그를 지웠습니다."));
}

// 탭 초기화
void CInventoryManagerDlg::InitTabs()
{
    m_tabMain.DeleteAllItems();
    m_tabMain.InsertItem(0, _T("재고현황"));
    m_tabMain.InsertItem(1, _T("통계"));
    m_tabMain.InsertItem(2, _T("설정"));
    m_tabMain.SetCurSel(0);
    m_nCurrentTab = 0;
}

void CInventoryManagerDlg::OnBnClickedButtonClearLog()
{
    ClearLog();
}

void CInventoryManagerDlg::OnBnClickedButtonRefresh()
{
    AddLog(_T("🔄 수동 새로고침 실행"));

    if (TestConnection())
    {
        RefreshInventoryData();  // 내부에서 SnapshotDisplayToCache 호출
        AddLog(_T("✅ 새로고침 완료"));
    }
    else
    {
        AddLog(_T("🔌 재연결 시도 중..."));
        ConnectDatabase();
        if (m_bDBConnected) {
            LoadInventoryData();
            UpdateInventoryList();
            SnapshotDisplayToCache();
        }
    }
}
void CInventoryManagerDlg::OnSelchangeTabMain(NMHDR*, LRESULT* pResult)
{
    m_nCurrentTab = m_tabMain.GetCurSel();
    ShowTabPage(m_nCurrentTab);
    *pResult = 0;
}



// DB 연결
void CInventoryManagerDlg::ConnectDatabase()
{
    m_pDBManager = CDBManager::GetInstance();
    if (m_pDBManager == nullptr)
    {
        AddLog(_T("❌ DB 관리자 초기화 실패"));
        m_bDBConnected = FALSE;
        return;
    }

    DB_CONFIG config;
    config.strHost = _T("192.168.0.92");
    config.nPort = 3306;
    config.strDatabase = _T("themost_db");
    config.strUser = _T("mfcuser");
    config.strPassword = _T("Moble1234");

    BOOL bResult = m_pDBManager->Connect(config);

    if (bResult)
    {
        AddLog(_T("✅ 데이터베이스 연결 성공!"));
        CString strInfo;
        strInfo.Format(_T("📊 DB: %s@%s:%d"), config.strDatabase, config.strHost, config.nPort);
        AddLog(strInfo);
        m_bDBConnected = TRUE;
    }
    else
    {
        CString strError;
        strError.Format(_T("❌ DB 연결 실패: %s"), m_pDBManager->GetLastError());
        AddLog(strError);
        m_bDBConnected = FALSE;
        MessageBox(strError, _T("데이터베이스 연결 오류"), MB_OK | MB_ICONERROR);
    }
}

// DB 연결 해제
void CInventoryManagerDlg::DisconnectDatabase()
{
    if (m_pDBManager != nullptr && m_bDBConnected)
    {
        m_pDBManager->Disconnect();
        AddLog(_T("🔌 데이터베이스 연결 해제"));
        m_bDBConnected = FALSE;
    }
}

// 연결 테스트
BOOL CInventoryManagerDlg::TestConnection()
{
    if (!m_bDBConnected || m_pDBManager == nullptr)
    {
        AddLog(_T("⚠️ 데이터베이스에 연결되지 않았습니다."));
        return FALSE;
    }

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
        strError.Format(_T("❌ 연결 테스트 실패: %s"), m_pDBManager->GetLastError());
        AddLog(strError);
        return FALSE;
    }
}

// 재고 데이터 로드
void CInventoryManagerDlg::LoadInventoryData()
{
    if (!m_bDBConnected || m_pDBManager == nullptr)
    {
        AddLog(_T("⚠️ 데이터베이스에 연결되지 않았습니다."));
        return;
    }

    AddLog(_T("📊 재고 데이터 로드 중..."));

    BOOL bResult = m_pDBManager->GetInventoryList(m_vecInventory);

    CString strDebug;
    strDebug.Format(_T("🔍 [DEBUG] GetInventoryList 결과: %s"), bResult ? _T("TRUE") : _T("FALSE"));
    AddLog(strDebug);

    strDebug.Format(_T("🔍 [DEBUG] 벡터 크기: %d"), (int)m_vecInventory.size());
    AddLog(strDebug);

    if (bResult)
    {
        CString strLog;
        strLog.Format(_T("✅ 재고 데이터 로드 완료 (%d건)"), (int)m_vecInventory.size());
        AddLog(strLog);

        int nDanger = 0, nWarning = 0, nNormal = 0;
        for (size_t i = 0; i < m_vecInventory.size(); i++)
        {
            if (m_vecInventory[i].nStock == 0)            nDanger++;
            else if (m_vecInventory[i].nStock < m_nDangerThreshold)  nDanger++;
            else if (m_vecInventory[i].nStock < m_nWarningThreshold) nWarning++;
            else nNormal++;
        }

        CString strStats;
        strStats.Format(_T("📈 정상: %d건 / 주의: %d건 / 위험: %d건"), nNormal, nWarning, nDanger);
        AddLog(strStats);

        if (nDanger > 0)
        {
            CString strAlert; strAlert.Format(_T("⚠️ 긴급 발주 필요: %d건!"), nDanger);
            AddLog(strAlert);
        }
    }
    else
    {
        CString strError;
        strError.Format(_T("❌ 재고 데이터 로드 실패: %s"), m_pDBManager->GetLastError());
        AddLog(strError);
    }
}

// 재고 데이터 새로고침
void CInventoryManagerDlg::RefreshInventoryData()
{
    m_vecInventory.clear();
    LoadInventoryData();
    UpdateInventoryList();
    SnapshotDisplayToCache();   // ★ 새로고침 때도 캐시 갱신
}

// 재고 리스트 초기화
void CInventoryManagerDlg::InitInventoryList()
{
    AddLog(_T("🔍 InitInventoryList 시작"));

    if (m_listInventory.GetSafeHwnd() == nullptr)
    {
        AddLog(_T("❌ 리스트 컨트롤 핸들이 없습니다!"));
        return;
    }

    m_listInventory.DeleteAllItems();

    int nColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
    for (int i = nColCount - 1; i >= 0; i--) m_listInventory.DeleteColumn(i);

    DWORD dwStyle = m_listInventory.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_listInventory.SetExtendedStyle(dwStyle);

    m_listInventory.InsertColumn(0, _T("상태"), LVCFMT_CENTER, 60);
    m_listInventory.InsertColumn(1, _T("품번"), LVCFMT_LEFT, 150);
    m_listInventory.InsertColumn(2, _T("상품명"), LVCFMT_LEFT, 200);
    m_listInventory.InsertColumn(3, _T("브랜드"), LVCFMT_LEFT, 100);
    m_listInventory.InsertColumn(4, _T("카테고리"), LVCFMT_LEFT, 100);
    m_listInventory.InsertColumn(5, _T("색상"), LVCFMT_CENTER, 80);
    m_listInventory.InsertColumn(6, _T("사이즈"), LVCFMT_CENTER, 80);
    m_listInventory.InsertColumn(7, _T("재고"), LVCFMT_RIGHT, 80);

    int nFinalColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
    CString strDebug; strDebug.Format(_T("📋 최종 컬럼 개수: %d"), nFinalColCount);
    AddLog(strDebug);

    if (nFinalColCount == 7) AddLog(_T("✅ 재고 리스트 초기화 완료"));
    else                     AddLog(_T("❌ 재고 리스트 초기화 실패 (컬럼 개수 불일치)"));
}

// 재고 리스트 업데이트
void CInventoryManagerDlg::UpdateInventoryList()
{
    AddLog(_T("🔍 UpdateInventoryList 시작"));

    if (m_listInventory.GetSafeHwnd() == nullptr) {
        AddLog(_T("❌ 리스트 컨트롤 핸들이 없습니다!"));
        return;
    }

    int nColCount = m_listInventory.GetHeaderCtrl()->GetItemCount();
    if (nColCount != 8)
    {
        CString strError; strError.Format(_T("❌ 컬럼 개수 오류 (현재: %d, 필요: 7)"), nColCount);
        AddLog(strError);
        AddLog(_T("🔄 리스트 재초기화 시도..."));
        InitInventoryList();
        //return;
    }

    CString strDebug; strDebug.Format(_T("🔍 업데이트할 데이터: %d건"), (int)m_vecInventory.size());
    AddLog(strDebug);
    if (m_vecInventory.empty()) {
        AddLog(_T("⚠️ 표시할 재고 데이터가 없습니다."));
        m_listInventory.DeleteAllItems();
        return;
    }

    m_listInventory.SetRedraw(FALSE);
    m_listInventory.DeleteAllItems();

    int nAddedCount = 0;
    for (size_t i = 0; i < m_vecInventory.size(); i++)
    {
        const INVENTORY_ITEM& item = m_vecInventory[i];

        CString strStatus;
        if (item.nStock == 0)                       strStatus = _T("품절");
        else if (item.nStock < m_nDangerThreshold)  strStatus = _T("위험");
        else if (item.nStock < m_nWarningThreshold) strStatus = _T("주의");
        else                                        strStatus = _T("정상");

        int nIndex = m_listInventory.InsertItem((int)i, strStatus);
        if (nIndex == -1) {
            CString strError; strError.Format(_T("❌ 항목 추가 실패 (인덱스: %d)"), (int)i);
            AddLog(strError);
            continue;
        }

        m_listInventory.SetItemText(nIndex, 1, item.strOptionCode);
        m_listInventory.SetItemText(nIndex, 2, item.strProductName);
        m_listInventory.SetItemText(nIndex, 3, item.strBrandName);
        m_listInventory.SetItemText(nIndex, 4, item.strCategoryName);
        m_listInventory.SetItemText(nIndex, 5, item.strColorName);
        m_listInventory.SetItemText(nIndex, 6, item.strSizeName);

        CString strStock; strStock.Format(_T("%d"), item.nStock);
        m_listInventory.SetItemText(nIndex, 7, strStock);

        m_listInventory.SetItemData(nIndex, (DWORD_PTR)item.nOptionID);

        nAddedCount++;
        if (i < 5) {
            CString strItemLog;
            strItemLog.Format(_T("  ✓ [%d] %s - %s"), nIndex, item.strOptionCode, item.strProductName);
            AddLog(strItemLog);
        }
    }

    m_listInventory.SetRedraw(TRUE);
    m_listInventory.Invalidate(TRUE);
    m_listInventory.UpdateWindow();

    MSG msg;
    while (PeekMessage(&msg, m_listInventory.GetSafeHwnd(), 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CString strLog; strLog.Format(_T("✅ 리스트 업데이트 완료 (%d건)"), nAddedCount);
    AddLog(strLog);
}

void CInventoryManagerDlg::OnBnClickedButtonOrder()
{
    int nSelected = m_listInventory.GetNextItem(-1, LVNI_SELECTED);
    if (nSelected == -1) {
        AfxMessageBox(_T("발주할 품목을 선택해주세요."));
        AddLog(_T("⚠️ 발주: 품목 선택 안 됨"));
        return;
    }

    int nOptionID = (int)m_listInventory.GetItemData(nSelected);

    INVENTORY_ITEM* pItem = nullptr;
    for (size_t i = 0; i < m_vecInventory.size(); i++) {
        if (m_vecInventory[i].nOptionID == nOptionID) { pItem = &m_vecInventory[i]; break; }
    }
    if (!pItem) {
        AfxMessageBox(_T("품목 정보를 찾을 수 없습니다."));
        AddLog(_T("❌ 발주: 품목 정보 없음"));
        return;
    }

    CString strDebugLog;
    strDebugLog.Format(_T("선택된 품목 - 품번: %s, 상품명: %s, 재고: %d"),
        pItem->strOptionCode, pItem->strProductName, pItem->nStock);
    AddLog(strDebugLog);

    COrderDlg dlg;
    dlg.m_nOptionID = pItem->nOptionID;
    dlg.m_strOptionCode = pItem->strOptionCode;
    dlg.m_strProductName = pItem->strProductName;
    dlg.m_nCurrentStock = pItem->nStock;

    if (dlg.DoModal() == IDOK)
    {
        CString strLog;
        strLog.Format(_T("📦 발주 시도: %s (수량: %d)"), pItem->strOptionCode, dlg.m_nOrderQuantity);
        AddLog(strLog);

        if (!m_bDBConnected || m_pDBManager == nullptr) {
            AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
            AddLog(_T("❌ 발주 실패: DB 연결 없음"));
            return;
        }

        BOOL bResult = m_pDBManager->AddStock(nOptionID, dlg.m_nOrderQuantity);
        if (bResult)
        {
            strLog.Format(_T("✅ 발주 성공: %s (%d개, %d → %d)"),
                pItem->strOptionCode, dlg.m_nOrderQuantity, pItem->nStock, pItem->nStock + dlg.m_nOrderQuantity);
            AddLog(strLog);
            AfxMessageBox(_T("발주가 완료되었습니다."));
            RefreshInventoryData();
        }
        else
        {
            CString strError; strError.Format(_T("❌ 발주 실패: %s"), m_pDBManager->GetLastError());
            AddLog(strError);
            AfxMessageBox(strError);
        }
    }
    else {
        AddLog(_T("🚫 발주 취소됨"));
    }
}

// 창 닫기
void CInventoryManagerDlg::OnClose()
{
    AddLog(_T("🚪 프로그램 종료 요청"));

    CWnd* pWnd = GetWindow(GW_ENABLEDPOPUP);
    if (pWnd != nullptr && pWnd != this)
    {
        AddLog(_T("⚠️ 열린 대화상자를 먼저 닫습니다..."));
        pWnd->SendMessage(WM_CLOSE);

        MSG msg; int nCount = 0;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) && nCount < 100) {
            TranslateMessage(&msg); DispatchMessage(&msg); nCount++;
        }
        AddLog(_T("✅ 대화상자 닫기 완료"));
    }

    DisconnectDatabase();
    AddLog(_T("👋 프로그램 종료 중..."));
    CDialogEx::OnClose();
}

void CInventoryManagerDlg::OnDblclkListInventory(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nItem = pNMItemActivate->iItem;

    if (nItem == -1) { AddLog(_T("⚠️ 재고 수정: 항목이 선택되지 않음")); *pResult = 0; return; }

    int nOptionID = (int)m_listInventory.GetItemData(nItem);

    INVENTORY_ITEM* pItem = nullptr;
    for (size_t i = 0; i < m_vecInventory.size(); i++) {
        if (m_vecInventory[i].nOptionID == nOptionID) { pItem = &m_vecInventory[i]; break; }
    }
    if (!pItem) {
        AfxMessageBox(_T("품목 정보를 찾을 수 없습니다."));
        AddLog(_T("❌ 재고 수정: 품목 정보 없음"));
        *pResult = 0; return;
    }

    CString strDebugLog;
    strDebugLog.Format(_T("📦 선택된 품목 - 품번: %s, 상품명: %s, 재고: %d"),
        pItem->strOptionCode, pItem->strProductName, pItem->nStock);
    AddLog(strDebugLog);

    CEditStockDlg dlg;
    dlg.m_nOptionID = pItem->nOptionID;
    dlg.m_strOptionCode = pItem->strOptionCode;
    dlg.m_strProductName = pItem->strProductName;
    dlg.m_nCurrentStock = pItem->nStock;

    if (dlg.DoModal() == IDOK)
    {
        CString strLog;
        strLog.Format(_T("✏️ 재고 수정 시도: %s (%d → %d)"),
            pItem->strOptionCode, pItem->nStock, dlg.m_nNewStock);
        AddLog(strLog);

        if (!m_bDBConnected || m_pDBManager == nullptr) {
            AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
            AddLog(_T("❌ 재고 수정 실패: DB 연결 없음"));
            *pResult = 0; return;
        }

        BOOL bResult = m_pDBManager->UpdateStock(nOptionID, dlg.m_nNewStock);
        if (bResult)
        {
            strLog.Format(_T("✅ 재고 수정 성공: %s (%d → %d, 변경: %+d)"),
                pItem->strOptionCode, pItem->nStock, dlg.m_nNewStock, dlg.m_nNewStock - pItem->nStock);
            AddLog(strLog);
            AfxMessageBox(_T("재고가 수정되었습니다."));

            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }

            RefreshInventoryData();

            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
        }
        else
        {
            CString strError; strError.Format(_T("❌ 재고 수정 실패: %s"), m_pDBManager->GetLastError());
            AddLog(strError);
            AfxMessageBox(strError);
        }
    }
    else {
        AddLog(_T("🚫 재고 수정 취소됨"));
    }

    *pResult = 0;
}

void CInventoryManagerDlg::OnBnClickedButton2()
{
    int nSelectedItem = m_listInventory.GetNextItem(-1, LVNI_SELECTED);
    if (nSelectedItem == -1) {
        AfxMessageBox(_T("삭제할 품목을 먼저 선택해주세요."));
        AddLog(_T("⚠️ 삭제: 품목이 선택되지 않음"));
        return;
    }

    int nOptionID = (int)m_listInventory.GetItemData(nSelectedItem);
    CString strProductCode = m_listInventory.GetItemText(nSelectedItem, 1);

    CString strConfirmMsg;
    strConfirmMsg.Format(_T("품번 '%s'을(를) 정말로 삭제하시겠습니까?\n이 작업은 되돌릴 수 없습니다."), strProductCode);
    if (AfxMessageBox(strConfirmMsg, MB_YESNO | MB_ICONWARNING) != IDYES) {
        AddLog(_T("🚫 삭제가 사용자에 의해 취소됨"));
        return;
    }

    if (!m_bDBConnected || m_pDBManager == nullptr) {
        AfxMessageBox(_T("데이터베이스에 연결되지 않았습니다."));
        AddLog(_T("❌ 삭제 실패: DB 연결 없음"));
        return;
    }

    CString strLog; strLog.Format(_T("🗑️ 삭제 시도: %s (OptionID: %d)"), strProductCode, nOptionID);
    AddLog(strLog);

    BOOL bResult = m_pDBManager->DeleteInventoryItem(nOptionID);
    if (bResult) {
        AfxMessageBox(_T("선택한 품목이 성공적으로 삭제되었습니다."));
        AddLog(_T("✅ 삭제 성공!"));
        RefreshInventoryData();
    }
    else {
        CString strError; strError.Format(_T("❌ 삭제 실패: %s"), m_pDBManager->GetLastError());
        AddLog(strError);
        AfxMessageBox(strError);
    }
}

void CInventoryManagerDlg::OnBnClickedButton3()
{
    CAddProductDlg dlg;
    if (dlg.DoModal() == IDOK) {
        AddLog(_T("✨ 새 상품이 추가되었습니다. 목록을 새로고침합니다."));
        RefreshInventoryData();
    }
}

// =========================
// 🔎 검색 버튼/엔터 처리
// =========================
void CInventoryManagerDlg::OnBnClickedBtnSearch()
{
    //CString q; m_editSearch.GetWindowText(q);
    //ApplySearchFilter(q);  // 캐시 기반 검색
    ApplyFiltersAndSearch();
}

BOOL CInventoryManagerDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        if (GetFocus() && GetFocus()->GetDlgCtrlID() == IDC_EDIT_SEARCH)
        {
            ApplyFiltersAndSearch();
            return TRUE;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CInventoryManagerDlg::OnSelchangeComboFilter()
{
    ApplyFiltersAndSearch(); // 핵심 함수 호출
}

// =========================
// 🔎 검색 캐시 루틴
// =========================
void CInventoryManagerDlg::SnapshotDisplayToCache()
{
    m_allRowsDisplay.clear();
    const int rowCount = m_listInventory.GetItemCount();

    m_allRowsDisplay.reserve(rowCount);
    for (int i = 0; i < rowCount; ++i)
    {
        DisplayRow r;
        r.col0 = m_listInventory.GetItemText(i, 0); // 상태
        r.col1 = m_listInventory.GetItemText(i, 1); // 품번
        r.col2 = m_listInventory.GetItemText(i, 2); // 상품명
        r.col3 = m_listInventory.GetItemText(i, 3); // 브랜드
        r.col4 = m_listInventory.GetItemText(i, 4); // 카테고리
        r.col5 = m_listInventory.GetItemText(i, 5); // 색상
        r.col6 = m_listInventory.GetItemText(i, 6); // 사이즈
        r.col7 = m_listInventory.GetItemText(i, 7); // 재고
        m_allRowsDisplay.push_back(r);
    }
}

void CInventoryManagerDlg::ShowRowsFromCache(const std::vector<DisplayRow>& rows)
{
    m_listInventory.SetRedraw(FALSE);
    m_listInventory.DeleteAllItems();

    for (const auto& r : rows)
    {
        int i = m_listInventory.InsertItem(m_listInventory.GetItemCount(), r.col0);
        m_listInventory.SetItemText(i, 1, r.col1);
        m_listInventory.SetItemText(i, 2, r.col2);
        m_listInventory.SetItemText(i, 3, r.col3);
        m_listInventory.SetItemText(i, 4, r.col4);
        m_listInventory.SetItemText(i, 5, r.col5);
        m_listInventory.SetItemText(i, 6, r.col6);
        m_listInventory.SetItemText(i, 7, r.col7);
    }

    m_listInventory.SetRedraw(TRUE);
    m_listInventory.Invalidate();
}

//void CInventoryManagerDlg::ApplySearchFilter(const CString& keywordRaw)
//{
//    CString q = keywordRaw; q.Trim();
//    if (q.IsEmpty()) {
//        ShowRowsFromCache(m_allRowsDisplay);
//        return;
//    }
//
//    const CString ql = ToLower(q);
//    std::vector<DisplayRow> filtered;
//    filtered.reserve(m_allRowsDisplay.size());
//
//    for (const auto& r : m_allRowsDisplay)
//    {
//        // 대상: 상태(0), 품번(1), 상품명(2), 브랜드(3), 색상(4), 사이즈(5)
//        const CString status = ToLower(r.col0);
//        const CString sku = ToLower(r.col1);
//        const CString name = ToLower(r.col2);
//        const CString brand = ToLower(r.col3);
//        const CString color = ToLower(r.col4);
//        const CString size = ToLower(r.col5);
//
//        bool match =
//            (sku.Find(ql) >= 0) ||
//            (name.Find(ql) >= 0) ||
//            (brand.Find(ql) >= 0) ||
//            (status.Find(ql) >= 0) ||
//            (color.Find(ql) >= 0) ||
//            (size.Find(ql) >= 0);
//
//        if (match) filtered.push_back(r);
//    }
//
//    ShowRowsFromCache(filtered);
//}

// [ADD] 탭별 UI 토글 구현
void CInventoryManagerDlg::ShowTabPage(int idx)
{
    const bool showInventory = (idx == 0);
    const bool showStats = (idx == 1);
    // const bool showSettings  = (idx == 2); // 필요 시 사용

    // 재고 리스트 보이기/숨기기 (필요한 컨트롤들만 토글해도 됨)
    m_listInventory.ShowWindow(showInventory ? SW_SHOW : SW_HIDE);

    // 통계 다이얼로그 보이기/숨기기
    if (m_pStatsDlg) {
        if (showStats) {
            // 플레이스홀더 위치에 맞춰 배치
            CRect rc;
            GetDlgItem(IDC_PLACE_STATS)->GetWindowRect(&rc);
            ScreenToClient(&rc);
            m_pStatsDlg->SetWindowPos(nullptr, rc.left, rc.top, rc.Width(), rc.Height(),
                SWP_NOZORDER | SWP_NOACTIVATE);

            // DB 상태 주입 + 데이터 로드
            m_pStatsDlg->InitDB(m_pDBManager, m_bDBConnected);
            // m_pStatsDlg->Reload();  // Reload가 있으면 이 한 줄로 대체

            m_pStatsDlg->Reload();
            m_pStatsDlg->ShowWindow(SW_SHOW);
        }
        else {
            m_pStatsDlg->ShowWindow(SW_HIDE);
        }
    }
}

void CInventoryManagerDlg::ApplyFiltersAndSearch()
{
    // 1. 현재 선택된 필터 값 가져오기
    CString strBrandFilter;
    int nBrandIndex = m_comboFilterBrand.GetCurSel();
    if (nBrandIndex > 0) // "전체 브랜드"가 아닐 경우
    {
        m_comboFilterBrand.GetLBText(nBrandIndex, strBrandFilter);
    }

    // (카테고리 필터는 향후 확장을 위해 변수만 선언)
    CString strCategoryFilter;
    int nCategoryIndex = m_comboFilterCategory.GetCurSel();
    if (nCategoryIndex > 0)
    {
        m_comboFilterCategory.GetLBText(nCategoryIndex, strCategoryFilter);
    }

    // 2. 현재 검색어 가져오기
    CString strSearchKeyword;
    m_editSearch.GetWindowText(strSearchKeyword);
    strSearchKeyword.Trim();
    const CString lowerKeyword = ToLower(strSearchKeyword);

    // 3. 필터링된 결과를 담을 새 목록 준비
    std::vector<DisplayRow> filteredRows;

    // 4. 원본 캐시 데이터를 하나씩 검사하며 필터링 진행
    for (const auto& row : m_allRowsDisplay)
    {
        // [조건 1] 브랜드 필터
        CString brandFromRow = row.col3;
        bool bBrandMatch = strBrandFilter.IsEmpty() || (brandFromRow.Trim() == strBrandFilter);
        if (!bBrandMatch)
        {
            continue;
        }

        // [조건 2] 카테고리 필터 (현재는 비활성화)
        CString categoryFromRow = row.col4;
        bool bCategoryMatch = strCategoryFilter.IsEmpty() || (categoryFromRow.Trim() == strCategoryFilter);
        if (!bCategoryMatch)
        {
            continue;
        }

        // [조건 3] 검색어 필터 (검색어가 있을 경우에만)
        if (!lowerKeyword.IsEmpty())
        {
            bool bKeywordMatch =
                (ToLower(row.col0).Find(lowerKeyword) >= 0) || // 상태
                (ToLower(row.col1).Find(lowerKeyword) >= 0) || // 품번
                (ToLower(row.col2).Find(lowerKeyword) >= 0) || // 상품명
                (ToLower(row.col3).Find(lowerKeyword) >= 0) || // 브랜드
                (ToLower(row.col4).Find(lowerKeyword) >= 0) || // 색상
                (ToLower(row.col5).Find(lowerKeyword) >= 0);   // 사이즈

            if (!bKeywordMatch)
            {
                continue;
            }
        }

        // 모든 조건을 통과한 데이터만 결과 목록에 추가
        filteredRows.push_back(row);
    }

    // 5. 최종 결과를 화면에 표시
    ShowRowsFromCache(filteredRows);
}