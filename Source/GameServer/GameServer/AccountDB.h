#pragma once

class AccountDB : public CoreDB
{
public:
	AccountDB(std::wstring_view dbName);
	virtual ~AccountDB() override;

public:
	virtual void Release(void) override;

public:
	bool Login(const int64_t uid, CoreToken& token);
	void Logout(const int64_t uid);

public:
	void LoadMoney(const int64_t uid, int32_t money[Define::Money_MONEY_END]);
};