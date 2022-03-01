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

	// ù �α���
	if (IS_NULL(account))
	{
		if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
		{
			CORE_ACCOUNT_MANAGER.Add(raw->uid(), token);
			result = Login::ErrorCode_SUCCESS;
		}
	}
	else
	{
		if (account->IsLogined())
			result = Login::ErrorCode_ALREADY_LOGINED;

		// ��ū ��� �Ⱓ�� ������
		else if (account->IsTokenExpired())
		{
			if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
			{
				account->SetLogin();
				account->UpdateToken(token);
				result = Login::ErrorCode_SUCCESS;
			}
		}
		else
		{
			account->SetLogin();
			result = Login::ErrorCode_SUCCESS;
		}
	}

	this->builder.Clear();

	flatbuffers::Offset<Login::SC_LOGIN_RES> message;

	// ����
	if (IS_SAME(Login::ErrorCode_SUCCESS, result))
	{
		session->SetAccountUID(raw->uid());

		CORE_TIME_DELEGATE_MANAGER.Push(
			CoreTimeDelegate<>(
				std::bind(&CoreClientSession::CheckPingPongTime, session),
				HALF_MIN));

		message = Login::CreateSC_LOGIN_RES(this->builder, result, Define::CharacterLimit_MaxCharacterSlot, Define::CharacterLimit_EmptyCharacterSlot);
	}
	// ����
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

	std::wstring name = STRING_MANAGER.Widen(raw->name()->string_view());
	if(!STRING_MANAGER.IsValidLanguage(name, Define::CharacterLimit_MinNameLen, Define::CharacterLimit_MaxNameLen))
		return;

	if (!IsBetween(raw->job(), Define::Job_MIN, Define::Job_MAX))
		return;

	if (account->GetCharacterCount() >= Define::CharacterLimit_MaxCharacterSlot)
		return;

	int64_t uid = 0;
	bool isSuccess = LOGIN_SERVER.GetGameDB()->CreateCharacter(session->GetAccountUID(), name.c_str(), 1, raw->job(), uid);

	this->builder.Clear();

	flatbuffers::Offset<Login::SC_CREATE_CHARACTER_RES> message;

	if (isSuccess)
	{
		account->AddCharacter(uid);
		auto info = Login::CreateCHARACTER_INFODirect(this->builder, uid, raw->name()->c_str(), 1, raw->job());
		message = Login::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess, info);
	}
	else
	{
		message = Login::CreateSC_CREATE_CHARACTER_RES(this->builder, isSuccess);
	}

	Write(session, Login::Packet_SC_CREATE_CHARACTER_RES, message.Union());
}