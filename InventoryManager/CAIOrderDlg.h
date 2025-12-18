#pragma once
#include "afxdialogex.h"
#include <vector>

// 추천 아이템 구조체
struct AI_RECOMMEND_ITEM
{
    int nOptionID;
    CString strProductName;
    CString strOptionCode;
    int nCurrentStock;     // 현재 재고
    int nSales30Days;      // 30일 판매량
    int nRecommended;      // 추천 발주량
};

class CAIOrderDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAIOrderDlg)

public:
    CAIOrderDlg(CWnd* pParent = nullptr);
    virtual ~CAIOrderDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_AI_ORDER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(); // 초기화 함수

    DECLARE_MESSAGE_MAP()

public:
    CListCtrl m_listRecommend;
    std::vector<AI_RECOMMEND_ITEM> m_vecRecommend; // 추천 목록 저장

    // 이 함수를 호출하여 외부에서 데이터를 넣어줍니다.
    void SetRecommendData(const std::vector<AI_RECOMMEND_ITEM>& data);

    afx_msg void OnBnClickedOk();
};