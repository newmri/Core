#pragma once

class GameDB : public CoreDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	virtual void Release(void) override;

public:
	void LoadCharacter(const int64_t accountUID, std::vector<CharacterLoadInfo>& infoList);
	bool CreateCharacter(const int64_t accountUID, CharacterLoadInfo& loadInfo);
};