#pragma once
#include "afxdialogex.h"

// CInventoryManagerDlg 클래스를 미리 알려줌 (순환 참조 방지)
class CInventoryManagerDlg;

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

	DECLARE_MESSAGE_MAP()

private:
	// 컨트롤 핸들링을 위한 멤버 변수
	CEdit m_editWarning;
	CEdit m_editDanger;

	// 메인 다이얼로그와 안전하게 통신하기 위한 포인터
	CInventoryManagerDlg* m_pParentDlg;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();

	// 메인 다이얼로그 포인터를 설정하는 함수
	void SetParentDlg(CInventoryManagerDlg* pParent);
	// 메인 다이얼로그로부터 현재 설정 값을 불러오는 함수
	void LoadSettings(int nWarning, int nDanger);
};