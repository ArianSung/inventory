// === CStatsDlg.cpp (FULL) ===
#include "pch.h"
#include "CStatsDlg.h"
#include "DBManager.h"
#include "resource.h"
#include <afxcmn.h> 

int CalcColWidthByContent(CListCtrl& list, int col, int pad /*=2*/)
{
    CHeaderCtrl* hdr = list.GetHeaderCtrl();
    CClientDC dc(&list);
    CFont* pOld = dc.SelectObject(list.GetFont());

    int w = 0;
    // (�ʿ��ϸ� ��� �ʺ� �ݿ�)
    // if (hdr) { ... }

    const int n = list.GetItemCount();
    for (int i = 0; i < n; ++i) {
        CString t = list.GetItemText(i, col);
        CSize s = dc.GetTextExtent(t);
        if (s.cx > w) w = s.cx;
    }

    dc.SelectObject(pOld);
    return w + pad;
}

IMPLEMENT_DYNAMIC(CStatsDlg, CDialogEx)

CStatsDlg::CStatsDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_STATS_DIALOG, pParent)
{
}

CStatsDlg::~CStatsDlg() {}

void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_STATS_ORDERS, m_listOrders);
    DDX_Control(pDX, IDC_LIST_STATS_BRAND, m_listBrand);
    DDX_Control(pDX, IDC_LIST_STATS_DAILY, m_listDaily);
}

BEGIN_MESSAGE_MAP(CStatsDlg, CDialogEx)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CStatsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    InitLists();
    LoadStats();
    return TRUE;
}

// [EDIT] CStatsDlg::InitLists() ��ü ��ü
void CStatsDlg::InitLists()
{
    // [ADD] Report ���� + OwnerData ����
    m_listOrders.ModifyStyle(0, LVS_REPORT);
    m_listBrand.ModifyStyle(0, LVS_REPORT);
    m_listDaily.ModifyStyle(0, LVS_REPORT);
    m_listOrders.ModifyStyle(LVS_OWNERDATA, 0);
    m_listBrand.ModifyStyle(LVS_OWNERDATA, 0);
    m_listDaily.ModifyStyle(LVS_OWNERDATA, 0);

    DWORD ex = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_listOrders.SetExtendedStyle(ex);
    m_listBrand.SetExtendedStyle(ex);
    m_listDaily.SetExtendedStyle(ex);

    // �ֹ���� �÷�
    m_listOrders.DeleteAllItems();
    if (auto* h = m_listOrders.GetHeaderCtrl())
        for (int i = h->GetItemCount() - 1; i >= 0; --i) m_listOrders.DeleteColumn(i);
    m_listOrders.InsertColumn(0, _T("�ֹ���ȣ"), LVCFMT_LEFT, 80);
    m_listOrders.InsertColumn(1, _T("�Ͻ�"), LVCFMT_LEFT, 120);
    m_listOrders.InsertColumn(2, _T("�귣��"), LVCFMT_LEFT, 100);
    m_listOrders.InsertColumn(3, _T("��ǰ��"), LVCFMT_LEFT, 180);
    m_listOrders.InsertColumn(4, _T("�ɼ��ڵ�"), LVCFMT_LEFT, 110);
    m_listOrders.InsertColumn(5, _T("����"), LVCFMT_LEFT, 60);
    m_listOrders.InsertColumn(6, _T("������"), LVCFMT_LEFT, 60);
    m_listOrders.InsertColumn(7, _T("����"), LVCFMT_RIGHT, 60);
    m_listOrders.InsertColumn(8, _T("�ܰ�"), LVCFMT_RIGHT, 80);
    m_listOrders.InsertColumn(9, _T("�ݾ�"), LVCFMT_LEFT, 90);

    // �귣�庰
    m_listBrand.DeleteAllItems();
    if (auto* h2 = m_listBrand.GetHeaderCtrl())
        for (int i = h2->GetItemCount() - 1; i >= 0; --i) m_listBrand.DeleteColumn(i);
    m_listBrand.InsertColumn(0, _T("�귣��"), LVCFMT_LEFT, 150);
    m_listBrand.InsertColumn(1, _T("�Ǹż���"), LVCFMT_RIGHT, 80);
    m_listBrand.InsertColumn(2, _T("����"), LVCFMT_LEFT, 100);

    // [ADD] ��¥�� �Ѹ���
    m_listDaily.DeleteAllItems();
    if (auto* h3 = m_listDaily.GetHeaderCtrl())
        for (int i = h3->GetItemCount() - 1; i >= 0; --i) m_listDaily.DeleteColumn(i);
    m_listDaily.InsertColumn(0, _T("��¥"), LVCFMT_LEFT, 100);
    m_listDaily.InsertColumn(1, _T("����"), LVCFMT_LEFT, 110);
}

void CStatsDlg::LoadStats()
{
    if (!m_bDBConnected || !m_pDB) {
        m_listOrders.DeleteAllItems();
        m_listBrand.DeleteAllItems();
        m_listDaily.DeleteAllItems();
        return;
    }

    std::vector<std::vector<CString>> rows;

    // �ֹ���� (�ֽ� 200)
    {
        CString sql =
            _T("SELECT o.order_id, o.order_date, b.brand_name, p.product_name, po.option_code, ")
            _T("       c.color_name, s.size_name, od.quantity, od.price_per_item, ")
            _T("       (od.quantity * od.price_per_item) AS line_total ")
            _T("FROM order_details od ")
            _T("JOIN orders o           ON o.order_id = od.order_id ")
            _T("JOIN product_options po ON po.option_id = od.option_id ")
            _T("JOIN products p         ON p.product_id = po.product_id ")
            _T("JOIN brands b           ON b.brand_id = p.brand_id ")
            _T("JOIN colors c           ON c.color_id = po.color_id ")
            _T("JOIN sizes  s           ON s.size_id  = po.size_id ")
            _T("ORDER BY o.order_date DESC, o.order_id DESC LIMIT 200");

        rows.clear();
        if (m_pDB->SelectToRows(sql, rows)) {
            m_listOrders.SetRedraw(FALSE);
            m_listOrders.DeleteAllItems();
            for (size_t i = 0; i < rows.size(); ++i) {
                const auto& r = rows[i];
                int idx = m_listOrders.InsertItem((int)i, r.size() > 0 ? r[0] : _T(""));
                for (int col = 1; col <= 9; ++col)
                    m_listOrders.SetItemText(idx, col, (r.size() > col ? r[col] : _T("")));
            }
            m_listOrders.SetRedraw(TRUE);
            AutoSizeColumns(m_listOrders); 
            AutoSizeColumns(m_listBrand);
            AutoSizeColumns(m_listDaily);
        }
    }

    // �귣�庰 �Ǹ� TOP 20
    {
        CString sql =
            _T("SELECT b.brand_name, SUM(od.quantity) AS qty_sold, ")
            _T("       SUM(od.quantity * od.price_per_item) AS revenue ")
            _T("FROM order_details od ")
            _T("JOIN orders o           ON o.order_id = od.order_id ")
            _T("JOIN product_options po ON po.option_id = od.option_id ")
            _T("JOIN products p         ON p.product_id = po.product_id ")
            _T("JOIN brands b           ON b.brand_id = p.brand_id ")
            _T("GROUP BY b.brand_id, b.brand_name ")
            _T("ORDER BY qty_sold DESC, revenue DESC LIMIT 20");

        rows.clear();
        if (m_pDB->SelectToRows(sql, rows)) {
            m_listBrand.SetRedraw(FALSE);
            m_listBrand.DeleteAllItems();
            for (size_t i = 0; i < rows.size(); ++i) {
                const auto& r = rows[i];
                int idx = m_listBrand.InsertItem((int)i, r.size() > 0 ? r[0] : _T(""));
                m_listBrand.SetItemText(idx, 1, r.size() > 1 ? r[1] : _T(""));
                m_listBrand.SetItemText(idx, 2, r.size() > 2 ? r[2] : _T(""));
            }
            m_listBrand.SetRedraw(TRUE);
            AutoSizeColumns(m_listBrand); // [ADD]
        }
    }

    // [ADD] ��¥�� �Ѹ��� (�ֱ� 60��)
    {
        CString sql =
            _T("SELECT DATE(o.order_date) AS ymd, ")
            _T("       COALESCE(SUM(od.quantity * od.price_per_item), 0) AS revenue ")
            _T("FROM orders o ")
            _T("JOIN order_details od ON od.order_id = o.order_id ")
            _T("GROUP BY DATE(o.order_date) ")
            _T("ORDER BY ymd DESC LIMIT 60");

        rows.clear();
        if (m_pDB->SelectToRows(sql, rows)) {
            m_listDaily.SetRedraw(FALSE);
            m_listDaily.DeleteAllItems();
            for (size_t i = 0; i < rows.size(); ++i) {
                const auto& r = rows[i];
                int idx = m_listDaily.InsertItem((int)i, r.size() > 0 ? r[0] : _T(""));
                m_listDaily.SetItemText(idx, 1, r.size() > 1 ? r[1] : _T(""));
            }
            m_listDaily.SetRedraw(TRUE);
            AutoSizeColumns(m_listDaily); // [ADD]
        }
    }
}
// ��޸��� ���� ó��(����)
void CStatsDlg::OnClose() { DestroyWindow(); }
void CStatsDlg::OnCancel() { DestroyWindow(); }
BOOL CStatsDlg::DestroyWindow()
{
    BOOL b = CDialogEx::DestroyWindow();
    delete this; // �ڱ� ����(���� ����)
    return b;
}


void CStatsDlg::AutoSizeColumns(CListCtrl& list)
{
    CHeaderCtrl* hdr = list.GetHeaderCtrl();
    if (!hdr) return;
    const int cols = hdr->GetItemCount();
    for (int c = 0; c < cols; ++c) {
        list.SetColumnWidth(c, LVSCW_AUTOSIZE);
        int wc = list.GetColumnWidth(c);
        list.SetColumnWidth(c, LVSCW_AUTOSIZE_USEHEADER);
        int wh = list.GetColumnWidth(c);
        list.SetColumnWidth(c, max(wc, wh) + 6);
    }
}
