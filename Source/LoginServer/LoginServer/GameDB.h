#pragma once

class GameDB : public CoreDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	virtual void Release(void) override;

public:
	bool CreateCharacter(const int64_t accountUID, const wchar_t* name, const uint8_t level, const Define::Job job, int64_t& uid);
};