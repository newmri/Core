#pragma once

class CoreWorldDB : public CoreDB
{
public:
	CoreWorldDB(std::wstring_view dbName);
	virtual ~CoreWorldDB() override;

public:
	void SetID(const uint8_t worldID, const uint8_t serverID);
	void GetServerInfo(ServerInfo& serverInfo);

public:
	void IncreaseUserCount(void);

private:
	uint8_t worldID = 0;
	uint8_t serverID = 0;
};