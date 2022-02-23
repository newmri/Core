#include "CoreInclude.h"

CoreAccount::CoreAccount(const int64_t& uid, const CoreToken& token) : uid(uid), token(token), isLogined(true)
{
}

bool CoreAccount::IsTokenExpired(void)
{
	return CORE_TIME_MANAGER.IsExpired(this->token.expireTime);
}

void CoreAccount::UpdateToken(const CoreToken& token)
{
	this->token = token;
}

bool CoreAccount::IsLogined(void)
{
	return this->isLogined;
}

void CoreAccount::SetLogin(void)
{
	this->isLogined = true;
}

void CoreAccount::SetLogout(void)
{
	this->isLogined = false;
}