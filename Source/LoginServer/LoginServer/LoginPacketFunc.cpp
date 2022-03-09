#include "Include.h"

thread_local flatbuffers::FlatBufferBuilder LoginPacketFunc::builder;

void LoginPacketFunc::Write(std::shared_ptr<CoreClientSession> session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = LoginPacket::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

flatbuffers::Offset<LoginPacket::CHARACTER_INFO> LoginPacketFunc::MakeCharacterInfo(const CharacterLoadInfo& loadInfo)
{
	auto info = LoginPacket::CreateCHARACTER_INFO(this->builder,
				loadInfo.uid,
				this->builder.CreateString(STRING_MANAGER.Narrow(loadInfo.name)),
				loadInfo.info.level,
				static_cast<Define::Job>(loadInfo.info.job),
				this->builder.CreateVector(loadInfo.info.gear.value, Define::GearType_MAX + 1));

	return info;
}

void LoginPacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const LoginPacket::CS_LOGIN_REQ*>(data);

	LoginPacket::ErrorCode result = LoginPacket::ErrorCode_UNKNOWN;

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(raw->uid());
	CoreToken token(raw->token());

	// 첫 로그인
	if (IS_NULL(account))
	{
		if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
		{
			account = CORE_ACCOUNT_MANAGER.Add(raw->uid(), token);
			result = LoginPacket::ErrorCode_SUCCESS;
		}
	}
	else
	{
		if (account->IsLogined())
			result = LoginPacket::ErrorCode_ALREADY_LOGINED;

		// 토큰 사용 기간이 지났음
		else if (account->IsTokenExpired())
		{
			if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
			{
				account->SetLogin();
				account->UpdateToken(token);
				result = LoginPacket::ErrorCode_SUCCESS;
			}
		}
		else
		{
			account->SetLogin();
			result = LoginPacket::ErrorCode_SUCCESS;
		}
	}

	this->builder.Clear();

	flatbuffers::Offset<LoginPacket::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(LoginPacket::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());

		std::vector<CharacterLoadInfo> infoList;
		LOGIN_SERVER.GetGameDB()->LoadCharacter(raw->uid(), infoList);
		uint8_t maxCharacterSlotCount = LOGIN_SERVER.GetGameDB()->LoadMaxCharacterSlotCount(raw->uid());
		account->SetMaxSlotCount(maxCharacterSlotCount);

		size_t size = infoList.size();
		uint8_t emptyCharacterSlotCount = maxCharacterSlotCount - static_cast<uint8_t>(size);

		if (size)
		{
			std::vector<flatbuffers::Offset<LoginPacket::CHARACTER_INFO>> sendList;
	
			for (int i = 0; i < size; ++i)
			{
				account->AddCharacter(std::make_shared<LoginCharacter>(session->GetAccountUID(), infoList[i].uid, infoList[i].info));

				sendList.push_back(MakeCharacterInfo(infoList[i]));
			}

			message = LoginPacket::CreateSC_LOGIN_RES(this->builder, result, Define::CharacterLimit_MaxCharacterSlot,
											    emptyCharacterSlotCount, this->builder.CreateVector(sendList));
		}

		else
		{
			message = LoginPacket::CreateSC_LOGIN_RES(this->builder, result, Define::CharacterLimit_MaxCharacterSlot, emptyCharacterSlotCount);
		}

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));
	}
	// 실패
	else
		message = LoginPacket::CreateSC_LOGIN_RES(this->builder, result);
	
	Write(session, LoginPacket::Packet_SC_LOGIN_RES, message.Union());
}

void LoginPacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	builder.Clear();
	auto message = LoginPacket::CreateSC_PING_REQ(builder);
	Write(session, LoginPacket::Packet_SC_PING_REQ, message.Union());

	CORE_TIME_DELEGATE_MANAGER.Push(
		CoreTimeDelegate<>(
			std::bind(&CoreClientSession::CheckPingPongTime, session),
			HALF_MIN));
}

void LoginPacketFunc::CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data)
{
	session->UpdatePingPongTime();
}

void LoginPacketFunc::CS_LOGOUT_NOTI(std::shared_ptr<CoreClientSession> session, const void* data)
{
	LOGIN_SERVER.Close(session);
}

void LoginPacketFunc::CS_CREATE_CHARACTER_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const LoginPacket::CS_CREATE_CHARACTER_REQ*>(data);

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(session->GetAccountUID());
	if (IS_NULL(account))
		return;

	std::wstring name = STRING_MANAGER.Widen(raw->name()->string_view());
	if(!STRING_MANAGER.IsValidLanguage(name, Define::CharacterLimit_MinNameLen, Define::CharacterLimit_MaxNameLen))
		return;

	if (!IsBetween(raw->job(), Define::Job_MIN, Define::Job_MAX))
		return;

	if (!account->CanCreateCharacter())
		return;

	CharacterLoadInfo loadInfo;
	wcscpy_s(loadInfo.name, Define::CharacterLimit_MaxNameLen, name.c_str());

	loadInfo.info = DATA_MANAGER.MakeCharacterInfo(1, raw->job());
	bool isSuccess = LOGIN_SERVER.GetGameDB()->CreateCharacter(session->GetAccountUID(), loadInfo);

	this->builder.Clear();

	flatbuffers::Offset<LoginPacket::SC_CREATE_CHARACTER_RES> message;

	if (isSuccess)
	{
		account->AddCharacter(std::make_shared<LoginCharacter>(session->GetAccountUID(), loadInfo.uid, loadInfo.info));

		auto info = MakeCharacterInfo(loadInfo);

		message = LoginPacket::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess, info);
	}
	else
	{
		message = LoginPacket::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess);
	}

	Write(session, LoginPacket::Packet_SC_CREATE_CHARACTER_RES, message.Union());
}