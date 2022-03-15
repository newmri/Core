#include "Include.h"

thread_local flatbuffers::FlatBufferBuilder GamePacketFunc::builder;

void GamePacketFunc::Write(std::shared_ptr<CoreClientSession> session, GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = GamePacket::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

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

	this->builder.Clear();

	flatbuffers::Offset<GamePacket::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(GamePacket::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());
		
#pragma region 캐릭터 로드
		Info::CreatureInfoT creatureInfo;
		GamePacket::MyCharacterInfoT characterInfo;
		if (!GAME_SERVER.GetGameDB()->LoadCharacter(raw->uid(), raw->character_uid(), creatureInfo, characterInfo))
			return;

		uint8_t maxCharacterSlotCount = GAME_SERVER.GetGameDB()->LoadMaxCharacterSlotCount(raw->uid());
		account->SetMaxSlotCount(maxCharacterSlotCount);

		CREATURE_MANAGER.AddPlayer(raw->character_uid(), session, creatureInfo, characterInfo);
#pragma endregion 캐릭터 로드

#pragma region 재화 로드
		Info::MoneyWrapperT money;
		GAME_SERVER.GetAccountDB()->LoadMoney(raw->uid(), money.value);
		for (int32_t i = 0; i < Define::MoneyType_END; ++i)
			account->PushMoney(money.value.value[i]);
#pragma endregion 재화 로드

		auto packedCreatureInfo = Info::CreatureInfo::Pack(this->builder, &creatureInfo);
		auto packedCharacterInfo = GamePacket::MyCharacterInfo::Pack(this->builder, &characterInfo);
		auto packedMoney = Info::MoneyWrapper::Pack(this->builder, &money);

		message = GamePacket::CreateSC_LOGIN_RES(this->builder, result, packedCreatureInfo, packedCharacterInfo, packedMoney);

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));
	}
	// 실패
	else
		message = GamePacket::CreateSC_LOGIN_RES(this->builder, result);
	
	Write(session, GamePacket::Packet_SC_LOGIN_RES, message.Union());
}

void GamePacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	builder.Clear();
	auto message = GamePacket::CreateSC_PING_REQ(builder);
	Write(session, GamePacket::Packet_SC_PING_REQ, message.Union());

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