#pragma once

class WorldDB : public CoreDB
{
public:
	WorldDB(std::wstring_view dbName);
	virtual ~WorldDB() override;

public:
	virtual void Release(void) override;

public:
	void IncreaseUserCount(const int64_t accountUID);
};