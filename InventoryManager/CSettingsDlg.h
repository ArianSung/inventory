#pragma once
#include "afxdialogex.h"

class CInventoryManagerDlg;

class CSettingsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSettingsDlg)

public:
    CSettingsDlg(CWnd* pParent = nullptr);
    virtual ~CSettingsDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SETTINGS_DIALOG };
#endif

public:
    int m_nWarningThreshold;
    int m_nDangerThreshold;
    void SetInitialValues(int nWarning, int nDanger);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonApply();
};