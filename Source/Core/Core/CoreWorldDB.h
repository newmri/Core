#pragma once

class CoreWorldDB : public CoreDB
{
public:
	CoreWorldDB(std::wstring_view dbName, const uint8_t worldID, const uint8_t serverID);
	virtual ~CoreWorldDB() override;

public:
	void GetServerInfo(ServerInfo& serverInfo);

public:
	void IncreaseUserCount(void);
};