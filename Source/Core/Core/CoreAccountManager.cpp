#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreAccountManager)

void CoreAccountManager::Init(void)
{
	
}

void CoreAccountManager::Release(void)
{
	GetInstance().~CoreAccountManager();
}

CoreAccount* CoreAccountManager::Find(const int64_t& uid)
{
	READ_LOCK(this->mutex);
	
	auto iter = this->accountList.find(uid);
	if (IS_NOT_SAME(iter, this->accountList.end()))
		return iter->second;

	return nullptr;
}

CoreAccount* CoreAccountManager::Add(const int64_t& uid, const CoreToken& token)
{
	CoreAccount* account = new CoreAccount(uid, token);

	WRITE_LOCK(this->mutex);
	this->accountList[uid] = account;
	return account;
}

void CoreAccountManager::SetLogout(const int64_t& uid)
{
	CoreAccount* account = Find(uid);

	if (IS_NULL(account))
		return;

	account->SetLogout();
	this->logoutFunc(account);
}

void CoreAccountManager::SetLogoutFunc(std::function<void(const CoreAccount*)> func)
{
	this->logoutFunc = func;
}