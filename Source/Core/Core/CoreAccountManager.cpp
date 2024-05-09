#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreAccountManager)

void CoreAccountManager::Init(void)
{
	
}

void CoreAccountManager::Release(void)
{
	GetInstance().~CoreAccountManager();
}

std::shared_ptr<CoreAccount> CoreAccountManager::Find(const int64_t& uid)
{
	READ_LOCK(this->mutex);
	
	auto iter = this->accountList.find(uid);
	if (IS_NOT_SAME(iter, this->accountList.end()))
		return iter->second;

	return nullptr;
}

std::shared_ptr<CoreAccount> CoreAccountManager::Add(const int64_t& uid, const CoreToken& token)
{
	auto account = std::make_shared<CoreAccount>(uid, token);
	account->Init();

	WRITE_LOCK(this->mutex);
	this->accountList[uid] = account;
	return account;
}

void CoreAccountManager::SetLogout(const int64_t& uid)
{
	auto account = Find(uid);

	if (IS_NULL(account))
		return;

	account->SetLogout();
	this->logoutFunc(account);
}

void CoreAccountManager::SetLogoutFunc(std::function<void(std::shared_ptr<CoreAccount>)> func)
{
	this->logoutFunc = func;
}