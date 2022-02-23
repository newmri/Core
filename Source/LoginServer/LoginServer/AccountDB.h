#pragma once

class AccountDB : public CoreDB
{
public:
	AccountDB(std::wstring_view dbName);
	virtual ~AccountDB() override;

public:
	virtual void Release(void) override;

public:
	bool Login(const int64_t uid, const int32_t token);

};