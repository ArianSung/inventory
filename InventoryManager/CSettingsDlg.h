#pragma once
#include "afxdialogex.h"

class CInventoryManagerDlg; // 메인 다이얼로그의 존재를 알려주는 '전방 선언'

// CSettingsDlg 대화 상자

class CSettingsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSettingsDlg)

public:
    CSettingsDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
    virtual ~CSettingsDlg();

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SETTINGS_DIALOG };
#endif

public:
    // UI 컨트롤과 연결할 멤버 변수
    int m_nWarningThreshold;
    int m_nDangerThreshold;

    // 부모(메인) 다이얼로그의 주소를 저장할 변수
    CInventoryManagerDlg* m_pParentDlg;

    // 메인 창에서 현재 설정값을 받아와 UI에 표시하는 함수
    void SetInitialValues(int nWarning, int nDanger);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(); // OnInitDialog 함수 선언 추가

    DECLARE_MESSAGE_MAP()

public:
    // '적용' 버튼 클릭 이벤트 처리 함수
    afx_msg void OnBnClickedButtonApply();
};