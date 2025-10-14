#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"
#include "resource.h"


// CSettingsDlg 대화 상자

// ✅ 이 매크로가 GetRuntimeClass 함수를 만듭니다.
IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
{
	// .h 파일에 선언된 변수들을 여기서 초기화합니다.
	m_nDangerValue = 0;
	m_nWarningValue = 0;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DANGER, m_nDangerValue);
	DDX_Text(pDX, IDC_EDIT_WARNING, m_nWarningValue);
}

// ✅ 이 매크로 블록이 GetMessageMap 함수를 만듭니다.
BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
END_MESSAGE_MAP()

void CSettingsDlg::OnOK()
{
	// 1. Edit Control의 텍스트를 멤버 변수(m_nDangerValue)로 가져옵니다.
	UpdateData(TRUE);

	// 2. 유효성 검사: 위험 재고 기준이 주의 재고 기준보다 크거나 같은지 확인
	if (m_nDangerValue >= m_nWarningValue)
	{
		// 3. 조건에 맞지 않으면 경고 메시지를 보여주고 함수를 종료합니다.
		//    (CDialogEx::OnOK()가 호출되지 않아 창이 닫히지 않습니다.)
		AfxMessageBox(_T("위험 재고 기준은 주의 재고 기준보다 작아야 합니다."));
		return;
	}

	// 4. 모든 조건이 정상이면, 부모 클래스의 OnOK를 호출하여 대화상자를 닫습니다.
	CDialogEx::OnOK();
}

