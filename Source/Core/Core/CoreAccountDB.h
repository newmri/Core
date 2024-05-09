#pragma once

class CoreAccountDB : public CoreDB
{
public:
	CoreAccountDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID);
	virtual ~CoreAccountDB() override;

public:
	bool Login(const int64_t accountUID, CoreToken& token);
	virtual void Logout(std::shared_ptr<CoreAccount> account) CORE_PURE;
};