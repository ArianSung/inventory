// CSettingsDlg.cpp: 구현 파일

// 변경 후 (전체 코드)
#include "pch.h"
#include "InventoryManager.h"
#include "CSettingsDlg.h"
#include "InventoryManagerDlg.h" // 부모 다이얼로그의 기능을 사용하기 위해 포함
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_pParentDlg(nullptr) // 멤버 변수 초기화
{
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 컨트롤 ID와 멤버 변수를 연결합니다.
	DDX_Control(pDX, IDC_EDIT_WARNING, m_editWarning);
	DDX_Control(pDX, IDC_EDIT_DANGER, m_editDanger);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	// '적용' 버튼(IDC_BUTTON_APPLY)을 클릭 시 OnBnClickedButtonApply 함수가 호출되도록 연결
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSettingsDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

// InventoryManagerDlg에서 호출하여 부모-자식 관계를 설정하는 함수
void CSettingsDlg::SetParentDlg(CInventoryManagerDlg* pParent)
{
	m_pParentDlg = pParent;
}

// 탭을 선택할 때마다 InventoryManagerDlg에서 호출하여 현재 값을 표시하는 함수
void CSettingsDlg::LoadSettings(int nWarning, int nDanger)
{
	CString strWarning, strDanger;
	strWarning.Format(_T("%d"), nWarning);
	strDanger.Format(_T("%d"), nDanger);

	m_editWarning.SetWindowText(strWarning);
	m_editDanger.SetWindowText(strDanger);
}

// '적용' 버튼을 클릭했을 때 호출되는 함수
void CSettingsDlg::OnBnClickedButtonApply()
{
	// 부모 다이얼로그 포인터가 유효한지 확인
	if (!m_pParentDlg) {
		AfxMessageBox(_T("오류: 메인 다이얼로그와 연결되지 않았습니다."), MB_ICONERROR);
		return;
	}

	CString strWarning, strDanger;
	m_editWarning.GetWindowText(strWarning);
	m_editDanger.GetWindowText(strDanger);

	int nWarning = _ttoi(strWarning);
	int nDanger = _ttoi(strDanger);

	// 입력 값 유효성 검사
	if (nWarning <= nDanger) {
		AfxMessageBox(_T("'주의' 기준값은 '위험' 기준값보다 커야 합니다."), MB_ICONWARNING);
		return;
	}
	if (nWarning <= 0 || nDanger < 0) {
		AfxMessageBox(_T("'주의' 기준값은 0보다 커야하며, '위험' 기준값은 0 이상이어야 합니다."), MB_ICONWARNING);
		return;
	}

	// 검증이 완료되면, 부모 다이얼로그의 public 함수를 호출하여 값을 전달
	m_pParentDlg->UpdateThresholds(nWarning, nDanger);
	AfxMessageBox(_T("설정이 적용되었습니다."), MB_ICONINFORMATION);
}