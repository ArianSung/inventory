#pragma once
#include "afxdialogex.h"
#include "resource.h"

// CSettingsDlg ��ȭ ����

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = nullptr);   // ǥ�� �������Դϴ�.
	virtual ~CSettingsDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	// ����ڰ� �Է��� ���� ������ ��� ����
	int m_nDangerValue;
	int m_nWarningValue;
};