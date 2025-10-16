// CSettingsDlg.cpp : 구현 파일
//

#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"
#include "InventoryManagerDlg.h" // ✅ [추가] 부모 다이얼로그의 함수를 호출하기 위해 include

// CSettingsDlg 대화 상자

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_nWarningThreshold(0)
	, m_nDangerThreshold(0)
	, m_pParentDlg(nullptr)
	// ✅ [추가] 멤버 변수 초기화
	, m_strDbHost(_T(""))
	, m_nDbPort(3306)
	, m_strDbName(_T(""))
	, m_strDbUser(_T(""))
	, m_strDbPass(_T(""))
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

	// ✅ [추가] 새로 추가할 컨트롤 ID와 변수를 연결합니다.
	DDX_Text(pDX, IDC_EDIT_DB_HOST, m_strDbHost);
	DDX_Text(pDX, IDC_EDIT_DB_PORT, m_nDbPort);
	DDX_Text(pDX, IDC_EDIT_DB_NAME, m_strDbName);
	DDX_Text(pDX, IDC_EDIT_DB_USER, m_strDbUser);
	DDX_Text(pDX, IDC_EDIT_DB_PASS, m_strDbPass);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSettingsDlg::OnBnClickedButtonApply)
	// ✅ [추가] 저장 버튼 ID와 핸들러 함수를 연결합니다.
	ON_BN_CLICKED(IDC_BTN_SAVE_DB, &CSettingsDlg::OnBnClickedButtonSaveDb)
END_MESSAGE_MAP()


// CSettingsDlg 메시지 처리기

void CSettingsDlg::SetParentDlg(CInventoryManagerDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CSettingsDlg::OnBnClickedButtonApply()
{
	UpdateData(TRUE); // 컨트롤의 값을 변수로 가져옴

	if (m_nWarningThreshold <= m_nDangerThreshold)
	{
		AfxMessageBox(_T("경고: '주의' 기준은 '위험' 기준보다 커야 합니다."));
		return;
	}

	if (m_pParentDlg)
	{
		m_pParentDlg->UpdateThresholds(m_nWarningThreshold, m_nDangerThreshold);
	}
}

void CSettingsDlg::LoadSettings(int nWarning, int nDanger)
{
	m_nWarningThreshold = nWarning;
	m_nDangerThreshold = nDanger;
	UpdateData(FALSE); // 변수의 값을 컨트롤에 표시
}

// ✅ [추가] 메인 다이얼로그로부터 DB 설정값을 받아와 에디트 컨트롤에 채워주는 함수
void CSettingsDlg::LoadDbSettings(const DB_CONFIG& dbConfig)
{
	m_strDbHost = dbConfig.strHost;
	m_nDbPort = dbConfig.nPort;
	m_strDbName = dbConfig.strDatabase;
	m_strDbUser = dbConfig.strUser;
	m_strDbPass = dbConfig.strPassword;

	UpdateData(FALSE); // 변수의 값을 컨트롤에 표시
}

// ✅ [추가] 'DB 설정 저장' 버튼을 눌렀을 때 호출될 함수
void CSettingsDlg::OnBnClickedButtonSaveDb()
{
	if (m_pParentDlg == nullptr)
	{
		AfxMessageBox(_T("오류: 부모 다이얼로그가 설정되지 않았습니다."));
		return;
	}

	UpdateData(TRUE); // 컨트롤의 값을 변수에 저장

	// 현재 UI에 입력된 값으로 새로운 DB_CONFIG 구조체를 만듭니다.
	DB_CONFIG newConfig;
	newConfig.strHost = m_strDbHost;
	newConfig.nPort = m_nDbPort;
	newConfig.strDatabase = m_strDbName;
	newConfig.strUser = m_strDbUser;
	newConfig.strPassword = m_strDbPass;

	// 부모(메인) 다이얼로그에 새로운 설정값으로 재연결을 요청합니다.
	m_pParentDlg->UpdateDbConfigAndReconnect(newConfig);
}