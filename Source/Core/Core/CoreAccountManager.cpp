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

void CoreAccountManager::Add(const int64_t& uid, const CoreToken& token)
{
	WRITE_LOCK(this->mutex);
	this->accountList[uid] = new CoreAccount(uid, token);
}

void CoreAccountManager::SetLogout(const int64_t& uid)
{
	CoreAccount* account = Find(uid);

	if (IS_NULL(account))
		return;

	account->SetLogout();
}