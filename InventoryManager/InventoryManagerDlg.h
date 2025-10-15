// InventoryManagerDlg.h: 헤더 파일
//

#pragma once

#include <vector>
#include "DBManager.h"
#include "CEditStockDlg.h"

class CStatsDlg;

// ===== 검색 캐시용 행 구조 =====
struct DisplayRow {
    CString col0, col1, col2, col3, col4, col5, col6;
};

// CInventoryManagerDlg 대화 상자
class CInventoryManagerDlg : public CDialogEx
{
    // 생성입니다.
public:
    CInventoryManagerDlg(CWnd* pParent = nullptr); // 표준 생성자입니다.
    virtual ~CInventoryManagerDlg();               // 소멸자입니다.
    CStatsDlg* m_pStatsDlg = nullptr;
    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_INVENTORYMANAGER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    HICON m_hIcon;

    // 생성된 메시지 맵 함수
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    DECLARE_MESSAGE_MAP()

public:
    // =========================
    // UI 컨트롤 변수
    // =========================
    CTabCtrl   m_tabMain;
    CEdit      m_editLog;
    CListCtrl  m_listInventory;

    // === 검색 UI ===
    CEdit      m_editSearch;   // IDC_EDIT_SEARCH
    CButton    m_btnSearch;    // IDC_BTN_SEARCH

public:
    // =========================
    // 멤버 변수
    // =========================
    int       m_nCurrentTab;        // 현재 선택된 탭 인덱스
    UINT_PTR  m_nTimerID;           // 타이머 ID
    int       m_nRefreshInterval;   // 자동 새로고침 간격 (초)
    BOOL      m_bAutoRefresh;       // 자동 새로고침 활성화 여부

    // =========================
    // 데이터베이스 관련 변수
    // =========================
    CDBManager* m_pDBManager;    // DB 관리자 (싱글톤 인스턴스)
    BOOL        m_bDBConnected;  // DB 연결 상태

    // =========================
    // 멤버 함수
    // =========================
    void AddLog(CString strLog);     // 로그 추가
    void ClearLog();                 // 로그 지우기
    void InitTabs();                 // 탭 초기화

    // =========================
    // 리스트 뷰 관련 함수
    // =========================
    void InitInventoryList();        // 리스트 초기화
    void UpdateInventoryList();      // 리스트 업데이트

    // =========================
    // 재고 데이터
    // =========================
    std::vector<INVENTORY_ITEM> m_vecInventory;  // 재고 목록

    afx_msg void OnBnClickedButtonClearLog();
    afx_msg void OnBnClickedButtonRefresh();
    afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);

    // =========================
    // 재고 관련 함수
    // =========================
    void LoadInventoryData();        // 재고 데이터 로드
    void RefreshInventoryData();     // 재고 데이터 새로고침

    // =========================
    // DB 관련 함수
    // =========================
    void ConnectDatabase();          // DB 연결
    void DisconnectDatabase();       // DB 연결 해제
    BOOL TestConnection();           // 연결 테스트

    int m_nDangerThreshold;  // 위험 재고 기준
    int m_nWarningThreshold; // 주의 재고 기준

    afx_msg void OnBnClickedButtonOrder();
    afx_msg void OnDblclkListInventory(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();

    // =========================
    // 검색 캐시/핸들러
    // =========================
    std::vector<DisplayRow> m_allRowsDisplay;   // 리스트 스냅샷 캐시

    void SnapshotDisplayToCache();
    void ShowRowsFromCache(const std::vector<DisplayRow>& rows);
    void ApplySearchFilter(const CString& keywordRaw);
    afx_msg void OnBnClickedBtnSearch();

protected:
    // (선택) 엔터키로 검색
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;
};
