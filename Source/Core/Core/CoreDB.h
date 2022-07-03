#pragma once

struct DBConfig
{
	std::string Name;
	std::string IP;
};

enum DBType
{
	World = 0,
	Account,
	Game
};

class CoreDB : CoreObject
{
	OVERRIDE_OBJECT(CoreDB)

public:
	CoreDB(std::wstring_view dbName);

protected:
	bool Connect(void);

protected:
	bool IsSuccess(void);
	bool IsNoData(void);

protected:
	void Prepare(const SQLWCHAR* spName);

protected:
	void BindCol(bool* data, const SQLLEN size);
	void BindCol(uint8_t* data, const SQLLEN size);
	void BindCol(int16_t* data, const SQLLEN size);
	void BindCol(uint16_t* data, const SQLLEN size);
	void BindCol(int32_t* data, const SQLLEN size);
	void BindCol(int64_t* data, const SQLLEN size);
	void BindCol(wchar_t** data, const SQLLEN size);
	
protected:
	void BindArgument(const uint8_t data);
	void BindArgument(const int8_t data);
	void BindArgument(const int16_t data);
	void BindArgument(const int32_t data);
	void BindArgument(const int64_t data);
	void BindArgument(const wchar_t* data);

protected:
	bool Execute(void);

private:
	void Release(void);

protected:
	std::wstring dbName;

private:
	std::wstring command;

protected:
	SQLRETURN retCode;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;

private:
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLSMALLINT currIndex = 0;
	SQLLEN colLen[CORE_SMALL_SIZE];
};