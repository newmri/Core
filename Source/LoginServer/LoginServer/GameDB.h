#pragma once

class GameDB : public CoreGameDB
{
public:
	GameDB() = default;
	GameDB(DBInfo&& dbInfo);
	virtual ~GameDB() override;

public:
	bool LoadCharacter(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList);
	bool LoadAllCharacterGear(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList);
	bool LoadCharacterGear(const int64_t accountUID, LoginPacket::CharacterInfoT& info);

	uint8_t LoadMaxCharacterSlotCount(const int64_t accountUID);
	void UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount);
	bool CreateCharacter(const int64_t accountUID, std::wstring_view name, LoginPacket::CharacterInfoT& info);
	bool DeleteCharacter(const int64_t accountUID, const int64_t characterUID);
};