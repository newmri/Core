#include "Include.h"

void LoginPacketFunc::SC_LOGIN_RES(CoreServerSession& session, const void* data)
{
	auto raw = static_cast<const LoginPacket::SC_LOGIN_RES*>(data);

	if (IS_SAME(LoginPacket::ErrorCode_SUCCESS, raw->result()))
	{
		auto loginClient = dynamic_cast<LoginClient*>(session.GetClient());

		auto account = CORE_ACCOUNT_MANAGER.Add(loginClient->GetAccountUID(), loginClient->GetToken());
		account->SetLogin();
		account->SetMaxSlotCount(raw->max_slot_count());
		
		auto infoList = raw->character_info();
		if (0 < infoList->size())
		{
			auto iter_begin = infoList->begin();
			auto iter_end = infoList->end();
			for (; iter_begin != iter_end; ++iter_begin)
			{
				auto info = iter_begin->UnPack();
				account->AddCharacter(std::make_shared<Character>(loginClient->GetAccountUID(), info->uid, *info));
			}
		}
	}

}

void LoginPacketFunc::SC_PING_REQ(CoreServerSession& session, const void* data)
{
	PACKET_SEND_MANAGER.builder.Clear();
	auto message = LoginPacket::CreateCS_PING_RES(PACKET_SEND_MANAGER.builder);
	PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_PING_RES, message.Union());
}

void LoginPacketFunc::SC_CREATE_CHARACTER_RES(CoreServerSession& session, const void* data)
{
}
