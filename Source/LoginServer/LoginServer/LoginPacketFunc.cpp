#include "Include.h"

thread_local flatbuffers::FlatBufferBuilder LoginPacketFunc::builder;

void LoginPacketFunc::Write(std::shared_ptr<CoreClientSession> session, Login::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = Login::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

void LoginPacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const Login::CS_LOGIN_REQ*>(data);

	Login::ErrorCode result = Login::ErrorCode_UNKNOWN;

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(raw->uid());
	CoreToken token(raw->token());

	// 첫 로그인
	if (IS_NULL(account))
	{
		if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
		{
			session->SetAccountUID(raw->uid());

			CORE_ACCOUNT_MANAGER.Add(raw->uid(), token);

			CORE_TIME_DELEGATE_MANAGER.Push(
				CoreTimeDelegate<>(
					std::bind(&CoreClientSession::CheckPingPongTime, session),
					HALF_MIN));

			result = Login::ErrorCode_SUCCESS;
		}
	}
	else
	{
		if (account->IsLogined())
			result = Login::ErrorCode_ALREADY_LOGINED;

		// 토큰 사용 기간이 지났음
		else if (account->IsTokenExpired())
		{
			if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
			{
				session->SetAccountUID(raw->uid());

				account->SetLogin();
				account->UpdateToken(token);

				CORE_TIME_DELEGATE_MANAGER.Push(
					CoreTimeDelegate<>(
						std::bind(&CoreClientSession::CheckPingPongTime, session),
						HALF_MIN));

				result = Login::ErrorCode_SUCCESS;
			}
		}
		else
		{
			result = Login::ErrorCode_SUCCESS;
		}
	}

	this->builder.Clear();

	flatbuffers::Offset<Login::SC_LOGIN_RES> message;

	// 성공
	if (IS_SAME(Login::ErrorCode_SUCCESS, result))
	{
		message = Login::CreateSC_LOGIN_RES(this->builder, result, Define::CharacterLimit_MaxCharacterSlot, Define::CharacterLimit_EmptyCharacterSlot);
	}
	// 실패
	else
		message = Login::CreateSC_LOGIN_RES(this->builder, result);
	
	Write(session, Login::Packet_SC_LOGIN_RES, message.Union());
}

void LoginPacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	builder.Clear();
	auto message = Login::CreateSC_PING_REQ(builder);
	Write(session, Login::Packet_SC_PING_REQ, message.Union());

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
	auto raw = static_cast<const Login::CS_CREATE_CHARACTER_REQ*>(data);

	CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(session->GetAccountUID());
	if (IS_NULL(account))
		return;

	// 길이 체크 문제가 있다..
	size_t nameLen = raw->name()->str().length();

	if (nameLen < Define::CharacterLimit_MinNameLen ||
		nameLen > Define::CharacterLimit_MaxNameLen)
		return;

	if (raw->job() < Define::Job_MIN ||
		raw->job() > Define::Job_MAX)
		return;

	if (account->GetCharacterCount() >= Define::CharacterLimit_MaxCharacterSlot)
		return;

	int64_t uid = 0;
	bool isSuccess = LOGIN_SERVER.GetGameDB()->CreateCharacter(session->GetAccountUID(), STRING_MANAGER.Widen(raw->name()->c_str()).c_str(), 1, raw->job(), uid);

	this->builder.Clear();

	flatbuffers::Offset<Login::SC_CREATE_CHARACTER_RES> message;

	if (isSuccess)
	{
		auto info = Login::CreateCHARACTER_INFODirect(this->builder, uid, raw->name()->c_str(), 1, raw->job());
		message = Login::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess, info);
	}
	else
	{
		message = Login::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess);
	}

	Write(session, Login::Packet_SC_CREATE_CHARACTER_RES, message.Union());
}