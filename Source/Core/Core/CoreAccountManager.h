#pragma once

#define CORE_ACCOUNT_MANAGER GET_INSTANCE(CoreAccountManager)

class CoreAccountManager
{
	DECLARE_SINGLETON(CoreAccountManager)

public:
	CoreAccount* Find(const int64_t& uid);
	CoreAccount* Add(const int64_t& uid, const CoreToken& token);
	void SetLogout(const int64_t& uid);

public:
	void SetLogoutFunc(std::function<void(const int64_t)> func);

private:
	CACHE_ALIGN std::shared_mutex mutex;
	std::map<int64_t, CoreAccount*> accountList;

private:
	std::function<void(const int64_t)> logoutFunc;
};