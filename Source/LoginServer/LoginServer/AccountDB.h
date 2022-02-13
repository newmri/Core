#pragma once

enum DBType
{
	Account = 0
};

class AccountDB : CoreDB
{
public:
	AccountDB(std::wstring_view dbName);
	virtual ~AccountDB() override;

public:
	virtual void Release(void) override;

public:
	int Login(const wchar_t* ID, const wchar_t* password);

};