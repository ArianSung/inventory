#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"
#include "InventoryManagerDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SETTINGS_DIALOG, pParent)
    , m_nWarningThreshold(0)
    , m_nDangerThreshold(0)
{
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_WARNING, m_nWarningThreshold);
    DDX_Text(pDX, IDC_EDIT_DANGER, m_nDangerThreshold);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSettingsDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()

BOOL CSettingsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    return TRUE;
}

void CSettingsDlg::SetInitialValues(int nWarning, int nDanger)
{
    m_nWarningThreshold = nWarning;
    m_nDangerThreshold = nDanger;
    if (GetSafeHnd()) {
        UpdateData(FALSE);
    }
}

void CSettingsDlg::OnBnClickedButtonApply()
{
    UpdateData(TRUE);

    if (m_nDangerThreshold >= m_nWarningThreshold) {
        AfxMessageBox(_T("위험 기준은 주의 기준보다 작은 값이어야 합니다."));
        return;
    }

    CInventoryManagerDlg* pParentDlg = (CInventoryManagerDlg*)GetParent();
    if (pParentDlg) {
        pParentDlg->m_nWarningThreshold = m_nWarningThreshold;
        pParentDlg->m_nDangerThreshold = m_nDangerThreshold;
        pParentDlg->UpdateInventoryList();
        pParentDlg->AddLog(_T("⚙️ 재고 기준이 변경되었습니다."));
        AfxMessageBox(_T("설정이 적용되었습니다."));
    }
}