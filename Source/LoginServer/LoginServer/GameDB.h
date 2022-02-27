#pragma once

class GameDB : public CoreDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	virtual void Release(void) override;

public:
	Login::ErrorCode CreateCharacter(const wchar_t*, Define::Job job, int64_t& uid);
};