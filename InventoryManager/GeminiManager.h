#pragma once

#include <string>
#include <vector>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

// ========================================
// Gemini API 설정 구조체
// ========================================
struct GEMINI_CONFIG
{
	CString strApiKey;          // Gemini API 키
	CString strModel;           // 사용할 모델 (예: gemini-pro)
	bool bEnabled;              // Gemini 기능 활성화 여부

	// 기본값 설정
	GEMINI_CONFIG()
	{
		strApiKey = _T("");
		strModel = _T("gemini-pro");
		bEnabled = false;
	}
};

// ========================================
// CGeminiManager 클래스 (싱글톤)
// ========================================
class CGeminiManager
{
private:
	// ========================================
	// 싱글톤 패턴
	// ========================================
	static CGeminiManager* m_pInstance;
	CGeminiManager();
	~CGeminiManager();

	// ========================================
	// 설정 정보
	// ========================================
	GEMINI_CONFIG m_config;
	CString m_strLastError;

	// ========================================
	// HTTP 요청 헬퍼
	// ========================================
	CString SendHttpRequest(const CString& strUrl, const CString& strJsonBody);
	CString BuildRequestBody(const CString& strPrompt);
	CString ParseResponse(const CString& strJsonResponse);

public:
	// ========================================
	// 싱글톤 인스턴스 관리
	// ========================================
	static CGeminiManager* GetInstance();
	static void DestroyInstance();

	// ========================================
	// 설정 관리
	// ========================================
	void SetConfig(const GEMINI_CONFIG& config);
	GEMINI_CONFIG GetConfig() const;
	bool IsEnabled() const;

	// ========================================
	// AI 기능
	// ========================================
	CString GenerateProductDescription(const CString& strProductName, const CString& strBrand, const CString& strCategory);
	CString GetInventoryInsight(const std::vector<CString>& vecLowStockItems);
	CString SuggestReorderQuantity(const CString& strProductName, int nCurrentStock, int nAverageSales);
	CString AnalyzeCategory(const CString& strCategoryName, int nTotalProducts, int nTotalStock);

	// ========================================
	// 오류 처리
	// ========================================
	CString GetLastError() const;

private:
	// ========================================
	// 유틸리티
	// ========================================
	CString UrlEncode(const CString& strInput);
	CString JsonEscape(const CString& strInput);
};
