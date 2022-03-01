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
	Release();
}

size_t CoreAccount::GetCharacterCount(void)
{
	READ_LOCK(this->characterMutex);
	return this->characterList.size();
}

std::shared_ptr<CoreCharacter> CoreAccount::GetCharacter(const int64_t& uid)
{
	READ_LOCK(this->characterMutex);

	auto iter = this->characterList.find(uid);
	if (IS_NOT_SAME(iter, this->characterList.end()))
		return iter->second->shared_from_this();

	return nullptr;
}

void CoreAccount::GetCharacter(CoreVector<std::shared_ptr<CoreCharacter>>& characterList)
{
	READ_LOCK(this->characterMutex);

	characterList.reserve(this->characterList.size());

	auto iter_begin = this->characterList.cbegin();
	auto iter_end = this->characterList.cend();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		characterList.push_back(iter_begin->second->shared_from_this());
	}
}

void CoreAccount::AddCharacter(std::shared_ptr<CoreCharacter> character)
{
	WRITE_LOCK(this->characterMutex);
	this->characterList[character->GetUID()] = character;
}

void CoreAccount::Release(void)
{
	WRITE_LOCK(this->characterMutex);

	auto iter_begin = this->characterList.begin();
	auto iter_end = this->characterList.end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		iter_begin = this->characterList.erase(iter_begin);
	}

	characterList.clear();
}