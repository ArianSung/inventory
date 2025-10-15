// CSettingsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"


// CSettingsDlg 대화 상자

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
{
	// 변수 초기화
	m_nDangerValue = 0;
	m_nWarningValue = 0;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// Edit Control의 ID와 멤버 변수를 연결합니다.
	DDX_Text(pDX, IDC_EDIT_DANGER, m_nDangerValue);
	DDX_Text(pDX, IDC_EDIT_WARNING, m_nWarningValue);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingsDlg 메시지 처리기

void CSettingsDlg::OnOK()
{
	// 1. 사용자가 입력한 값을 변수로 가져옵니다.
	UpdateData(TRUE);

	//    '위험' 기준이 '주의' 기준보다 크거나 같은지 검사합니다.
	if (m_nDangerValue >= m_nWarningValue)
	{
		// 3. 조건에 맞지 않으면, 경고 메시지를 띄우고
		//    창이 닫히지 않도록 return 합니다.
		AfxMessageBox(_T("위험 재고 기준은 주의 재고 기준보다 작아야 합니다."));
		return;
	}

	// 4. 모든 조건이 정상이면, 창을 닫습니다.
	CDialogEx::OnOK();
}