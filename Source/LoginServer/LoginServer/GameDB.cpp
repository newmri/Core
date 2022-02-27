#include "Include.h"
#include "boost/current_function.hpp"

GameDB::GameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

GameDB::~GameDB()
{
	Release();
}

void GameDB::Release(void)
{
}

Login::ErrorCode GameDB::CreateCharacter(const wchar_t* characterName, Define::Job job, int64_t& uid)
{
	Prepare(L"CreateCharacter");
	BindArgument(characterName);
	BindArgument(job);
	Execute();

	int8_t result = 0;
	Login::ErrorCode errorCode = Login::ErrorCode_UNKNOWN;
	int64_t characterUID = 0;

	BindCol(&result, sizeof(result));
	BindCol(&characterUID, sizeof(characterUID));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);

		if (IsSuccess())
		{
			errorCode = static_cast<Login::ErrorCode>(result);
			uid = characterUID;
		}
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return errorCode;
}