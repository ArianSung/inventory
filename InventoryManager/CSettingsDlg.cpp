// CSettingsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"
#include "InventoryManagerDlg.h" // 메인 다이얼로그 헤더를 포함해야 합니다.
#include "resource.h"


// CSettingsDlg 대화 상자

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SETTINGS_DIALOG, pParent)
    , m_nWarningThreshold(0)  // 멤버 변수 초기화
    , m_nDangerThreshold(0)   // 멤버 변수 초기화
{
}

CSettingsDlg::~CSettingsDlg()
{
}

// Edit Control과 멤버 변수를 연결합니다.
void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_WARNING, m_nWarningThreshold);
    DDX_Text(pDX, IDC_EDIT_DANGER, m_nDangerThreshold);
}

// '적용' 버튼과 함수를 연결합니다.
BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSettingsDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CSettingsDlg 메시지 처리기

// 다이얼로그가 처음 생성될 때 호출됩니다.
BOOL CSettingsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 부모 윈도우(CInventoryManagerDlg)의 주소를 저장해 둡니다.
    m_pParentDlg = (CInventoryManagerDlg*)GetParent();

    return TRUE;
}

// 외부에서 현재 기준값을 받아와 Edit Control에 설정하는 함수입니다.
void CSettingsDlg::SetInitialValues(int nWarning, int nDanger)
{
    m_nWarningThreshold = nWarning;
    m_nDangerThreshold = nDanger;

    // 다이얼로그 창이 실제로 만들어진 상태에서만 화면을 업데이트합니다.
    if (GetSafeHwnd()) {
        UpdateData(FALSE); // 멤버 변수의 값을 컨트롤(화면)에 표시
    }
}

// '적용' 버튼을 클릭했을 때 실행될 함수입니다.
void CSettingsDlg::OnBnClickedButtonApply()
{
    UpdateData(TRUE); // 컨트롤(화면)에 입력된 값을 멤버 변수로 가져옴

    // 입력값 유효성 검사
    if (m_nDangerThreshold >= m_nWarningThreshold) {
        AfxMessageBox(_T("위험 기준은 주의 기준보다 작은 값이어야 합니다."));
        return;
    }

    // 부모 다이얼로그 포인터가 유효한지 확인
    if (m_pParentDlg) {
        // 메인 다이얼로그의 멤버 변수 값을 직접 변경합니다.
        m_pParentDlg->m_nWarningThreshold = m_nWarningThreshold;
        m_pParentDlg->m_nDangerThreshold = m_nDangerThreshold;

        // 변경된 기준을 즉시 재고 리스트에 반영하도록 메인 다이얼로그에 요청합니다.
        m_pParentDlg->UpdateInventoryList();

        m_pParentDlg->AddLog(_T("⚙️ 재고 기준이 변경되었습니다."));
        AfxMessageBox(_T("설정이 적용되었습니다."));
    }
}