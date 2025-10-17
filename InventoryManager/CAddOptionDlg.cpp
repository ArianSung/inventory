// CAddOptionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "InventoryManager.h"
#include "afxdialogex.h"
#include "CAddOptionDlg.h"
#include "DBManager.h"


// CAddOptionDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddOptionDlg, CDialogEx)

CAddOptionDlg::CAddOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_OPTION, pParent)
	, m_nProductID(0)           // 추가
	, m_strProductName(_T(""))  // 추가
	, m_strBrandName(_T(""))    // 추가
	, m_strCategoryName(_T("")) // 추가
	, m_strColorName(_T(""))    // 추가
	, m_strSizeName(_T(""))     // 추가
	, m_nStock(0)
{

}

CAddOptionDlg::~CAddOptionDlg()
{
}

void CAddOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PRODUCT_NAME_DISPLAY, m_staticProductName);
	DDX_Control(pDX, IDC_STATIC_BRAND_DISPLAY, m_staticBrandName);
	DDX_Control(pDX, IDC_STATIC_CATEGORY_DISPLAY, m_staticCategoryName);
	DDX_Control(pDX, IDC_COMBO_OPTION_COLOR, m_comboColor);
	DDX_Control(pDX, IDC_COMBO_OPTION_SIZE, m_comboSize);
	DDX_Text(pDX, IDC_EDIT_OPTION_STOCK, m_nStock);
	DDX_Control(pDX, IDC_STATIC_OPTION_CODE_PREVIEW, m_staticPreviewCode);
}


BOOL CAddOptionDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 상품 정보 표시 (읽기 전용)
    CString strText;
    strText.Format(_T("상품명: %s"), m_strProductName);
    m_staticProductName.SetWindowText(strText);

    strText.Format(_T("브랜드: %s"), m_strBrandName);
    m_staticBrandName.SetWindowText(strText);

    strText.Format(_T("카테고리: %s"), m_strCategoryName);
    m_staticCategoryName.SetWindowText(strText);

    // DBManager 인스턴스 가져오기
    CDBManager* pDBManager = CDBManager::GetInstance();
    std::vector<CString> vecItems;

    // 색상 목록 채우기
    if (pDBManager->GetColorList(vecItems)) {
        for (const auto& item : vecItems) m_comboColor.AddString(item);
        if (m_comboColor.GetCount() > 0) m_comboColor.SetCurSel(0);
    }

    // 사이즈 목록 채우기 (카테고리별 필터링)
    if (pDBManager->GetSizeListByCategory(m_strCategoryName, vecItems)) {
        for (const auto& item : vecItems) m_comboSize.AddString(item);
        if (m_comboSize.GetCount() > 0) m_comboSize.SetCurSel(0);
    }

    // 초기 재고 기본값 설정
    m_nStock = 0;
    UpdateData(FALSE);

    // 옵션 코드 미리보기 업데이트
    UpdatePreviewCode();

    return TRUE;
}

BEGIN_MESSAGE_MAP(CAddOptionDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_OPTION_COLOR, &CAddOptionDlg::OnCbnSelchangeComboOptionColor)
    ON_CBN_SELCHANGE(IDC_COMBO_OPTION_SIZE, &CAddOptionDlg::OnCbnSelchangeComboOptionSize)
END_MESSAGE_MAP()


// CAddOptionDlg 메시지 처리기

void CAddOptionDlg::OnOK()
{
    UpdateData(TRUE);

    // 유효성 검사: 색상 선택 확인
    int nColorIndex = m_comboColor.GetCurSel();
    if (nColorIndex == CB_ERR) {
        AfxMessageBox(_T("색상을 선택해주세요."));
        return;
    }
    m_comboColor.GetLBText(nColorIndex, m_strColorName);

    // 유효성 검사: 사이즈 선택 확인
    int nSizeIndex = m_comboSize.GetCurSel();
    if (nSizeIndex == CB_ERR) {
        AfxMessageBox(_T("사이즈를 선택해주세요."));
        return;
    }
    m_comboSize.GetLBText(nSizeIndex, m_strSizeName);

    // 유효성 검사: 재고 확인
    if (m_nStock < 0) {
        AfxMessageBox(_T("재고는 0 이상이어야 합니다."));
        return;
    }

    // 모든 검사를 통과하면 다이얼로그 종료
    CDialogEx::OnOK();
}

void CAddOptionDlg::UpdatePreviewCode()
{
    CString strColor, strSize;
    int nIndex;

    // 현재 선택된 색상 가져오기
    nIndex = m_comboColor.GetCurSel();
    if (nIndex != CB_ERR) m_comboColor.GetLBText(nIndex, strColor);

    // 현재 선택된 사이즈 가져오기
    nIndex = m_comboSize.GetCurSel();
    if (nIndex != CB_ERR) m_comboSize.GetLBText(nIndex, strSize);

    // 이름을 코드로 변환
    CDBManager* pDBManager = CDBManager::GetInstance();
    CString strBrandCode = pDBManager->GetBrandCodeFromName(m_strBrandName);
    CString strCategoryCode = pDBManager->GetCategoryCodeFromName(m_strCategoryName);
    CString strColorCode = pDBManager->GetColorCodeFromName(strColor);
    CString strSizeCode = strSize;

    // 옵션 코드 미리보기 생성
    CString strPreview;
    strPreview.Format(_T("옵션 코드: %s-%s-%03d-%s-%s"),
        strBrandCode, strCategoryCode, m_nProductID, strColorCode, strSizeCode);

    m_staticPreviewCode.SetWindowText(strPreview);
}

void CAddOptionDlg::OnCbnSelchangeComboOptionColor()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdatePreviewCode();
}

void CAddOptionDlg::OnCbnSelchangeComboOptionSize()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdatePreviewCode();
}
