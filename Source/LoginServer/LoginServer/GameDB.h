#pragma once

class GameDB : public CoreDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~AccountDB() override;

public:
	virtual void Release(void) override;
};