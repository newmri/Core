#include "CoreInclude.h"

CoreDB::CoreDB()
{
	abort();
}

CoreDB::CoreDB(std::wstring_view dbName) : dbName(dbName)
{
	Init();
}

CoreDB::~CoreDB()
{
	Release();
}

void CoreDB::Init(void)
{
	this->command.reserve(CORE_BIG_SIZE);

	if (Connect())
	{
		CORE_LOG.Log(LogType::LOG_CONNECT, STRING_MANAGER.Narrow(this->dbName) + "DB is Connected");
	}
	else
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "Can't Connect");
		abort();
	}

}

bool CoreDB::Connect(void)
{
	// Allocate environment handle  
	this->retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->henv);

	// Set the ODBC version environment attribute  
	if (IsSuccess())
	{
		this->retCode = SQLSetEnvAttr(this->henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER*>(SQL_OV_ODBC3), SQL_IS_INTEGER);

		// Allocate connection handle  
		if (IsSuccess())
		{
			this->retCode = SQLAllocHandle(SQL_HANDLE_DBC, this->henv, &this->hdbc);

			// Set login timeout to 5 seconds  
			if (IsSuccess())
			{
				SQLSetConnectAttr(this->hdbc, SQL_LOGIN_TIMEOUT, reinterpret_cast<SQLPOINTER>(5), 0);

				this->retCode = SQLConnect(this->hdbc, const_cast<SQLWCHAR*>(this->dbName.c_str()), SQL_NTS, NULL, SQL_NTS, NULL, SQL_NTS);
				this->retCode = SQLAllocHandle(SQL_HANDLE_STMT, this->hdbc, &this->hstmt);

				// Allocate statement handle  
				return IsSuccess();
			}
		}
	}

	return false;
}

bool CoreDB::IsSuccess(void)
{
	return (IS_SAME(SQL_SUCCESS, this->retCode) || IS_SAME(SQL_SUCCESS_WITH_INFO, this->retCode));
}

bool CoreDB::IsNoData(void)
{
	return (IS_SAME(SQL_NO_DATA, this->retCode));
}

void CoreDB::Prepare(const SQLWCHAR* spName)
{
	this->currIndex = 0;

	this->command = L"EXEC ";
	this->command += L"dbo.";
	this->command += spName;
}

void CoreDB::BindCol(bool* data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_BIT, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindCol(uint8_t* data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_TINYINT, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindCol(int16_t* data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_SMALLINT, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindCol(int32_t* data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_INTEGER, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindCol(int64_t* data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_C_SBIGINT, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindCol(wchar_t** data, const SQLLEN size)
{
	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1, SQL_WCHAR, data, size, &this->colLen[this->currIndex]);
	++this->currIndex;
}

void CoreDB::BindArgument(const uint8_t data)
{
	this->command += L" ";
	this->command += TO_WSTR(data);
	this->command += L",";
}

void CoreDB::BindArgument(const int8_t data)
{
	this->command += L" ";
	this->command += TO_WSTR(data);
	this->command += L",";
}

void CoreDB::BindArgument(const int16_t data)
{
	this->command += L" ";
	this->command += TO_WSTR(data);
	this->command += L",";
}

void CoreDB::BindArgument(const int32_t data)
{
	this->command += L" ";
	this->command += TO_WSTR(data);
	this->command += L",";
}

void CoreDB::BindArgument(const int64_t data)
{
	this->command += L" ";
	this->command += TO_WSTR(data);
	this->command += L",";
}

void CoreDB::BindArgument(const wchar_t* data)
{
	this->command += L" N'";
	this->command += data;
	this->command += L"'";
	this->command += L",";
}

bool CoreDB::Execute(void)
{
	size_t index = this->command.length() - 1;
	this->command.replace(index, index, L"\0");

	this->retCode = SQLExecDirect(this->hstmt, const_cast<SQLWCHAR*>(this->command.c_str()), SQL_NTS);

	return IsSuccess();
}

void CoreDB::Release(void)
{
	if (this->hstmt)
	{
		if (IsSuccess())
			this->retCode = SQLCancel(this->hstmt);

		this->retCode = SQLFreeStmt(this->hstmt, SQL_CLOSE);
		this->retCode = SQLFreeHandle(SQL_HANDLE_STMT, this->hstmt);
	}

	if (this->hdbc)
	{
		this->retCode = SQLDisconnect(this->hdbc);
		this->retCode = SQLFreeHandle(SQL_HANDLE_DBC, this->hdbc);
	}

	if (this->henv)
	{
		this->retCode = SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
	}
}