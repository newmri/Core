#pragma once

class CoreWorldDB : public CoreDB
{
public:
	CoreWorldDB() = default;
	CoreWorldDB(DBInfo&& dbInfo);
	virtual ~CoreWorldDB() override;

public:
	void GetServerInfo(ServerInfo& serverInfo);
};