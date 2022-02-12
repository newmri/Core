#pragma once

struct DBConfig
{
	std::string Name;
	std::string IP;
};

class CoreDB : CoreObj
{
	OVERRIDE_OBJ(CoreDB)

public:
	CoreDB(std::wstring_view dbName);

public:
	void SetDBName(std::wstring_view dbName);

public:
	bool Connect(void);

protected:
	bool IsSuccess(void);
	void Prepare(SQLWCHAR* spName);
	template<typename T>
	void BindCol(T& data, const size_t size);
	template<typename T>
	void BindArgument(T& data);
	bool Execute(void);
	void Release(void);

protected:
	std::wstring dbName;

private:
	std::wstring command;

private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retCode;
	size_t currIndex = 0;
	SQLLEN colLen[10];
};