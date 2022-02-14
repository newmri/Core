#pragma once

struct DBConfig
{
	std::string Name;
	std::string IP;
};

enum DBType
{
	World = 0,
	Account
};

class CoreDB : CoreObj
{
	OVERRIDE_OBJ(CoreDB)

public:
	CoreDB(std::wstring_view dbName);

protected:
	bool Connect(void);

protected:
	bool IsSuccess(void);

protected:
	void Prepare(const SQLWCHAR* spName);

protected:
	void BindCol(int* data, const SQLLEN size);
	void BindCol(const wchar_t* data, const SQLLEN size);
	
protected:
	void BindArgument(const wchar_t* data);
	void BindArgument(const int data);

protected:
	bool Execute(void);

private:
	virtual void Release(void);

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
	SQLLEN colLen[10];
};