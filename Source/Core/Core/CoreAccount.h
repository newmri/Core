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

class CoreAccount : public CoreMemoryPoolObj<CoreAccount, CORE_BIG_SIZE>
{
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
	size_t GetCharacterCount(void);
	std::shared_ptr<CoreCharacter> GetCharacter(const int64_t& uid);
	void GetCharacter(CoreVector<std::shared_ptr<CoreCharacter>>& characterList);

	void AddCharacter(std::shared_ptr<CoreCharacter> character);

private:
	void Release(void);

private:
	int64_t uid;
	CoreToken token;
	bool isLogined;

private:
	CACHE_ALIGN std::shared_mutex characterMutex;
	std::map<int64_t, std::shared_ptr<CoreCharacter>> characterList;
};