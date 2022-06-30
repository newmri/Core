#pragma once

class CoreGameDB : public CoreDB
{
public:
	CoreGameDB(std::wstring_view dbName);
	virtual ~CoreGameDB() override;

public:
	CoreItemUID GetItemUID(const uint8_t serverID);
};