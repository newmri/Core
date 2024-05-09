#pragma once

class AccountDB : public CoreAccountDB
{
public:
	AccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID);
	virtual ~AccountDB() override;

public:
	virtual void Logout(std::shared_ptr<CoreAccount> account) override;

public:
	void LoadMoney(const int64_t uid, NativeInfo::Money& money);
};