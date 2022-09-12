#include "Include.h"

void LoginPacketFunc::SC_LOGIN_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const LoginPacket::SC_LOGIN_RES*>(data);

	if (IS_SAME(LoginPacket::ErrorCode_SUCCESS, raw->result()))
	{
		auto account = CORE_ACCOUNT_MANAGER.Add(session->GetAccountUID(), session->GetToken());
		account->SetLogin();
		account->SetMaxSlotCount(raw->max_slot_count());
		
		auto infoList = raw->character_info();
		if (IS_NULL(infoList))
		{
			LOGIN_PACKET_SEND_MANAGER.Clear();
			auto pakcedCharacterName = LOGIN_PACKET_SEND_MANAGER.builder.CreateString(session->GetAccountID());
			auto job = static_cast<Define::Job>(CORE_RANDOM_MANAGER_INT.GetRandom(Define::Job_MIN, Define::Job_DUELIST));
			auto message = LoginPacket::CreateCS_CREATE_CHARACTER_REQ(LOGIN_PACKET_SEND_MANAGER.builder, pakcedCharacterName, job);
			LOGIN_PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_CREATE_CHARACTER_REQ, message.Union());
		}
		else
		{
			auto iter_begin = infoList->begin();
			auto iter_end = infoList->end();
			int64_t characterUID = iter_begin->uid();
			std::string characterName = iter_begin->name()->str();
			for (; iter_begin != iter_end; ++iter_begin)
			{
				auto info = iter_begin->UnPack();
				account->AddCharacter(std::make_shared<Character>(session->GetAccountUID(), info->uid, *info));
			}

			DUMMY_CLIENT.ConnectToGameServer(session, characterUID, characterName);
		}
	}

}

void LoginPacketFunc::SC_PING_REQ(std::shared_ptr<CoreServerSession> session, const void* data)
{
	LOGIN_PACKET_SEND_MANAGER.Clear();
	auto message = LoginPacket::CreateCS_PING_RES(LOGIN_PACKET_SEND_MANAGER.builder);
	LOGIN_PACKET_SEND_MANAGER.Send(session, LoginPacket::Packet_CS_PING_RES, message.Union());
}

void LoginPacketFunc::SC_CREATE_CHARACTER_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
	auto raw = static_cast<const LoginPacket::SC_CREATE_CHARACTER_RES*>(data);
	if (raw->is_success())
	{
		auto characterInfo = *raw->UnPack()->character_info;

		CoreAccount* account = CORE_ACCOUNT_MANAGER.Find(session->GetAccountUID());
		if (IS_NULL(account))
			return;

		account->AddCharacter(std::make_shared<Character>(session->GetAccountUID(), characterInfo.uid, characterInfo));

		DUMMY_CLIENT.ConnectToGameServer(session, characterInfo.uid, characterInfo.name);
	}
}

void LoginPacketFunc::SC_DELETE_CHARACTER_RES(std::shared_ptr<CoreServerSession> session, const void* data)
{
}
