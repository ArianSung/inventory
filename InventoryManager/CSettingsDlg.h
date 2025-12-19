// CSettingsDlg.h : 헤더 파일
//

#pragma once
#include "afxdialogex.h"
#include "DBManager.h"

// CInventoryManagerDlg 전방 선언
class CInventoryManagerDlg;

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

	DECLARE_MESSAGE_MAP()
private:
	int m_nWarningThreshold;
	int m_nDangerThreshold;
	CInventoryManagerDlg* m_pParentDlg;
public:
	void SetParentDlg(CInventoryManagerDlg* pDlg);
	afx_msg void OnBnClickedButtonApply();
	void LoadSettings(int nWarning, int nDanger);

	CString m_strDbHost;
	UINT    m_nDbPort;
	CString m_strDbName;
	CString m_strDbUser;
	CString m_strDbPass;

	void LoadDbSettings(const DB_CONFIG& dbConfig);
	afx_msg void OnBnClickedButtonSaveDb();
	BOOL m_bAutoOrderEnable;
	int m_nAutoOrderThreshold;
	int m_nAutoOrderQuantity;
	int m_nAutoOrderInterval;
	afx_msg void OnBnClickedButtonAutoOrderApply();

	void SaveAutoOrderConfig();       // config.ini에 자동발주 설정 저장
	void LoadAutoOrderConfig();       // config.ini에서 자동발주 설정 로드
};