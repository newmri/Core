#pragma once

#define CORE_ACCOUNT_MANAGER GET_INSTANCE(CoreAccountManager)

class CoreAccountManager
{
	DECLARE_SINGLETON(CoreAccountManager)

public:
	std::shared_ptr<CoreAccount> Find(const int64_t& uid);
	std::shared_ptr<CoreAccount> Add(const int64_t& uid, const CoreToken& token);
	void SetLogout(const int64_t& uid);

public:
	void SetLogoutFunc(std::function<void(std::shared_ptr<CoreAccount>)> func);

private:
	CACHE_ALIGN std::shared_mutex mutex;
	std::unordered_map<int64_t, std::shared_ptr<CoreAccount>> accountList;

private:
	std::function<void(std::shared_ptr<CoreAccount>)> logoutFunc;
};