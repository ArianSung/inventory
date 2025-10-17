#pragma once

#include <mysql.h>
#include <vector>

// ========================================
// �����ͺ��̽� ���� ���� ����ü
// ========================================
struct DB_CONFIG
{
	CString strHost;        // ȣ��Ʈ (��: localhost, 192.168.1.100)
	int nPort;              // ��Ʈ (�⺻: 3306)
	CString strDatabase;    // �����ͺ��̽� �̸�
	CString strUser;        // ����� �̸�
	CString strPassword;    // ��й�ȣ

	// �⺻�� ����
	DB_CONFIG()
	{
		strHost = _T("192.168.0.92");
		nPort = 3306;
		strDatabase = _T("themost_db");
		strUser = _T("mfcuser");
		strPassword = _T("Moble1234");
	}
};

// ========================================
// ��� ������ ����ü
// ========================================
struct INVENTORY_ITEM
{
	CString strOptionCode;      // �ɼ� �ڵ� (��: SP-TS-001-BK-L)
	CString strProductName;     // ��ǰ��
	CString strBrandName;       // �귣���
	CString strCategoryName;	// ī�װ���
	CString strColorName;       // �����
	CString strSizeName;        // �������
	int nStock;                 // ��� ����
	int nOptionID;              // �ɼ� ID (PK)
	int nProductID;             // ��ǰ ID

	INVENTORY_ITEM()
	{
		nStock = 0;
		nOptionID = 0;
		nProductID = 0;
	}
};

// ========================================
// CDBManager Ŭ���� (�̱���)
// ========================================
class CDBManager
{
private:
	// ========================================
	// �̱��� ����
	// ========================================
	static CDBManager* m_pInstance;     // �̱��� �ν��Ͻ�
	CDBManager();                       // private ������
	~CDBManager();                      // private �Ҹ���

	// ========================================
	// MySQL ���� ��ü
	// ========================================
	MYSQL* m_pConnection;               // MySQL ���� �ڵ�
	MYSQL_RES* m_pResult;               // ���� ���

	// ========================================
	// ���� ����
	// ========================================
	DB_CONFIG m_config;                 // DB ����
	BOOL m_bConnected;                  // ���� ����

	// ========================================
	// ���� ����
	// ========================================
	CString m_strLastError;             // ������ ���� �޽���
	int m_nLastErrorCode;               // ������ ���� �ڵ�

public:
	// ========================================
	// �̱��� �ν��Ͻ� ����
	// ========================================
	static CDBManager* GetInstance();
	static void DestroyInstance();
	BOOL SelectToRows(const CString& sql, std::vector<std::vector<CString>>& rows);
	// ========================================
	// ���� ����
	// ========================================
	BOOL Connect(const DB_CONFIG& config);  // DB ����
	void Disconnect();                      // DB ���� ����
	BOOL IsConnected() const;               // ���� ���� Ȯ��
	BOOL Reconnect();                       // �翬��

	// ========================================
	// ���� ����
	// ========================================
	BOOL ExecuteQuery(const CString& strQuery);  // �Ϲ� ���� (INSERT, UPDATE, DELETE)
	BOOL ExecuteSelect(const CString& strQuery); // SELECT ����

	// ========================================
	// ��� ��������
	// ========================================
	MYSQL_ROW FetchRow();                   // �� �� ��������
	int GetRowCount();                      // ��� �� ��
	void FreeResult();                      // ��� ����

	// ========================================
	// ��� ������ ��ȸ
	// ========================================
	BOOL GetInventoryList(std::vector<INVENTORY_ITEM>& vecItems);  // ��ü ��� ��ȸ
	BOOL UpdateStock(int nOptionID, int nNewStock);                // ��� ����
	BOOL AddStock(int nOptionID, int nQuantity);                   // ��� �߰� (����)
	BOOL AddStockToItems(const std::vector<int>& vecOptionIDs, int nQuantity); // �ϰ� ����

	// ========================================
	// [����] ��ǰ �ɼ� ���� �� ��ǰ ����
	// ========================================
	BOOL DeleteOptionsAndCleanup(const std::vector<int>& vecOptionIDs);

	// ========================================
	// ���� ó��
	// ========================================
	CString GetLastError() const;           // ������ ���� �޽���
	int GetLastErrorCode() const;           // ������ ���� �ڵ�

	// ========================================
	// ��ƿ��Ƽ
	// ========================================
	CString EscapeString(const CString& strInput);  // SQL Injection ����

	//======================================
	//ǰ�� �߰�
	//======================================
	CString GetBrandCodeFromName(const CString& strName);
	CString GetCategoryCodeFromName(const CString& strName); // '����'�� ���� �Լ�
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

	// ��ǰ ���� ��ȸ (product_id��)
	BOOL GetProductInfo(int nProductID, CString& strProductName,CString& strBrandName, CString& strCategoryName);

	// �ɼ� �ߺ� üũ (���� product_id�� ���� color+size ������ �ִ���)
	BOOL CheckOptionExists(int nProductID, const CString& strColorName,const CString& strSizeName);

	// ��ǰ �ɼ� �߰�
	BOOL AddProductOption(int nProductID, const CString& strColorName,const CString& strSizeName, int nStock);

	BOOL GetSizeListByCategory(const CString& strCategoryName, std::vector<CString>& vecSizes);
private:
	// ========================================
	// ���� �Լ�
	// ========================================
	void SetError(const CString& strError, int nCode = 0);  // ���� ����
	CString ConvertToUTF8(const CString& strInput);         // UTF-8 ��ȯ
	CString ConvertFromUTF8(const char* szInput);           // UTF-8���� ��ȯ


};
