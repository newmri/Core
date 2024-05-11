#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID) : CoreAccountDB(dbName, worldID, serverID)
{
	CORE_ACCOUNT_MANAGER.SetLogoutFunc(std::bind(&AccountDB::Logout, this, std::placeholders::_1));
}

AccountDB::~AccountDB()
{

}

void AccountDB::Logout(std::shared_ptr<CoreAccount> account)
{
	CoreAccountDB::Logout(account);
}
