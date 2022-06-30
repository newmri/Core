#pragma once

class AccountDB : public CoreAccountDB
{
public:
	AccountDB(std::wstring_view dbName);
	virtual ~AccountDB() override;

public:
	virtual void Logout(const CoreAccount* account) override;
};