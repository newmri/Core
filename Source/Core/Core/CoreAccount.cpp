#include "CoreInclude.h"

CoreAccount::~CoreAccount()
{

}

void CoreAccount::Init(void)
{
	CoreUIDObject::Init();
}

CoreAccount::CoreAccount(const int64_t& uid, const CoreToken& token) : CoreUIDObject(uid), token(token), isLogined(true), maxCharacterSlotCount(0)
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

uint8_t CoreAccount::GetMaxSlotCount(void)
{
	READ_LOCK(this->characterMutex);
	return this->maxCharacterSlotCount;
}

void CoreAccount::SetMaxSlotCount(const uint8_t maxCharacterSlotCount)
{
	WRITE_LOCK(this->characterMutex);
	this->maxCharacterSlotCount = maxCharacterSlotCount;
}

bool CoreAccount::CanCreateCharacter(void)
{
	READ_LOCK(this->characterMutex);
	return (this->maxCharacterSlotCount > this->characterList.size());
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

bool CoreAccount::HaveCharacter(const int64_t& uid)
{
	READ_LOCK(this->characterMutex);
	auto iter = this->characterList.find(uid);
	return (IS_NOT_SAME(iter, this->characterList.end()));
}

void CoreAccount::AddCharacter(std::shared_ptr<CoreCharacter> character)
{
	WRITE_LOCK(this->characterMutex);
	this->characterList[character->GetUID()] = character;
}

void CoreAccount::DeleteCharacter(const int64_t& uid)
{
	WRITE_LOCK(this->characterMutex);
	this->characterList.erase(uid);
}

void CoreAccount::SetPlayerOID(const int64_t& oid)
{
	this->playerOID = oid;
}

int64_t CoreAccount::GetPlayerOID(void) const
{
	return this->playerOID;
}

void CoreAccount::PushMoney(const int32_t money)
{
	this->moneyList.push_back(money);
}

void CoreAccount::AddMoney(const uint8_t index, const int32_t money)
{
	this->moneyList[index] += money;
}

bool CoreAccount::UseMoney(const uint8_t index, const int32_t money)
{
	if (this->moneyList[index] < money)
		return false;

	AddMoney(index, -money);
	return true;
}

void CoreAccount::Release(void)
{
	WRITE_LOCK(this->characterMutex);

	if (IS_SAME(0, this->characterList.size()))
		return;

	characterList.clear();
}