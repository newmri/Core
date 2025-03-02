#pragma once

class CoreAccount;

class CoreAccountDB : public CoreDB
{
public:
	CoreAccountDB() = default;
	CoreAccountDB(DBInfo&& dbInfo);
	virtual ~CoreAccountDB() override;

public:
	virtual void Init(void) override;

public:
	bool Login(const int64_t accountUID, CoreToken& token);
	void SetLogin(const int64_t accountUID);
	virtual void Logout(std::shared_ptr<CoreAccount> account);
};