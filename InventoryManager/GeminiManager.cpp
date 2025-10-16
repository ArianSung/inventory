// GeminiManager.cpp : Gemini API 관리 클래스 구현
//

#include "pch.h"
#include "GeminiManager.h"
#include <sstream>
#include <atlconv.h>

// ========================================
// 싱글톤 인스턴스 초기화
// ========================================
CGeminiManager* CGeminiManager::m_pInstance = nullptr;

// ========================================
// 생성자 / 소멸자
// ========================================
CGeminiManager::CGeminiManager()
{
	m_strLastError = _T("");
}

CGeminiManager::~CGeminiManager()
{
}

// ========================================
// 싱글톤 인스턴스 관리
// ========================================
CGeminiManager* CGeminiManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGeminiManager();
	}
	return m_pInstance;
}

void CGeminiManager::DestroyInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ========================================
// 설정 관리
// ========================================
void CGeminiManager::SetConfig(const GEMINI_CONFIG& config)
{
	m_config = config;
}

GEMINI_CONFIG CGeminiManager::GetConfig() const
{
	return m_config;
}

bool CGeminiManager::IsEnabled() const
{
	return m_config.bEnabled && !m_config.strApiKey.IsEmpty();
}

// ========================================
// HTTP 요청 전송
// ========================================
CString CGeminiManager::SendHttpRequest(const CString& strUrl, const CString& strJsonBody)
{
	m_strLastError = _T("");
	CString strResponse;

	// URL 파싱
	URL_COMPONENTS urlComp;
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);

	WCHAR szHostName[256];
	WCHAR szUrlPath[1024];
	urlComp.lpszHostName = szHostName;
	urlComp.dwHostNameLength = sizeof(szHostName) / sizeof(WCHAR);
	urlComp.lpszUrlPath = szUrlPath;
	urlComp.dwUrlPathLength = sizeof(szUrlPath) / sizeof(WCHAR);

	if (!WinHttpCrackUrl(strUrl, strUrl.GetLength(), 0, &urlComp))
	{
		m_strLastError = _T("Failed to parse URL");
		return _T("");
	}

	// WinHTTP 세션 초기화
	HINTERNET hSession = WinHttpOpen(
		L"InventoryManager/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0
	);

	if (!hSession)
	{
		m_strLastError = _T("Failed to initialize WinHTTP session");
		return _T("");
	}

	// 연결 생성
	HINTERNET hConnect = WinHttpConnect(
		hSession,
		urlComp.lpszHostName,
		urlComp.nPort,
		0
	);

	if (!hConnect)
	{
		m_strLastError = _T("Failed to connect to server");
		WinHttpCloseHandle(hSession);
		return _T("");
	}

	// 요청 생성
	HINTERNET hRequest = WinHttpOpenRequest(
		hConnect,
		L"POST",
		urlComp.lpszUrlPath,
		NULL,
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		WINHTTP_FLAG_SECURE
	);

	if (!hRequest)
	{
		m_strLastError = _T("Failed to create request");
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return _T("");
	}

	// 헤더 설정
	CString strHeaders = _T("Content-Type: application/json\r\n");
	WinHttpAddRequestHeaders(
		hRequest,
		strHeaders,
		strHeaders.GetLength(),
		WINHTTP_ADDREQ_FLAG_ADD
	);

	// UTF-8로 변환
	int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, strJsonBody, -1, NULL, 0, NULL, NULL);
	char* pUtf8Data = new char[nUtf8Len];
	WideCharToMultiByte(CP_UTF8, 0, strJsonBody, -1, pUtf8Data, nUtf8Len, NULL, NULL);

	// 요청 전송
	BOOL bResult = WinHttpSendRequest(
		hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0,
		pUtf8Data,
		strlen(pUtf8Data),
		strlen(pUtf8Data),
		0
	);

	delete[] pUtf8Data;

	if (bResult)
	{
		bResult = WinHttpReceiveResponse(hRequest, NULL);
	}

	if (bResult)
	{
		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		std::string responseData;

		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				m_strLastError = _T("Error in WinHttpQueryDataAvailable");
				break;
			}

			char* pszOutBuffer = new char[dwSize + 1];
			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				delete[] pszOutBuffer;
				m_strLastError = _T("Error in WinHttpReadData");
				break;
			}

			responseData.append(pszOutBuffer, dwDownloaded);
			delete[] pszOutBuffer;

		} while (dwSize > 0);

		// UTF-8에서 유니코드로 변환
		int nWideLen = MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, NULL, 0);
		WCHAR* pWideStr = new WCHAR[nWideLen];
		MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, pWideStr, nWideLen);
		strResponse = pWideStr;
		delete[] pWideStr;
	}
	else
	{
		m_strLastError = _T("Failed to send request");
	}

	// 정리
	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);

	return strResponse;
}

// ========================================
// 요청 본문 생성
// ========================================
CString CGeminiManager::BuildRequestBody(const CString& strPrompt)
{
	CString strEscapedPrompt = JsonEscape(strPrompt);
	
	CString strBody;
	strBody.Format(
		_T("{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}"),
		strEscapedPrompt
	);

	return strBody;
}

// ========================================
// 응답 파싱
// ========================================
CString CGeminiManager::ParseResponse(const CString& strJsonResponse)
{
	// 간단한 JSON 파싱 (실제로는 더 견고한 JSON 파서 사용 권장)
	int nStart = strJsonResponse.Find(_T("\"text\":"));
	if (nStart == -1)
	{
		m_strLastError = _T("Failed to parse response");
		return _T("");
	}

	nStart = strJsonResponse.Find(_T("\""), nStart + 7);
	if (nStart == -1)
	{
		m_strLastError = _T("Failed to parse response");
		return _T("");
	}

	int nEnd = strJsonResponse.Find(_T("\""), nStart + 1);
	if (nEnd == -1)
	{
		m_strLastError = _T("Failed to parse response");
		return _T("");
	}

	CString strResult = strJsonResponse.Mid(nStart + 1, nEnd - nStart - 1);
	
	// JSON 이스케이프 문자 처리
	strResult.Replace(_T("\\n"), _T("\n"));
	strResult.Replace(_T("\\r"), _T("\r"));
	strResult.Replace(_T("\\t"), _T("\t"));
	strResult.Replace(_T("\\\""), _T("\""));
	strResult.Replace(_T("\\\\"), _T("\\"));

	return strResult;
}

// ========================================
// AI 기능: 제품 설명 생성
// ========================================
CString CGeminiManager::GenerateProductDescription(const CString& strProductName, const CString& strBrand, const CString& strCategory)
{
	if (!IsEnabled())
	{
		m_strLastError = _T("Gemini is not enabled or API key is missing");
		return _T("");
	}

	CString strPrompt;
	strPrompt.Format(
		_T("Generate a concise and professional product description for an inventory system. Product Name: %s, Brand: %s, Category: %s. Write in Korean. Keep it under 100 words."),
		strProductName, strBrand, strCategory
	);

	CString strRequestBody = BuildRequestBody(strPrompt);
	
	// API 엔드포인트 생성
	CString strUrl;
	strUrl.Format(
		_T("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s"),
		m_config.strModel, m_config.strApiKey
	);

	CString strResponse = SendHttpRequest(strUrl, strRequestBody);
	if (strResponse.IsEmpty())
	{
		return _T("");
	}

	return ParseResponse(strResponse);
}

// ========================================
// AI 기능: 재고 인사이트
// ========================================
CString CGeminiManager::GetInventoryInsight(const std::vector<CString>& vecLowStockItems)
{
	if (!IsEnabled())
	{
		m_strLastError = _T("Gemini is not enabled or API key is missing");
		return _T("");
	}

	if (vecLowStockItems.empty())
	{
		return _T("모든 재고가 안전한 수준입니다.");
	}

	CString strItemList;
	for (size_t i = 0; i < vecLowStockItems.size() && i < 10; i++)
	{
		strItemList += vecLowStockItems[i];
		if (i < vecLowStockItems.size() - 1)
		{
			strItemList += _T(", ");
		}
	}

	CString strPrompt;
	strPrompt.Format(
		_T("다음은 재고가 부족한 상품 목록입니다: %s. 이 상품들의 재고 상황을 분석하고 재주문 우선순위와 권장 사항을 한국어로 간단히 제시해주세요. 200자 이내로 작성해주세요."),
		strItemList
	);

	CString strRequestBody = BuildRequestBody(strPrompt);
	
	CString strUrl;
	strUrl.Format(
		_T("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s"),
		m_config.strModel, m_config.strApiKey
	);

	CString strResponse = SendHttpRequest(strUrl, strRequestBody);
	if (strResponse.IsEmpty())
	{
		return _T("");
	}

	return ParseResponse(strResponse);
}

// ========================================
// AI 기능: 재주문 수량 제안
// ========================================
CString CGeminiManager::SuggestReorderQuantity(const CString& strProductName, int nCurrentStock, int nAverageSales)
{
	if (!IsEnabled())
	{
		m_strLastError = _T("Gemini is not enabled or API key is missing");
		return _T("");
	}

	CString strPrompt;
	strPrompt.Format(
		_T("상품명: %s, 현재 재고: %d개, 평균 판매량: %d개/월. 이 정보를 바탕으로 적정 재주문 수량을 추천해주세요. 한국어로 간단히 답변해주세요."),
		strProductName, nCurrentStock, nAverageSales
	);

	CString strRequestBody = BuildRequestBody(strPrompt);
	
	CString strUrl;
	strUrl.Format(
		_T("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s"),
		m_config.strModel, m_config.strApiKey
	);

	CString strResponse = SendHttpRequest(strUrl, strRequestBody);
	if (strResponse.IsEmpty())
	{
		return _T("");
	}

	return ParseResponse(strResponse);
}

// ========================================
// AI 기능: 카테고리 분석
// ========================================
CString CGeminiManager::AnalyzeCategory(const CString& strCategoryName, int nTotalProducts, int nTotalStock)
{
	if (!IsEnabled())
	{
		m_strLastError = _T("Gemini is not enabled or API key is missing");
		return _T("");
	}

	CString strPrompt;
	strPrompt.Format(
		_T("카테고리: %s, 총 제품 수: %d개, 총 재고: %d개. 이 카테고리의 재고 상황을 분석하고 인사이트를 한국어로 제공해주세요. 150자 이내로 작성해주세요."),
		strCategoryName, nTotalProducts, nTotalStock
	);

	CString strRequestBody = BuildRequestBody(strPrompt);
	
	CString strUrl;
	strUrl.Format(
		_T("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s"),
		m_config.strModel, m_config.strApiKey
	);

	CString strResponse = SendHttpRequest(strUrl, strRequestBody);
	if (strResponse.IsEmpty())
	{
		return _T("");
	}

	return ParseResponse(strResponse);
}

// ========================================
// 오류 처리
// ========================================
CString CGeminiManager::GetLastError() const
{
	return m_strLastError;
}

// ========================================
// 유틸리티: JSON 이스케이프
// ========================================
CString CGeminiManager::JsonEscape(const CString& strInput)
{
	CString strResult = strInput;
	strResult.Replace(_T("\\"), _T("\\\\"));
	strResult.Replace(_T("\""), _T("\\\""));
	strResult.Replace(_T("\n"), _T("\\n"));
	strResult.Replace(_T("\r"), _T("\\r"));
	strResult.Replace(_T("\t"), _T("\\t"));
	return strResult;
}

// ========================================
// 유틸리티: URL 인코딩
// ========================================
CString CGeminiManager::UrlEncode(const CString& strInput)
{
	// 기본 URL 인코딩 구현
	CString strResult;
	for (int i = 0; i < strInput.GetLength(); i++)
	{
		WCHAR ch = strInput[i];
		if ((ch >= L'A' && ch <= L'Z') ||
			(ch >= L'a' && ch <= L'z') ||
			(ch >= L'0' && ch <= L'9') ||
			ch == L'-' || ch == L'_' || ch == L'.' || ch == L'~')
		{
			strResult += ch;
		}
		else
		{
			CString strHex;
			strHex.Format(_T("%%%02X"), (unsigned char)ch);
			strResult += strHex;
		}
	}
	return strResult;
}
