// CAIOrderDlg.cpp

#include "pch.h"
#include "InventoryManager.h" // 프로젝트 헤더
#include "CAIOrderDlg.h"
#include "afxdialogex.h"

// [헬퍼 함수] 숫자 3자리마다 콤마(,) 찍기 (가독성 UP)
CString FormatComma(int nNum)
{
    CString str;
    str.Format(_T("%d"), nNum);
    int nLen = str.GetLength();
    for (int i = nLen - 3; i > 0; i -= 3)
        str.Insert(i, _T(","));
    return str;
}

IMPLEMENT_DYNAMIC(CAIOrderDlg, CDialogEx)

CAIOrderDlg::CAIOrderDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_AI_ORDER_DIALOG, pParent)
{
}

CAIOrderDlg::~CAIOrderDlg()
{
}

void CAIOrderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_AI_RECOMMEND, m_listRecommend);
}

BEGIN_MESSAGE_MAP(CAIOrderDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CAIOrderDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// =======================================================
// 🟢 [핵심] 리스트 초기화 (컬럼 설정 및 데이터 표시)
// =======================================================
BOOL CAIOrderDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 1. [중요] 리스트 스타일을 'Report(자세히 보기)'로 강제 변경
    // (리소스 편집기에서 설정을 깜빡했어도 코드로 해결됨)
    m_listRecommend.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
    m_listRecommend.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // 2. 컬럼 설정 (말씀하신 순서대로 배치: 품번 -> 상품명 -> 발주개수)
    // 너비를 넉넉하게 잡아서 잘리지 않게 했습니다.
    m_listRecommend.InsertColumn(0, _T("품번"), LVCFMT_LEFT, 120);
    m_listRecommend.InsertColumn(1, _T("상품명"), LVCFMT_LEFT, 220); // 상품명은 길니까 넓게
    m_listRecommend.InsertColumn(2, _T("발주 개수"), LVCFMT_RIGHT, 100); // ★ 가장 중요!

    // (참고용 정보: 왜 이만큼 발주하는지 근거 데이터)
    m_listRecommend.InsertColumn(3, _T("현재고"), LVCFMT_RIGHT, 80);
    m_listRecommend.InsertColumn(4, _T("30일 판매량"), LVCFMT_RIGHT, 100);

    // 3. 데이터 채워 넣기
    for (int i = 0; i < m_vecRecommend.size(); i++)
    {
        const auto& item = m_vecRecommend[i];

        // 0번 컬럼: 품번
        int nIndex = m_listRecommend.InsertItem(i, item.strOptionCode);

        // 1번 컬럼: 상품명
        m_listRecommend.SetItemText(nIndex, 1, item.strProductName);

        // 2번 컬럼: 추천 발주량 (콤마 적용)
        m_listRecommend.SetItemText(nIndex, 2, FormatComma(item.nRecommended));

        // 3번 컬럼: 현재고
        m_listRecommend.SetItemText(nIndex, 3, FormatComma(item.nCurrentStock));

        // 4번 컬럼: 30일 판매량
        m_listRecommend.SetItemText(nIndex, 4, FormatComma(item.nSales30Days));
    }

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAIOrderDlg::SetRecommendData(const std::vector<AI_RECOMMEND_ITEM>& data)
{
    m_vecRecommend = data;
}

void CAIOrderDlg::OnBnClickedOk()
{
    if (m_vecRecommend.empty())
    {
        AfxMessageBox(_T("발주할 내역이 없습니다."));
        return;
    }

    CString strMsg;
    strMsg.Format(_T("총 %d건의 품목을 추천 수량대로 발주하시겠습니까?"), (int)m_vecRecommend.size());
    if (AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        CDialogEx::OnOK();
    }
}