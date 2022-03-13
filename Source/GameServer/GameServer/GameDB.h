#pragma once

class GameDB : public CoreDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	virtual void Release(void) override;

public:
	bool LoadCharacter(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);
	bool LoadCharacterStat(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);
	uint8_t LoadMaxCharacterSlotCount(const int64_t accountUID);
	void UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount);
};