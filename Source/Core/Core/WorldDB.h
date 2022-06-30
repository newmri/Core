#pragma once

class WorldDB : public CoreDB
{
public:
	WorldDB(std::wstring_view dbName);
	virtual ~WorldDB() override;

public:
	virtual void Release(void) override;

public:
	void SetID(const uint8_t worldID, const uint8_t serverID);
	void GetServerInfo(ServerInfo& serverInfo);

public:
	void IncreaseUserCount(void);

private:
	uint8_t worldID = 0;
	uint8_t serverID = 0;
};