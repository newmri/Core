#pragma once

class WorldDB : public CoreDB
{
public:
	WorldDB(std::wstring_view dbName);
	virtual ~WorldDB() override;

public:
	virtual void Release(void) override;

public:
	void SetID(const int32_t worldID, const int32_t serverID);
	void GetServerInfo(ServerInfo& serverInfo);

public:
	void IncreaseUserCount(void);

private:
	int32_t worldID = 0;
	int32_t serverID = 0;
};