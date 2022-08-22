#include "Include.h"

Player::Player(const int64_t& uid, const std::shared_ptr<CoreClientSession> session,
	const Info::ObjectInfoWithPosT& objectInfoWithPos, const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	uid(uid), session(session), Creature(objectInfoWithPos, creatureInfo), characterInfo(characterInfo)
{
	Init();
}

int64_t Player::GetUID(void) const
{
	return this->uid;
}

std::shared_ptr<CoreClientSession> Player::GetSession(void)
{
	return this->session->shared_from_this();
}

Player::~Player()
{

}

void Player::Init(void)
{
	CalculateAbilityWithNoLock();
}

void Player::Update(void)
{

}

void Player::Clear(void)
{
	Creature::Clear();
}

GamePacket::CharacterInfoT Player::GetCharacterInfo(void)
{
	GamePacket::CharacterInfoT info;

	READ_LOCK(this->infoMutex);
	info.name = this->characterInfo.name;
	info.job = this->characterInfo.job;
	memcpy_s(&info.gear, sizeof(info.gear), &this->characterInfo.gear, sizeof(this->characterInfo.gear));
	return info;
}

void Player::MakeSpawnPacket(GamePacket::Packet& packetType, flatbuffers::Offset<void>& packet)
{
	auto objectInfoWithPos = GetObjectInfoWithPos();
	auto creatureInfo = GetCreatureInfo();
	auto characterInfo = GetCharacterInfo();

	PACKET_SEND_MANAGER.Clear();
	auto packedObjectInfoWithPos = Info::ObjectInfoWithPos::Pack(PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
	auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
	auto packedCharacterInfo = GamePacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
	auto message = GamePacket::CreateSC_SPAWN_PLAYER_NOTI(PACKET_SEND_MANAGER.builder, packedObjectInfoWithPos, packedCreatureInfo, packedCharacterInfo);

	packetType = GamePacket::Packet_SC_SPAWN_PLAYER_NOTI;
	packet = message.Union();
}

void Player::CalculateAbilityWithNoLock(void)
{
	Creature::CalculateAbilityWithNoLock();
	CORE_ITEM_DATA_MANAGER.CalculateAbility(this->characterInfo.gear, this->creatureInfo.ability);

	CHARACTER_DATA_MANAGER.CalculateSpeed(this->characterInfo.job, this->creatureInfo.speed);
}

void Player::Send(GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	PACKET_SEND_MANAGER.Send(this->session, packetType, packet);
}

void Player::SetState(const Define::ObjectState state)
{
	Creature::SetState(state);

	if (IS_NOT_SAME(Define::ObjectState_WALK, state))
	{
		this->lastMoveTime = 0;
	}
}

bool Player::IsValidMoveSpeed(const NativeInfo::Vec2Int& destPos)
{
	int32_t clientDist = 0;
	float_t moveSpeed = 0.0f;
	{
		READ_LOCK(this->infoMutex);
		NativeInfo::Vec2Int dir = destPos - GetPosWithNoLock();

		// 한칸씩 이동 가능
		clientDist = dir.GetMagnitude();
		if (clientDist > 1)
			return false;

		float_t runSpeed = GetStateWithNoLock() == Define::ObjectState_RUN ? 1.3f : 1.0f;
		moveSpeed = GetSpeedWithNoLock(Define::SpeedType_MOVE_SPEED) * runSpeed;
	}

	// 첫 이동
	if (IS_SAME(0, this->lastMoveTime))
	{
		lastMoveTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
		return true;
	}

	TIME_VALUE nowTime = CORE_TIME_MANAGER.GetNowMilliSeconds();
	TIME_VALUE timeDiff = nowTime - this->lastMoveTime;
	int32_t serverDist = CoreUtil::IntRound(moveSpeed * (timeDiff / SEC_FLOAT));

	if (clientDist > serverDist)
	{
		CORE_LOG.Log(LogType::LOG_HACK, GetOID(), "ClientDist: " + TO_STR(clientDist) + "ServerDist: " + TO_STR(serverDist));
		return false;
	}

	this->lastMoveTime = nowTime;
	return true;
}

void Player::AddSkill(const int32_t skillID)
{
	const SkillData* const skillData = CHARACTER_DATA_MANAGER.GetSkillData(skillID);
	if (IS_NULL(skillData))
		return;

	WRITE_LOCK(this->skillMutex);

	switch (skillData->skillType)
	{
	case Define::SkillType_NORMAL:
		this->skillList[skillID] = std::make_shared<Skill>(Object::downcasted_shared_from_this<Creature>(), *skillData);
		break;
	case Define::SkillType_ARROW:
		this->skillList[skillID] = std::make_shared<ProjectileSkill>(Object::downcasted_shared_from_this<Creature>(), *skillData);
		break;
	}
}

bool Player::LoadData(void)
{
	if (!LoadItemInventory())
		return false;

	return true;
}

bool Player::LoadItemInventory(void)
{
	this->maxItemInventorySlotCount = GAME_SERVER.GetGameDB()->LoadMaxItemInventorySlotCount(this->session->GetAccountUID(), GetUID());
	if (IS_SAME(0, this->maxItemInventorySlotCount))
		return false;

	this->itemInventory.reserve(this->maxItemInventorySlotCount);
	return GAME_SERVER.GetGameDB()->LoadItemInventory(this->session->GetAccountUID(), GetUID(), this->itemInventory);
}

void Player::SendItemInventoryInfo(void)
{
	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_ITEM_INVENTORY_INFO_NOTI> message;

	{
		READ_LOCK(itemInventoryMutex);

		size_t size = this->itemInventory.size();
		if (size)
		{
			std::vector<flatbuffers::Offset<Info::ItemSlotInfo>> sendList;
			for (auto& d : this->itemInventory)
			{
				sendList.push_back(Info::ItemSlotInfo::Pack(PACKET_SEND_MANAGER.builder, &d.second));
			}

			message = GamePacket::CreateSC_ITEM_INVENTORY_INFO_NOTI(PACKET_SEND_MANAGER.builder, this->maxItemInventorySlotCount, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
		}
		else
		{
			message = GamePacket::CreateSC_ITEM_INVENTORY_INFO_NOTI(PACKET_SEND_MANAGER.builder, this->maxItemInventorySlotCount);
		}
	}

	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_ITEM_INVENTORY_INFO_NOTI, message.Union());
}

void Player::EquipGear(const int64_t itemUID)
{
	GamePacket::ErrorCode result = GamePacket::ErrorCode::ErrorCode_SUCCESS;
	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_EQUIP_GEAR_RES> message;
	Info::Ability ability;
	{
		WRITE_LOCK(this->itemInventoryMutex);

		auto itemSlotInfo = GetItemSlotInfoWithNoLock(itemUID, 1);
		if (IS_NULL(itemSlotInfo))
			return;

		auto itemData = CORE_ITEM_DATA_MANAGER.GetItemData(itemSlotInfo->item_id);
		if (IS_NULL(itemData))
			return;

		if (!CORE_ITEM_DATA_MANAGER.IsValidGearType(itemData->gearType))
			return;

		{
			WRITE_LOCK(this->infoMutex);

			if (this->characterInfo.gear.info[itemData->gearType].itemID)
				return;

			if (GAME_SERVER.GetGameDB()->EquipGear(this->session->GetAccountUID(), GetUID(), itemData->gearType, *itemSlotInfo))
			{
				CORE_ITEM_DATA_MANAGER.CalculateAbility(itemSlotInfo->item_id, this->creatureInfo.ability);
				ability = flatbuffers::PackAbility(this->creatureInfo.ability);

				this->characterInfo.gear.info[itemData->gearType].itemUID = itemSlotInfo->item_uid;
				this->characterInfo.gear.info[itemData->gearType].itemID = itemSlotInfo->item_id;

				this->itemInventory.erase(itemUID);

				message = GamePacket::CreateSC_EQUIP_GEAR_RES(PACKET_SEND_MANAGER.builder, result, itemUID);
			}
			else
			{
				result = GamePacket::ErrorCode::ErrorCode_UNKNOWN;
				message = GamePacket::CreateSC_EQUIP_GEAR_RES(PACKET_SEND_MANAGER.builder, result);
			}
		}
	}

	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_EQUIP_GEAR_RES, message.Union());

	if (IS_SAME(GamePacket::ErrorCode::ErrorCode_SUCCESS, result))
		SendAbility(ability);
}

void Player::UnEquipGear(const Define::GearType gearType)
{
	if (!CORE_ITEM_DATA_MANAGER.IsValidGearType(gearType))
		return;

	GamePacket::ErrorCode result = GamePacket::ErrorCode::ErrorCode_SUCCESS;
	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_UNEQUIP_GEAR_RES> message;
	Info::Ability ability;
	{
		WRITE_LOCK(this->infoMutex);

		if (IS_ZERO(this->characterInfo.gear.info[gearType].itemID))
			return;

		{
			WRITE_LOCK(this->itemInventoryMutex);

			if (GAME_SERVER.GetGameDB()->UnEquipGear(this->session->GetAccountUID(), GetUID(), gearType, this->characterInfo.gear.info[gearType]))
			{
				Info::ItemSlotInfoT itemSlotInfo;
				itemSlotInfo.item_uid = this->characterInfo.gear.info[gearType].itemUID;
				itemSlotInfo.item_id = this->characterInfo.gear.info[gearType].itemID;
				itemSlotInfo.item_count = 1;

				CORE_ITEM_DATA_MANAGER.DecreaseAbility(itemSlotInfo.item_id, this->creatureInfo.ability);
				ability = flatbuffers::PackAbility(this->creatureInfo.ability);

				this->itemInventory[itemSlotInfo.item_uid] = itemSlotInfo;
				this->characterInfo.gear.info[gearType].itemUID = 0;
				this->characterInfo.gear.info[gearType].itemID = 0;

				message = GamePacket::CreateSC_UNEQUIP_GEAR_RES(PACKET_SEND_MANAGER.builder, result, gearType);
			}
			else
			{
				result = GamePacket::ErrorCode::ErrorCode_UNKNOWN;
				message = GamePacket::CreateSC_UNEQUIP_GEAR_RES(PACKET_SEND_MANAGER.builder, result);
			}
		}
	}

	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_UNEQUIP_GEAR_RES, message.Union());
	
	if (IS_SAME(GamePacket::ErrorCode::ErrorCode_SUCCESS, result))
		SendAbility(ability);
}

bool Player::AddItemToInventory(const int32_t itemID, const uint16_t itemCount)
{
	auto itemData = CORE_ITEM_DATA_MANAGER.GetItemData(itemID);
	if (IS_NULL(itemData))
		return false;

	uint8_t needSlotCount = CoreUtil::IntRound(static_cast<float_t>(itemCount) / itemData->maxStackCount);
	uint8_t emptySlotCount = 0;

	std::vector<Info::ItemSlotInfoT> itemSlotInfoList;
	{
		WRITE_LOCK(this->itemInventoryMutex);

		emptySlotCount = this->maxItemInventorySlotCount - static_cast<uint8_t>(this->itemInventory.size());
		if (emptySlotCount < needSlotCount)
			return false;

		ItemCreateSlotInfo itemCreateSlotInfo(this->session->GetAccountUID(), GetUID(), itemID, itemCount, itemData->maxStackCount, needSlotCount);
		GAME_SERVER.GetGameDB()->AddItemToInventory(itemCreateSlotInfo, itemSlotInfoList);
		if (itemSlotInfoList.empty())
			return false;

		for (auto& d : itemSlotInfoList)
			this->itemInventory[d.item_uid] = d;
	}

	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI> message;
	std::vector<flatbuffers::Offset<Info::ItemSlotInfo>> sendList;
	for (auto& d : itemSlotInfoList)
	{
		sendList.push_back(Info::ItemSlotInfo::Pack(PACKET_SEND_MANAGER.builder, &d));
	}

	message = GamePacket::CreateSC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI(PACKET_SEND_MANAGER.builder, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI, message.Union());

	return true;
}

Info::ItemSlotInfoT* Player::GetItemSlotInfoWithNoLock(const int64_t itemUID, const uint16_t itemCount)
{
	if (auto iter = this->itemInventory.find(itemUID); iter != this->itemInventory.end())
	{
		if (iter->second.item_count >= itemCount)
			return &iter->second;
	}

	return nullptr;
}

void Player::SendAbility(const Info::Ability& ability)
{
	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_ABILITY_INFO_NOTI> message;
	message = GamePacket::CreateSC_ABILITY_INFO_NOTI(PACKET_SEND_MANAGER.builder, &ability);
	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_ABILITY_INFO_NOTI, message.Union());
}

void Player::AddStat(const Define::StatType statType)
{
	if (!IsBetween<Define::StatType>(statType, Define::StatType_MIN, static_cast<Define::StatType>(Define::StatType_END - 1)))
		return;

	GamePacket::ErrorCode result = GamePacket::ErrorCode::ErrorCode_SUCCESS;
	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_ADD_STAT_RES> message;
	Info::Ability ability;

	{
		WRITE_LOCK(this->infoMutex);
		if (IS_ZERO(this->characterInfo.bonus_stat))
			return;

		if (GAME_SERVER.GetGameDB()->AddStat(this->session->GetAccountUID(), GetUID(), statType))
		{
			this->creatureInfo.stat.value[statType] += 1;

			CalculateAbilityWithNoLock();
			ability = flatbuffers::PackAbility(this->creatureInfo.ability);

			message = GamePacket::CreateSC_ADD_STAT_RES(PACKET_SEND_MANAGER.builder, result, statType);
		}
		else
		{
			result = GamePacket::ErrorCode::ErrorCode_UNKNOWN;
			message = GamePacket::CreateSC_ADD_STAT_RES(PACKET_SEND_MANAGER.builder, result);
		}
	}

	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_ADD_STAT_RES, message.Union());

	if (IS_SAME(GamePacket::ErrorCode::ErrorCode_SUCCESS, result))
		SendAbility(ability);
}

void Player::OnLevelUpCheat(const uint8_t newLevel)
{
	auto levelData = CHARACTER_DATA_MANAGER.GetLevelData(newLevel);
	if (IS_NULL(levelData))
		return;

	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_MY_LEVEL_UP_NOTI> myLevelUpMessage;

	{
		WRITE_LOCK(this->infoMutex);

		int32_t newBonusStatPoint = this->characterInfo.bonus_stat + CHARACTER_DATA_MANAGER.GetBonusStatPoint(this->creatureInfo.level, newLevel);
		if (!GAME_SERVER.GetGameDB()->OnLevelUp(this->session->GetAccountUID(), GetUID(),
			newLevel, newBonusStatPoint, this->creatureInfo.ability.value[Define::AbilityType_HP], this->creatureInfo.ability.value[Define::AbilityType_MP]))
			return;

		this->creatureInfo.level = newLevel;
		this->creatureInfo.exp = 0;
		this->characterInfo.bonus_stat = newBonusStatPoint;
		this->creatureInfo.hp = this->creatureInfo.ability.value[Define::AbilityType_HP];
		this->creatureInfo.mp = this->creatureInfo.ability.value[Define::AbilityType_MP];

		myLevelUpMessage = GamePacket::CreateSC_MY_LEVEL_UP_NOTI(PACKET_SEND_MANAGER.builder, newLevel, newBonusStatPoint);
	}

	PACKET_SEND_MANAGER.Send(this->session, GamePacket::Packet_SC_MY_LEVEL_UP_NOTI, myLevelUpMessage.Union());

	PACKET_SEND_MANAGER.Clear();
	flatbuffers::Offset<GamePacket::SC_LEVEL_UP_NOTI> levelUpMessage;
	levelUpMessage = GamePacket::CreateSC_LEVEL_UP_NOTI(PACKET_SEND_MANAGER.builder, GetOID());
	ZONE_MANAGER.SendAllExceptMe(GetMapID(), GetOID(), GamePacket::Packet_SC_LEVEL_UP_NOTI, levelUpMessage.Union(), GetPos());
}