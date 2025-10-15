
#include "pch.h"
#include "DBManager.h"
#include <atlconv.h>
// CString을 UTF-8 CStringA로 변환하는 헬퍼 함수
CStringA ConvertToUtf8A(const CString & strUnicode)
{
	int nLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLen];
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, pBuffer, nLen, NULL, NULL);
	CStringA strResult(pBuffer);
	delete[] pBuffer;
	return strResult;
}

// ========================================
// 싱글톤 인스턴스 초기화
// ========================================
CDBManager* CDBManager::m_pInstance = nullptr;

// ========================================
// 생성자
// ========================================
CDBManager::CDBManager()
	: m_pConnection(nullptr)
	, m_pResult(nullptr)
	, m_bConnected(FALSE)
	, m_nLastErrorCode(0)
{
	// MySQL 라이브러리 초기화
	m_pConnection = mysql_init(nullptr);
	if (m_pConnection == nullptr)
	{
		SetError(_T("MySQL 초기화 실패"));
	}
}

// ========================================
// 소멸자
// ========================================
CDBManager::~CDBManager()
{
	Disconnect();
	if (m_pConnection != nullptr)
	{
		mysql_close(m_pConnection);
		m_pConnection = nullptr;
	}
}

// ========================================
// 싱글톤 인스턴스 가져오기
// ========================================
CDBManager* CDBManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CDBManager();
	}
	return m_pInstance;
}

// ========================================
// 싱글톤 인스턴스 삭제
// ========================================
void CDBManager::DestroyInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ========================================
// 데이터베이스 연결
// ========================================
BOOL CDBManager::Connect(const DB_CONFIG& config)
{
	if (m_bConnected)
	{
		Disconnect();
	}
	m_config = config;
	mysql_options(m_pConnection, MYSQL_SET_CHARSET_NAME, "utf8mb4");
	mysql_options(m_pConnection, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4");

	MYSQL* pResult = mysql_real_connect(
		m_pConnection,
		CStringA(m_config.strHost),
		CStringA(m_config.strUser),
		CStringA(m_config.strPassword),
		CStringA(m_config.strDatabase),
		m_config.nPort,
		nullptr,
		0
	);

	if (pResult == nullptr)
	{
		CString strError;
		strError.Format(_T("DB 연결 실패: %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		SetError(strError, mysql_errno(m_pConnection));
		m_bConnected = FALSE;
		return FALSE;
	}

	mysql_set_character_set(m_pConnection, "utf8mb4");
	m_bConnected = TRUE;
	SetError(_T(""), 0);
	return TRUE;
}

// ========================================
// 데이터베이스 연결 해제
// ========================================
void CDBManager::Disconnect()
{
	FreeResult();
	if (m_pConnection != nullptr && m_bConnected)
	{
		mysql_close(m_pConnection);
		m_pConnection = mysql_init(nullptr);
		m_bConnected = FALSE;
	}
}

// ========================================
// 연결 상태 확인
// ========================================
BOOL CDBManager::IsConnected() const
{
	return m_bConnected;
}

// ========================================
// 재연결
// ========================================
BOOL CDBManager::Reconnect()
{
	Disconnect();
	return Connect(m_config);
}

// ========================================
// 일반 쿼리 실행 (INSERT, UPDATE, DELETE)
// ========================================
BOOL CDBManager::ExecuteQuery(const CString& strQuery)
{
	if (!m_bConnected)
	{
		SetError(_T("데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}

	// 쿼리 실행 (UTF-8로 변환하여 전송)
	CStringA strQueryUtf8 = ConvertToUtf8A(strQuery);
	if (mysql_query(m_pConnection, strQueryUtf8) != 0)
	{
		CString strError;
		strError.Format(_T("쿼리 실행 실패: %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		SetError(strError, mysql_errno(m_pConnection));
		return FALSE;
	}

	SetError(_T(""), 0);
	return TRUE;
}

// ========================================
// SELECT 쿼리 실행
// ========================================
BOOL CDBManager::ExecuteSelect(const CString& strQuery)
{
	FreeResult();
	if (!ExecuteQuery(strQuery))
	{
		return FALSE;
	}

	m_pResult = mysql_store_result(m_pConnection);
	if (m_pResult == nullptr)
	{
		if (mysql_errno(m_pConnection) != 0)
		{
			CString strError;
			strError.Format(_T("결과 가져오기 실패: %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
			SetError(strError, mysql_errno(m_pConnection));
			return FALSE;
		}
	}
	return TRUE;
}

// ========================================
// 한 행 가져오기
// ========================================
MYSQL_ROW CDBManager::FetchRow()
{
	if (m_pResult == nullptr) return nullptr;
	return mysql_fetch_row(m_pResult);
}

// ========================================
// 결과 행 수
// ========================================
int CDBManager::GetRowCount()
{
	if (m_pResult == nullptr) return 0;
	return (int)mysql_num_rows(m_pResult);
}

// ========================================
// 결과 해제
// ========================================
void CDBManager::FreeResult()
{
	if (m_pResult != nullptr)
	{
		mysql_free_result(m_pResult);
		m_pResult = nullptr;
	}
}

// ========================================
// 마지막 에러 메시지
// ========================================
CString CDBManager::GetLastError() const
{
	return m_strLastError;
}

// ========================================
// 마지막 에러 코드
// ========================================
int CDBManager::GetLastErrorCode() const
{
	return m_nLastErrorCode;
}

// ========================================
// 에러 설정 (내부 함수)
// ========================================
void CDBManager::SetError(const CString& strError, int nCode)
{
	m_strLastError = strError;
	m_nLastErrorCode = nCode;
}

// ========================================
// UTF-8 변환 (UTF-8 → CString)
// ========================================
CString CDBManager::ConvertFromUTF8(const char* szInput)
{
	if (szInput == nullptr || strlen(szInput) == 0) return _T("");
	int nLen = MultiByteToWideChar(CP_UTF8, 0, szInput, -1, nullptr, 0);
	if (nLen <= 0) return _T("");
	wchar_t* pBuffer = new wchar_t[nLen + 1];
	MultiByteToWideChar(CP_UTF8, 0, szInput, -1, pBuffer, nLen);
	CString strResult(pBuffer);
	delete[] pBuffer;
	return strResult;
}

// ========================================
// UTF-8 변환 (CString → UTF-8)
// ========================================
CString CDBManager::ConvertToUTF8(const CString& strInput)
{
	if (strInput.IsEmpty()) return _T("");
	int nLen = WideCharToMultiByte(CP_UTF8, 0, strInput, -1, nullptr, 0, nullptr, nullptr);
	if (nLen <= 0) return strInput;
	char* pBuffer = new char[nLen + 1];
	WideCharToMultiByte(CP_UTF8, 0, strInput, -1, pBuffer, nLen, nullptr, nullptr);
	CString strResult(pBuffer);
	delete[] pBuffer;
	return strResult;
}

// ========================================
// SQL Injection 방지 (문자열 이스케이프)
// ========================================
CString CDBManager::EscapeString(const CString& strInput)
{
	if (!m_bConnected || m_pConnection == nullptr)
	{
		return strInput;
	}

	// CString(UTF-16) -> CStringA(UTF-8) 변환
	CStringA strInputUtf8 = ConvertToUtf8A(strInput);
	int nLength = strInputUtf8.GetLength();

	// 이스케이프를 위한 버퍼 (UTF-8 기준)
	char* szBuffer = new char[nLength * 2 + 1];
	memset(szBuffer, 0, nLength * 2 + 1);

	mysql_real_escape_string(m_pConnection, szBuffer, strInputUtf8, nLength);

	// 이스케이프된 UTF-8 char* -> CString(UTF-16)으로 다시 변환
	CString strResult = ConvertFromUTF8(szBuffer);
	delete[] szBuffer;

	return strResult;
}


// ========================================
// 전체 재고 조회
// ========================================
BOOL CDBManager::GetInventoryList(std::vector<INVENTORY_ITEM>& vecItems)
{
	vecItems.clear();
	if (!m_bConnected) {
		SetError(_T("데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}
	CString strQuery =
		_T("SELECT po.option_id, po.option_code, po.stock, p.product_id, p.product_name, ")
		_T("b.brand_name, c.color_name, s.size_name, cat.category_name ")
		_T("b.brand_name, c.color_name, s.size_name ")
		_T("FROM product_options po ")
		_T("JOIN products p ON po.product_id = p.product_id ")
		_T("JOIN brands b ON p.brand_id = b.brand_id ")
		_T("JOIN colors c ON po.color_id = c.color_id ")
		_T("JOIN sizes s ON po.size_id = s.size_id ")
		_T("JOIN categories cat ON p.category_id = cat.category_id ")
		_T("ORDER BY po.stock ASC, p.product_name ASC");

	if (!ExecuteSelect(strQuery)) return FALSE;
	if (m_pResult == nullptr) return TRUE; // 데이터가 0건인 것도 성공이므로 TRUE 반환

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(m_pResult)) != nullptr) {
		INVENTORY_ITEM item;
		if (row[0] && row[1] && row[2] && row[3] && row[4] && row[5] && row[6] && row[7] && row[8]) {
			item.nOptionID = atoi(row[0]);
			item.strOptionCode = ConvertFromUTF8(row[1]);
			item.nStock = atoi(row[2]);
			item.nProductID = atoi(row[3]);
			item.strProductName = ConvertFromUTF8(row[4]);
			item.strBrandName = ConvertFromUTF8(row[5]);
			item.strColorName = ConvertFromUTF8(row[6]);
			item.strSizeName = ConvertFromUTF8(row[7]);
			item.strCategoryName = ConvertFromUTF8(row[8]);
			vecItems.push_back(item);
		}
	}
	FreeResult();
	SetError(_T(""), 0);
	return TRUE;
}

// ========================================
// 재고 수정
// ========================================
BOOL CDBManager::UpdateStock(int nOptionID, int nNewStock)
{
	if (!m_bConnected) {
		SetError(_T("데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}
	if (nNewStock < 0) {
		SetError(_T("재고는 0 이상이어야 합니다."));
		return FALSE;
	}
	CString strQuery;
	strQuery.Format(_T("UPDATE product_options SET stock = %d WHERE option_id = %d"), nNewStock, nOptionID);
	return ExecuteQuery(strQuery);
}

// ========================================
// 재고 추가 - 발주
// ========================================
BOOL CDBManager::AddStock(int nOptionID, int nQuantity)
{
	if (!m_bConnected) {
		SetError(_T("데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}
	if (nQuantity <= 0) {
		SetError(_T("발주 수량은 1 이상이어야 합니다."));
		return FALSE;
	}
	CString strQuery;
	strQuery.Format(_T("UPDATE product_options SET stock = stock + %d WHERE option_id = %d"), nQuantity, nOptionID);
	return ExecuteQuery(strQuery);
}

// ========================================
// 품목 삭제
// ========================================
BOOL CDBManager::DeleteInventoryItem(int nOptionID)
{
	if (m_pConnection == nullptr) {
		AfxMessageBox(_T("디버그: DBManager 오류 - DB에 연결되지 않았습니다."));
		SetError(_T("DB에 연결되지 않았습니다."));
		return FALSE;
	}

	if (mysql_query(m_pConnection, "START TRANSACTION")) {
		CString msg;
		msg.Format(_T("디버그: 트랜잭션 시작 실패 - %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		AfxMessageBox(msg);
		SetError(ConvertFromUTF8(mysql_error(m_pConnection)));
		return FALSE;
	}
	AfxMessageBox(_T("디버그: 1. 트랜잭션 시작 성공"));

	CString strQuery;
	my_ulonglong affected_rows;

	// order_details 삭제
	strQuery.Format(_T("DELETE FROM order_details WHERE option_id = %d"), nOptionID);
	if (mysql_query(m_pConnection, ConvertToUtf8A(strQuery))) {
		CString msg;
		msg.Format(_T("디버그: order_details 삭제 실패 - %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		AfxMessageBox(msg);
		SetError(ConvertFromUTF8(mysql_error(m_pConnection)));
		mysql_query(m_pConnection, "ROLLBACK");
		return FALSE;
	}
	affected_rows = mysql_affected_rows(m_pConnection);
	strQuery.Format(_T("디버그: 2. order_details 삭제 시도 (영향 받은 행: %llu)"), affected_rows);
	AfxMessageBox(strQuery);

	// cart 삭제
	strQuery.Format(_T("DELETE FROM cart WHERE option_id = %d"), nOptionID);
	if (mysql_query(m_pConnection, ConvertToUtf8A(strQuery))) {
		CString msg;
		msg.Format(_T("디버그: cart 삭제 실패 - %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		AfxMessageBox(msg);
		SetError(ConvertFromUTF8(mysql_error(m_pConnection)));
		mysql_query(m_pConnection, "ROLLBACK");
		return FALSE;
	}
	affected_rows = mysql_affected_rows(m_pConnection);
	strQuery.Format(_T("디버그: 3. cart 삭제 시도 (영향 받은 행: %llu)"), affected_rows);
	AfxMessageBox(strQuery);

	// product_options 삭제
	strQuery.Format(_T("DELETE FROM product_options WHERE option_id = %d"), nOptionID);
	if (mysql_query(m_pConnection, ConvertToUtf8A(strQuery))) {
		CString msg;
		msg.Format(_T("디버그: product_options 삭제 실패 - %s"), ConvertFromUTF8(mysql_error(m_pConnection)));
		AfxMessageBox(msg);
		SetError(ConvertFromUTF8(mysql_error(m_pConnection)));
		mysql_query(m_pConnection, "ROLLBACK");
		return FALSE;
	}
	affected_rows = mysql_affected_rows(m_pConnection);
	strQuery.Format(_T("디버그: 4. product_options 삭제 시도 (영향 받은 행: %llu)"), affected_rows);
	AfxMessageBox(strQuery);

	// ✨ 핵심 진단: product_options 테이블에서 행이 삭제되지 않았다면 실패 처리
	if (affected_rows == 0) {
		AfxMessageBox(_T("디버그: 5. [실패] product_options 테이블에서 삭제된 행이 0개입니다. 롤백합니다."));
		SetError(_T("DB 오류: 삭제할 품목을 찾지 못했습니다 (영향받은 행 없음). Option ID 값을 확인하세요."));
		mysql_query(m_pConnection, "ROLLBACK");
		return FALSE;
	}
	// 6. 모든 삭제가 성공했으면 최종 적용 (커밋)
	if (mysql_query(m_pConnection, "COMMIT") != 0) { // ✅ != 0 비교 추가!
		SetError(ConvertFromUTF8(mysql_error(m_pConnection)));
		mysql_query(m_pConnection, "ROLLBACK");
		return FALSE;
	}

	SetError(_T(""), 0); // 성공 시 에러 메시지 초기화
	return TRUE;
}

// ========================================
// '상품 추가' 관련 신규 함수들
// ========================================

BOOL CDBManager::GetBrandList(std::vector<CString>& vecBrands)
{
	vecBrands.clear();
	CString strQuery = _T("SELECT brand_name FROM brands ORDER BY brand_name");
	if (!ExecuteSelect(strQuery)) return FALSE;
	MYSQL_ROW row;
	while ((row = FetchRow()) != nullptr) {
		if (row[0]) vecBrands.push_back(ConvertFromUTF8(row[0]));
	}
	FreeResult();
	return TRUE;
}

BOOL CDBManager::GetColorList(std::vector<CString>& vecColors)
{
	vecColors.clear();
	CString strQuery = _T("SELECT color_name FROM colors ORDER BY color_name");
	if (!ExecuteSelect(strQuery)) return FALSE;
	MYSQL_ROW row;
	while ((row = FetchRow()) != nullptr) {
		if (row[0]) vecColors.push_back(ConvertFromUTF8(row[0]));
	}
	FreeResult();
	return TRUE;
}

BOOL CDBManager::GetSizeList(std::vector<CString>& vecSizes)
{
	vecSizes.clear();
	CString strQuery = _T("SELECT size_code FROM sizes ORDER BY size_id");
	if (!ExecuteSelect(strQuery)) return FALSE;
	MYSQL_ROW row;
	while ((row = FetchRow()) != nullptr) {
		if (row[0]) vecSizes.push_back(ConvertFromUTF8(row[0]));
	}
	FreeResult();
	return TRUE;
}

BOOL CDBManager::GetCategoryList(std::vector<CString>& vecCategories)
{
	vecCategories.clear();
	CString strQuery = _T("SELECT category_name FROM categories ORDER BY category_name");
	if (!ExecuteSelect(strQuery)) return FALSE;
	MYSQL_ROW row;
	while ((row = FetchRow()) != nullptr) {
		if (row[0]) vecCategories.push_back(ConvertFromUTF8(row[0]));
	}
	FreeResult();
	return TRUE;
}

BOOL CDBManager::AddProduct(
	const CString& strBrandName,
	const CString& strCategoryName,
	const CString& strProductName,
	int nPrice,
	const CString& strDescription,
	const CString& strImagesJson,
	const CString& strOptionsJson)
{
	if (!m_bConnected) {
		SetError(_T("데이터베이스에 연결되지 않았습니다."));
		return FALSE;
	}
	CString escBrandName = EscapeString(strBrandName);
	CString escCategoryName = EscapeString(strCategoryName);
	CString escProductName = EscapeString(strProductName);
	CString escDescription = EscapeString(strDescription);
	CString escImagesJson = EscapeString(strImagesJson);
	CString escOptionsJson = EscapeString(strOptionsJson);

	CString strQuery;
	strQuery.Format(
		_T("CALL add_product('%s', '%s', '%s', %d, '%s', '%s', '%s')"),
		escBrandName, escCategoryName, escProductName, nPrice,
		escDescription, escImagesJson, escOptionsJson
	);
	if (!ExecuteQuery(strQuery)) {
		return FALSE;
	}
	SetError(_T(""), 0);
	return TRUE;
}

CString CDBManager::GetBrandCodeFromName(const CString& strName)
{
	CString strQuery;
	strQuery.Format(_T("SELECT brand_code FROM brands WHERE brand_name = '%s'"), EscapeString(strName));
	if (ExecuteSelect(strQuery) && GetRowCount() > 0) {
		MYSQL_ROW row = FetchRow();
		CString strCode = ConvertFromUTF8(row[0]);
		FreeResult();
		return strCode;
	}
	FreeResult();
	return _T("?");
}

CString CDBManager::GetCategoryCodeFromName(const CString& strName)
{
	CString strQuery;
	strQuery.Format(_T("SELECT category_code FROM categories WHERE category_name = '%s'"), EscapeString(strName));
	if (ExecuteSelect(strQuery) && GetRowCount() > 0) {
		MYSQL_ROW row = FetchRow();
		CString strCode = ConvertFromUTF8(row[0]);
		FreeResult();
		return strCode;
	}
	FreeResult();
	return _T("?");
}

CString CDBManager::GetColorCodeFromName(const CString& strName)
{
	CString strQuery;
	strQuery.Format(_T("SELECT color_code FROM colors WHERE color_name = '%s'"), EscapeString(strName));
	if (ExecuteSelect(strQuery) && GetRowCount() > 0) {
		MYSQL_ROW row = FetchRow();
		CString strCode = ConvertFromUTF8(row[0]);
		FreeResult();
		return strCode;
	}
	FreeResult();
	return _T("?");
}

BOOL CDBManager::SelectToRows(const CString& sql, std::vector<std::vector<CString>>& rows)
{
	rows.clear();

	if (!ExecuteSelect(sql))    // 프로젝트에 이미 있는 함수명 그대로 사용
		return FALSE;

	MYSQL_RES* res = m_pResult; // 프로젝트 멤버명(m_pResult) 그대로 사용
	if (!res) { FreeResult(); return FALSE; }

	int nFields = (int)mysql_num_fields(res);
	MYSQL_ROW row = nullptr;

	while ((row = mysql_fetch_row(res)) != nullptr)
	{
		unsigned long* lengths = mysql_fetch_lengths(res);
		std::vector<CString> one; one.reserve(nFields);
		for (int i = 0; i < nFields; ++i) {
			if (!row[i]) one.push_back(_T(""));
			else {
				CStringW w = CA2W(row[i], CP_UTF8);
				one.push_back(w);
			}
		}
		rows.push_back(std::move(one));
	}

	FreeResult();
	return TRUE;
}