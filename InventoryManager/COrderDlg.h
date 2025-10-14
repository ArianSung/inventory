#pragma once

// COrderDlg ��ȭ ����

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
	// ========================================
	// �Է� ������ (�ܺο��� ����)
	// ========================================
	int m_nOptionID;             // �ɼ� ID
	CString m_strOptionCode;     // ǰ��
	CString m_strProductName;    // ��ǰ��
	int m_nCurrentStock;         // ���� ���

	// ========================================
	// ��� ������
	// ========================================
	int m_nOrderQuantity;        // ���� ����

	// ========================================
	// ��� �Լ�
	// ========================================
	void UpdateExpectedStock();
	virtual BOOL OnInitDialog();

	// ========================================
	// �̺�Ʈ �ڵ鷯
	// ========================================
	afx_msg void OnBnClickedButtonMinus10();
	afx_msg void OnBnClickedButtonMinus5();
	afx_msg void OnBnClickedButtonPlus5();
	afx_msg void OnBnClickedButtonPlus10();
	afx_msg void OnEnChangeEditOrderQuantity();
	afx_msg void OnBnClickedOk();
};