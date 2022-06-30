#pragma once

class GameDB : public CoreGameDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	void LoadCharacter(const int64_t accountUID, std::vector<LoginPacket::CharacterInfoT>& infoList);
	uint8_t LoadMaxCharacterSlotCount(const int64_t accountUID);
	void UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount);
	bool CreateCharacter(const int64_t accountUID, std::wstring_view name, LoginPacket::CharacterInfoT& info);
	bool DeleteCharacter(const int64_t accountUID, const int64_t characterUID);
};