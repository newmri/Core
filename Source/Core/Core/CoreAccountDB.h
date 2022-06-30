#pragma once

class CoreAccountDB : public CoreDB
{
public:
	CoreAccountDB(std::wstring_view dbName);
	virtual ~CoreAccountDB() override;

public:
	bool Login(const int64_t accountUID, CoreToken& token);
	virtual void Logout(const CoreAccount* account) CORE_PURE;
};