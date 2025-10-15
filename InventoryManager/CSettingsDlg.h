#pragma once
#include "afxdialogex.h"
#include "resource.h" // 리소스 ID를 사용하기 위해 추가

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnOK(); // '확인' 버튼 클릭 시 유효성 검사를 위해 추가

	DECLARE_MESSAGE_MAP()

public:
	// 사용자가 입력할 값을 저장할 멤버 변수
	int m_nDangerValue;
	int m_nWarningValue;
};