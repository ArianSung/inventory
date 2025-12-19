#pragma once
#include <vector>
#include "resource.h"
#include <afxcmn.h>

class CDBManager;

class CStatsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CStatsDlg)

public:
    CStatsDlg(CWnd* pParent = nullptr);
    virtual ~CStatsDlg();

    void InitDB(CDBManager* pDB, BOOL bConnected) { m_pDB = pDB; m_bDBConnected = bConnected; }
    void Reload();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_STATS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

    CListCtrl m_listOrders;
    CListCtrl m_listBrand;
    CListCtrl m_listDaily;
    CDateTimeCtrl m_dtpStart;
    CDateTimeCtrl m_dtpEnd;

    void AutoSizeColumns(CListCtrl& list);

    CDBManager* m_pDB = nullptr;
    BOOL        m_bDBConnected = FALSE;

    void InitLists();
    void LoadStats();

    void LoadStatsByPeriod(const CString& strStartDate, const CString& strEndDate);
    void LoadBrandStatsByPeriod(const CString& strStartDate, const CString& strEndDate);
    void LoadDailyStatsByPeriod(const CString& strStartDate, const CString& strEndDate);

    afx_msg void OnClose();
    virtual void OnCancel();
    virtual BOOL DestroyWindow();

    afx_msg void OnBnClickedButtonSearchPeriod();
};
