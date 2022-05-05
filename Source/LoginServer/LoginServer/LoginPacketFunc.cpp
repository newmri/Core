#include "Include.h"

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

	PACKET_SEND_MANAGER.builder.Clear();

	flatbuffers::Offset<LoginPacket::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(LoginPacket::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());

		LOGIN_SERVER.GetWorldDB()->IncreaseUserCount();

		std::vector<LoginPacket::CharacterInfoT> infoList;
		LOGIN_SERVER.GetGameDB()->LoadCharacter(raw->uid(), infoList);
		uint8_t maxCharacterSlotCount = LOGIN_SERVER.GetGameDB()->LoadMaxCharacterSlotCount(raw->uid());
		account->SetMaxSlotCount(maxCharacterSlotCount);

		size_t size = infoList.size();
		uint8_t emptyCharacterSlotCount = maxCharacterSlotCount - static_cast<uint8_t>(size);

		if (size)
		{
			std::vector<flatbuffers::Offset<LoginPacket::CharacterInfo>> sendList;
	
			for (int32_t i = 0; i < size; ++i)
			{
				account->AddCharacter(std::make_shared<Character>(session->GetAccountUID(), infoList[i].uid, infoList[i]));
				sendList.push_back(LoginPacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &infoList[i]));
			}

			message = LoginPacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result, Define::CharacterLimit_MaxCharacterSlot,
													  emptyCharacterSlotCount, PACKET_SEND_MANAGER.builder.CreateVector(sendList));
		}

		else
		{
			message = LoginPacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result, Define::CharacterLimit_MaxCharacterSlot, emptyCharacterSlotCount);
		}

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));
	}
	// 실패
	else
		message = LoginPacket::CreateSC_LOGIN_RES(PACKET_SEND_MANAGER.builder, result);
	
	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_SC_LOGIN_RES, message.Union());
}

void LoginPacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = LoginPacket::CreateSC_PING_REQ(PACKET_SEND_MANAGER.builder);
	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_SC_PING_REQ, message.Union());

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

	std::string name = raw->name()->str();
	std::wstring wName = STRING_MANAGER.Widen(name);
	if(!STRING_MANAGER.IsValidLanguage(wName, Define::CharacterLimit_MinNameLen, Define::CharacterLimit_MaxNameLen))
		return;

	if (!IsBetween(raw->job(), Define::Job_MIN, Define::Job_MAX))
		return;

	if (!account->CanCreateCharacter())
		return;

	LoginPacket::CharacterInfoT info;
	info.name = name;
	info.level = 1;
	info.job = raw->job();

	bool isSuccess = LOGIN_SERVER.GetGameDB()->CreateCharacter(session->GetAccountUID(), wName, info);

	PACKET_SEND_MANAGER.builder.Clear();

	flatbuffers::Offset<LoginPacket::SC_CREATE_CHARACTER_RES> message;

	if (isSuccess)
	{
		account->AddCharacter(std::make_shared<Character>(session->GetAccountUID(), info.uid, info));
		
		auto packedInfo = LoginPacket::CharacterInfo::Pack(PACKET_SEND_MANAGER.builder, &info);

		message = LoginPacket::CreateSC_CREATE_CHARACTER_RES(PACKET_SEND_MANAGER.builder, isSuccess, packedInfo);
	}
	else
	{
		message = LoginPacket::CreateSC_CREATE_CHARACTER_RES(PACKET_SEND_MANAGER.builder, isSuccess);
	}

	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_SC_CREATE_CHARACTER_RES, message.Union());
}

void LoginPacketFunc::CS_TEST_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const LoginPacket::CS_TEST_REQ*>(data);

	std::cout << raw->test() << std::endl;

	//for (int i = 0; i < 10; ++i)
	//{
	//	PACKET_SEND_MANAGER.builder.Clear();
	//	auto message = LoginPacket::CreateSC_TEST_RES(PACKET_SEND_MANAGER.builder, i);
	//	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_SC_TEST_RES, message.Union());
	//}
}
