#include "CoreInclude.h"

CoreGameDB::CoreGameDB(std::wstring_view dbName) : CoreDB(dbName)
{

}

CoreGameDB::~CoreGameDB()
{

}

CoreItemUID CoreGameDB::GetItemUID(const uint8_t serverID)
{
	Prepare(L"GetItemUID");
	BindArgument(serverID);
	Execute();

	CoreItemUID itemUID;
	BindCol(&itemUID.uid, sizeof(itemUID.uid));

	while (IsSuccess())
	{
		this->retCode = SQLFetch(this->hstmt);
	};

	SQLFreeStmt(this->hstmt, SQL_CLOSE);

	return itemUID;
}
