#pragma once

struct CoreToken
{
	CoreToken() {}
	CoreToken(const int32_t key, const TIME_VALUE expireTime = 0) : key(key), expireTime(expireTime)
	{

	}

	int32_t key = 0;
	TIME_VALUE expireTime = 0;
};

class CoreAccount : public CoreUIDObject, public CoreMemoryPoolObj<CoreAccount, CORE_BIG_SIZE>
{
	OVERRIDE_OBJECT_DELETE_DEFAULT_CONSTRUCTOR(CoreAccount)

public:
	CoreAccount(const int64_t& uid, const CoreToken& token);

public:
	bool IsTokenExpired(void);
	void UpdateToken(const CoreToken& token);

public:
	bool IsLogined(void);
	void SetLogin(void);
	void SetLogout(void);

public:
	uint8_t GetMaxSlotCount(void);
	void SetMaxSlotCount(const uint8_t maxCharacterSlotCount);
	bool CanCreateCharacter(void);

public:
	std::shared_ptr<CoreCharacter> GetCharacter(const int64_t& uid);
	void GetCharacter(CoreVector<std::shared_ptr<CoreCharacter>>& characterList);

public:
	bool HaveCharacter(const int64_t& uid);

public:
	void AddCharacter(std::shared_ptr<CoreCharacter> character);
	void DeleteCharacter(const int64_t& uid);

public:
	void SetPlayerOID(const int64_t& oid);
	int64_t GetPlayerOID(void) const;

public:
	void PushMoney(const int32_t money);
	void AddMoney(const uint8_t index, const int32_t money);
	bool UseMoney(const uint8_t index, const int32_t money);

private:
	void Release(void);

private:
	CoreToken token;
	bool isLogined;

private:
	CACHE_ALIGN std::shared_mutex characterMutex;
	uint8_t maxCharacterSlotCount;
	std::unordered_map<int64_t, std::shared_ptr<CoreCharacter>> characterList;
	CoreVector<int32_t> moneyList;

private:
	int64_t playerOID = 0;
};