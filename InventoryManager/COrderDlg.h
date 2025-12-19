#pragma once

class COrderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderDlg)

public:
	COrderDlg(CWnd* pParent = nullptr);
	virtual ~COrderDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	int m_nOptionID;
	CString m_strOptionCode;
	CString m_strProductName;
	int m_nCurrentStock;
	int m_nWarningThreshold;
	int m_nDangerThreshold;

	int m_nOrderQuantity;

	void UpdateExpectedStock();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonMinus10();
	afx_msg void OnBnClickedButtonMinus5();
	afx_msg void OnBnClickedButtonPlus5();
	afx_msg void OnBnClickedButtonPlus10();
	afx_msg void OnEnChangeEditOrderQuantity();
	afx_msg void OnBnClickedOk();
};
