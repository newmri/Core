#include "CoreInclude.h"

CoreDB::CoreDB()
{
	
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
		CORE_LOG.Log(LogType::LOG_CONNECT, "Connected");
	}
	else
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "Can't Connect");
		abort();
	}

}

void CoreDB::SetDBName(std::wstring_view dbName)
{
	this->dbName = dbName;
}

bool CoreDB::Connect(void)
{
	// Allocate environment handle  
	this->retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->henv);

	// Set the ODBC version environment attribute  
	if (IsSuccess())
	{
		this->retCode = SQLSetEnvAttr(this->henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER*>(SQL_OV_ODBC3), 0);

		// Allocate connection handle  
		if (IsSuccess())
		{
			this->retCode = SQLAllocHandle(SQL_HANDLE_DBC, this->henv, &this->hdbc);

			// Set login timeout to 5 seconds  
			if (IsSuccess())
			{
				SQLSetConnectAttr(this->hdbc, SQL_LOGIN_TIMEOUT, reinterpret_cast<SQLPOINTER>(5), 0);

				this->retCode = SQLConnect(this->hdbc, const_cast<SQLWCHAR*>(this->dbName.c_str()), SQL_NTS, NULL, 0, NULL, 0);

				// Allocate statement handle  
				return IsSuccess();
			}
		}
	}

	return false;
}

bool CoreDB::IsSuccess(void)
{
	return(IS_SAME(SQL_SUCCESS, this->retCode) || IS_SAME(SQL_SUCCESS_WITH_INFO, this->retCode));
}

void CoreDB::Prepare(SQLWCHAR* spName)
{
	SQLAllocHandle(SQL_HANDLE_STMT, this->hdbc, &this->hstmt);

	this->command = L"EXCE ";
	this->command += spName + L' ';
}

template<typename T>
void CoreDB::BindCol(T& data, const size_t size)
{
	SQLUSMALLINT sqlType = CORE_DATA_TYPE_MANAGER.GetSQLType(typeid(T).name());
	if (IS_SAME(SQL_UNKNOWN_TYPE, sqlType))
	{
		std::string err = CORE_LOG.MakeLog(LogType::LOG_ERROR, "DB ERROR", __FILE__, __FUNCTION__, __LINE__);
		CORE_LOG.Log(err);
		abort();
	}

	this->retCode = SQLBindCol(this->hstmt, this->currIndex + 1,
			    sqlType, data, size, this->colLen[this->currIndex]);

	++this->currIndex;
}

template<typename T>
void CoreDB::BindArgument(T& data)
{

}

bool CoreDB::Execute(void)
{
	this->retCode = SQLExecDirect(this->hstmt, const_cast<SQLWCHAR*>(this->command.c_str()), SQL_NTS);
	return IsSuccess();
}

void CoreDB::Release(void)
{
	if (IsSuccess())
	{
		SQLCancel(this->hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, this->hstmt);
	}

	SQLDisconnect(this->hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, this->hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
}