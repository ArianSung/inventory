#pragma once
#include "afxdialogex.h"



// CAddOptionDlg 대화 상자

class CAddOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOptionDlg)

public:
	CAddOptionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddOptionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticProductName;
	CStatic m_staticBrandName;
	CStatic m_staticCategoryName;
	CComboBox m_comboColor;
	CComboBox m_comboSize;
	int m_nStock;
	CStatic m_staticPreviewCode;

	// ========================================
	// 입력 변수 (외부에서 설정)
	// ========================================
	int m_nProductID;           // 옵션을 추가할 상품의 ID
	CString m_strProductName;   // 상품명 (표시용)
	CString m_strBrandName;     // 브랜드명 (표시용)
	CString m_strCategoryName;  // 카테고리명 (표시용)

	// ========================================
	// 출력 변수 (사용자가 선택한 값)
	// ========================================
	CString m_strColorName;     // 선택된 색상 이름
	CString m_strSizeName;      // 선택된 사이즈 이름

private:
	void UpdatePreviewCode();
public:
	afx_msg void OnCbnSelchangeComboOptionColor();
	afx_msg void OnCbnSelchangeComboOptionSize();
};
