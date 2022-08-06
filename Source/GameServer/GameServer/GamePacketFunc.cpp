#include "Include.h"

void GamePacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::CS_LOGIN_REQ*>(data);

	GamePacket::ErrorCode result = GamePacket::ErrorCode_UNKNOWN;

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(raw->uid());
	std::shared_ptr<Player> player;
	CoreToken token(raw->token());

	// 첫 로그인
	if (IS_NULL(account))
	{
		if (GAME_SERVER.GetAccountDB()->Login(raw->uid(), token))
		{
			account = CORE_ACCOUNT_MANAGER.Add(raw->uid(), token);
			result = GamePacket::ErrorCode_SUCCESS;
		}
	}
	else
	{
		if (account->IsLogined())
			result = GamePacket::ErrorCode_ALREADY_LOGINED;

		// 토큰 사용 기간이 지났음
		else if (account->IsTokenExpired())
		{
			if (GAME_SERVER.GetAccountDB()->Login(raw->uid(), token))
			{
				account->SetLogin();
				account->UpdateToken(token);
				result = GamePacket::ErrorCode_SUCCESS;
			}
		}
		else
		{
			account->SetLogin();
			result = GamePacket::ErrorCode_SUCCESS;
		}
	}

	flatbuffers::Offset<GamePacket::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(GamePacket::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());
		
		GAME_SERVER.GetWorldDB()->IncreaseUserCount();

#pragma region 캐릭터 로드
		Info::ObjectInfoWithPosT objectInfoWithPos;
		Info::CreatureInfoT creatureInfo;
		GamePacket::MyCharacterInfoT characterInfo;
		if (!GAME_SERVER.GetGameDB()->LoadCharacter(raw->uid(), raw->character_uid(), creatureInfo, characterInfo))
			return;

		uint8_t maxCharacterSlotCount = GAME_SERVER.GetGameDB()->LoadMaxCharacterSlotCount(raw->uid());
		if (IS_SAME(0, maxCharacterSlotCount))
			return;

		account->SetMaxSlotCount(maxCharacterSlotCount);
		player = OBJECT_MANAGER.AddPlayer(raw->character_uid(), session, objectInfoWithPos, creatureInfo, characterInfo);
		if (IS_NULL(player))
			return;

		account->SetPlayerOID(objectInfoWithPos.object_info.oid);
#pragma endregion 캐릭터 로드

#pragma region 재화 로드
		Info::MoneyWrapperT money;
		GAME_SERVER.GetAccountDB()->LoadMoney(raw->uid(), money.value);
		for (int32_t i = 0; i < Define::MoneyType_END; ++i)
			account->PushMoney(money.value.value[i]);
#pragma endregion 재화 로드

		PACKET_SEND_MANAGER.Clear();
		auto packedObjectInfoWithPos = Info::ObjectInfoWithPos::Pack(PACKET_SEND_MANAGER.builder, &objectInfoWithPos);
		auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
		auto packedCharacterInfo = GamePacket::MyCharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
		auto packedMoney = Info::MoneyWrapper::Pack(PACKET_SEND_MANAGER.builder, &money);

		message = GamePacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result, packedObjectInfoWithPos, packedCreatureInfo, packedCharacterInfo, packedMoney);

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));
	}
	// 실패
	else
	{
		PACKET_SEND_MANAGER.Clear();
		message = GamePacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result);
	}

	PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_SC_LOGIN_RES, message.Union());

	if (IS_NOT_NULL(player))
	{
		player->SendItemInventoryInfo();
	}
}

void GamePacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	if (!session->IsConnected())
		return;

	PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateSC_PING_REQ(PACKET_SEND_MANAGER.builder);
	PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_SC_PING_REQ, message.Union());

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&CoreClientSession::CheckPingPongTime, session),
			HALF_MIN));
}

void GamePacketFunc::CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data)
{
	session->UpdatePingPongTime();
}

void GamePacketFunc::CS_LOGOUT_NOTI(std::shared_ptr<CoreClientSession> session, const void* data)
{
	GAME_SERVER.Close(session);
}

void GamePacketFunc::CS_MOVE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	auto raw = static_cast<const GamePacket::CS_MOVE_REQ*>(data);
	auto isRun = raw->UnPack()->is_run;
	auto destPos = raw->UnPack()->pos_info.pos;
	if (!player->IsValidMoveSpeed(destPos))
		return;

	if (player->IsDead())
		return;

	ZONE_MANAGER.Move(player, destPos, isRun);
}

void GamePacketFunc::CS_SET_STATE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	int64_t oid = session->GetPlayerOID();
	auto player = OBJECT_MANAGER.FindPlayer(oid);
	if (IS_NULL(player))
		return;

	if (player->IsDead())
		return;

	auto raw = static_cast<const GamePacket::CS_SET_STATE_REQ*>(data);
	auto state = raw->UnPack()->state;
	player->SetState(state);
	
	PACKET_SEND_MANAGER.Clear();
	auto packetObjectInfo = player->GetPackedObjectInfo();
	auto message = GamePacket::CreateSC_SET_STATE_RES(PACKET_SEND_MANAGER.builder, &packetObjectInfo, state);
	ZONE_MANAGER.SendAllExceptMe(player->GetMapID(), oid, GamePacket::Packet_SC_SET_STATE_RES, message.Union(), player->GetPos());
}

void GamePacketFunc::CS_USE_SKILL_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	if (player->IsDead())
		return;

	auto raw = static_cast<const GamePacket::CS_USE_SKILL_REQ*>(data);
	player->UseSkill(raw->skill_id());
}

void GamePacketFunc::CS_REVIVE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	player->Revive();
}

void GamePacketFunc::CS_EQUIP_GEAR_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	auto raw = static_cast<const GamePacket::CS_EQUIP_GEAR_REQ*>(data);

	player->EquipGear(raw->item_uid());
}

void GamePacketFunc::CS_UNEQUIP_GEAR_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	auto raw = static_cast<const GamePacket::CS_UNEQUIP_GEAR_REQ*>(data);

	player->UnEquipGear(raw->gear_type());
}

void GamePacketFunc::CS_NORMAL_CHAT_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	auto raw = static_cast<const GamePacket::CS_NORMAL_CHAT_REQ*>(data);

	std::string chatMessage = raw->message()->str();
	std::wstring wChatMessage = STRING_MANAGER.Widen(chatMessage);
	if (wChatMessage.size() == 0)
		return;

	// 치트
	if (wChatMessage._Starts_with(L"/"))
	{
		CHEAT_MANAGER.OnCheat(wChatMessage, player);
		return;
	}

	else if (!STRING_MANAGER.IsValidLanguage(wChatMessage, Define::ChatLimit_MinChatLen, Define::ChatLimit_MaxChatLen))
		return;

	// 채팅 내용 체크 필요
	switch (raw->chat_type())
	{
	case Define::ChatType_NORMAL:
		break;
	case Define::ChatType_PARTY:
		break;
	case Define::ChatType_GUILD:
		break;
	default:
		return;
	}

	PACKET_SEND_MANAGER.Clear();
	auto message = GamePacket::CreateSC_NORMAL_CHAT_RES(PACKET_SEND_MANAGER.builder,
													    player->GetOID(), raw->chat_type(), PACKET_SEND_MANAGER.builder.CreateString(raw->message()));

	ZONE_MANAGER.SendAll(player->GetMapID(), GamePacket::Packet_SC_NORMAL_CHAT_RES, message.Union(), player->GetPos());
}

void GamePacketFunc::CS_ADD_STAT_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto player = OBJECT_MANAGER.FindPlayer(session->GetPlayerOID());
	if (IS_NULL(player))
		return;

	auto raw = static_cast<const GamePacket::CS_ADD_STAT_REQ*>(data);

	player->AddStat(raw->stat_type());
}
