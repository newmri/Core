#pragma once

class AccountDB : public CoreAccountDB
{
public:
	AccountDB() = default;
	AccountDB(DBInfo&& dbInfo);
	virtual ~AccountDB() override;

public:
	virtual void Logout(std::shared_ptr<CoreAccount> account) override;
};