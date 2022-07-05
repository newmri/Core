#pragma once

class GameDB : public CoreGameDB
{
public:
	GameDB(std::wstring_view dbName);
	virtual ~GameDB() override;

public:
	bool LoadCharacter(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);
	bool LoadCharacterStat(const int64_t accountUID, const int64_t uid, Info::CreatureInfoT& creatureInfo, GamePacket::MyCharacterInfoT& characterInfo);
	bool LoadCharacterGear(const int64_t accountUID, const int64_t uid, GamePacket::MyCharacterInfoT& characterInfo);

	uint8_t LoadMaxCharacterSlotCount(const int64_t accountUID);
	void UpdateMaxCharacterSlotCount(const int64_t accountUID, const uint8_t maxCharacterSlotCount);

public:
	uint8_t LoadMaxItemInventorySlotCount(const int64_t accountUID, const int64_t uid);
	bool LoadItemInventory(const int64_t accountUID, const int64_t uid, std::unordered_map<int64_t, Info::ItemSlotInfoT>& itemInventory);

public:
	bool EquipGear(const int64_t accountUID, const int64_t uid, const Define::GearType gearType, const Info::ItemSlotInfoT& itemSlotInfo);
	bool UnEquipGear(const int64_t accountUID, const int64_t uid, const Define::GearType gearType, const NativeInfo::GearSlotInfo& gearSlotInfo);
};