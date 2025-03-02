#include "Include.h"
#include "boost/current_function.hpp"

AccountDB::AccountDB(DBInfo&& dbInfo) : CoreAccountDB(std::move(dbInfo))
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
