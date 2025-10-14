#pragma once
#include "afxdialogex.h"
#include "resource.h"

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
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	// 사용자가 입력할 값을 저장할 멤버 변수
	int m_nDangerValue;
	int m_nWarningValue;
};