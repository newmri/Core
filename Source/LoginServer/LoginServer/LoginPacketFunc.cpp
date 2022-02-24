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
			session->SetAccountID(raw->uid());

			CORE_ACCOUNT_MANAGER.Add(raw->uid(), token);
			CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<>(std::bind(&CoreClientSession::CheckPingPongTime, session), SEC));
			
			result = Login::ErrorCode_SUCCESS;
		}
	}
	else
	{
		if(account->IsLogined())
			result = Login::ErrorCode_ALREADY_LOGINED;

		// 토큰 사용 기간이 지났음
		else if (account->IsTokenExpired())
		{
			if (LOGIN_SERVER.GetAccountDB()->Login(raw->uid(), token))
			{
				session->SetAccountID(raw->uid());

				account->SetLogin();
				account->UpdateToken(token);

				CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<>(std::bind(&CoreClientSession::CheckPingPongTime, session), SEC));
				
				result = Login::ErrorCode_SUCCESS;
			}
		}
		else
		{
			result = Login::ErrorCode_SUCCESS;
		}
	}

	this->builder.Clear();
	auto message = Login::CreateSC_LOGIN_RES(this->builder, result);
	Write(session, Login::Packet_SC_LOGIN_RES, message.Union());
}

void LoginPacketFunc::SC_PING_REQ(std::shared_ptr<CoreClientSession> session)
{
	Login::ErrorCode result = Login::ErrorCode_UNKNOWN;
	builder.Clear();
	auto message = Login::CreateSC_LOGIN_RES(builder, result);
	Write(session, Login::Packet_SC_LOGIN_RES, message.Union());

	CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<>(std::bind(&CoreClientSession::CheckPingPongTime, session), SEC));
}

void LoginPacketFunc::CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data)
{
	session->UpdatePingPongTime();
}

void LoginPacketFunc::CS_CHARACTER_CREATE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const Login::CS_CHARACTER_CREATE_REQ*>(data);
}