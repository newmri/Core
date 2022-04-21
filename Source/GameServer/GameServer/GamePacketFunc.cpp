#include "Include.h"

void GamePacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const GamePacket::CS_LOGIN_REQ*>(data);

	GamePacket::ErrorCode result = GamePacket::ErrorCode_UNKNOWN;

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(raw->uid());
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

	PACKET_SEND_MANAGER.builder.Clear();

	flatbuffers::Offset<GamePacket::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(GamePacket::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());
		
#pragma region 캐릭터 로드
		Info::ObjectInfoT objectInfo;
		Info::CreatureInfoT creatureInfo;
		GamePacket::MyCharacterInfoT characterInfo;
		if (!GAME_SERVER.GetGameDB()->LoadCharacter(raw->uid(), raw->character_uid(), creatureInfo, characterInfo))
			return;

		uint8_t maxCharacterSlotCount = GAME_SERVER.GetGameDB()->LoadMaxCharacterSlotCount(raw->uid());
		account->SetMaxSlotCount(maxCharacterSlotCount);
		account->SetPlayerOID(OBJECT_MANAGER.AddPlayer(raw->character_uid(), session, objectInfo, creatureInfo, characterInfo));
#pragma endregion 캐릭터 로드

#pragma region 재화 로드
		Info::MoneyWrapperT money;
		GAME_SERVER.GetAccountDB()->LoadMoney(raw->uid(), money.value);
		for (int32_t i = 0; i < Define::MoneyType_END; ++i)
			account->PushMoney(money.value.value[i]);
#pragma endregion 재화 로드

		auto packedObjectInfo = Info::ObjectInfo::Pack(PACKET_SEND_MANAGER.builder, &objectInfo);
		auto packedCreatureInfo = Info::CreatureInfo::Pack(PACKET_SEND_MANAGER.builder, &creatureInfo);
		auto packedCharacterInfo = GamePacket::MyCharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &characterInfo);
		auto packedMoney = Info::MoneyWrapper::Pack(PACKET_SEND_MANAGER.builder, &money);

		message = GamePacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result, packedObjectInfo, packedCreatureInfo, packedCharacterInfo, packedMoney);

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));
	}
	// 실패
	else
		message = GamePacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result);
	
	PACKET_SEND_MANAGER.Send(session, GamePacket::Packet_SC_LOGIN_RES, message.Union());
}

void GamePacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	PACKET_SEND_MANAGER.builder.Clear();
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
	auto destPos = raw->UnPack()->pos_info.pos;
	if (!player->IsValidMoveSpeed(destPos))
		return;

	if (player->IsDead())
		return;

	ZONE_MANAGER.Move(player, destPos);
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
	
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = GamePacket::CreateSC_SET_STATE_RES(PACKET_SEND_MANAGER.builder, player->GetObjectType(), oid, state);
	ZONE_MANAGER.SendAllExceptMe(player->GetMapID(), oid, GamePacket::Packet_SC_SET_STATE_RES, message.Union(), player->GetPos());
}

void GamePacketFunc::CS_USE_SKILL_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	int64_t oid = session->GetPlayerOID();
	auto player = OBJECT_MANAGER.FindPlayer(oid);
	if (IS_NULL(player))
		return;

	if (player->IsDead())
		return;

	auto raw = static_cast<const GamePacket::CS_USE_SKILL_REQ*>(data);
	player->UseSkill(raw->skill_id());
}

void GamePacketFunc::CS_REVIVE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	int64_t oid = session->GetPlayerOID();
	auto player = OBJECT_MANAGER.FindPlayer(oid);
	if (IS_NULL(player))
		return;

	player->Revive();
}
