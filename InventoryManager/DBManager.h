#pragma once

#include <mysql.h>
#include <vector>

struct DB_CONFIG
{
	CString strHost;
	int nPort;
	CString strDatabase;
	CString strUser;
	CString strPassword;

	DB_CONFIG()
	{
		strHost = _T("192.168.0.92");
		nPort = 3306;
		strDatabase = _T("themost_db");
		strUser = _T("mfcuser");
		strPassword = _T("Moble1234");
	}
};

struct INVENTORY_ITEM
{
	CString strOptionCode;
	CString strProductName;
	CString strBrandName;
	CString strCategoryName;
	CString strColorName;
	CString strSizeName;
	int nStock;
	int nOptionID;
	int nProductID;

	INVENTORY_ITEM()
	{
		nStock = 0;
		nOptionID = 0;
		nProductID = 0;
	}
};

class CDBManager
{
private:
	static CDBManager* m_pInstance;
	CDBManager();
	~CDBManager();

	MYSQL* m_pConnection;
	MYSQL_RES* m_pResult;

	DB_CONFIG m_config;
	BOOL m_bConnected;

	CString m_strLastError;
	int m_nLastErrorCode;

public:
	static CDBManager* GetInstance();
	static void DestroyInstance();
	BOOL SelectToRows(const CString& sql, std::vector<std::vector<CString>>& rows);

	BOOL Connect(const DB_CONFIG& config);
	void Disconnect();
	BOOL IsConnected() const;
	BOOL Reconnect();

	BOOL ExecuteQuery(const CString& strQuery);
	BOOL ExecuteSelect(const CString& strQuery);

	MYSQL_ROW FetchRow();
	int GetRowCount();
	void FreeResult();

	BOOL GetInventoryList(std::vector<INVENTORY_ITEM>& vecItems);
	BOOL UpdateStock(int nOptionID, int nNewStock);
	BOOL AddStock(int nOptionID, int nQuantity);
	BOOL AddStockToItems(const std::vector<int>& vecOptionIDs, int nQuantity);

	BOOL DeleteOptionsAndCleanup(const std::vector<int>& vecOptionIDs);

	CString GetLastError() const;
	int GetLastErrorCode() const;

	CString EscapeString(const CString& strInput);

	CString GetBrandCodeFromName(const CString& strName);
	CString GetCategoryCodeFromName(const CString& strName);
	CString GetColorCodeFromName(const CString& strName);
	BOOL GetBrandList(std::vector<CString>& vecBrands);
	BOOL GetColorList(std::vector<CString>& vecColors);
	BOOL GetSizeList(std::vector<CString>& vecSizes);

	BOOL AddProduct(
		const CString& strBrandName,
		const CString& strCategoryName,
		const CString& strProductName,
		int nPrice,
		const CString& strDescription,
		const CString& strImagesJson,
		const CString& strOptionsJson
	);

	BOOL GetCategoryList(std::vector<CString>& vecCategories);

private:
	void SetError(const CString& strError, int nCode = 0);
	CString ConvertToUTF8(const CString& strInput);
	CString ConvertFromUTF8(const char* szInput);
};
