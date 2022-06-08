#include "Include.h"

LoginPacketHandler::LoginPacketHandler()
{
    Register();
}

void LoginPacketHandler::Handle(std::shared_ptr<CoreClientSession> session, const LoginPacket::Packet packetID, const void* data)
{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}

void LoginPacketHandler::Register(void)
{
    this->func[LoginPacket::Packet_CS_LOGIN_REQ] = &LoginPacketFunc::CS_LOGIN_REQ;
    this->func[LoginPacket::Packet_CS_PING_RES] = &LoginPacketFunc::CS_PING_RES;
    this->func[LoginPacket::Packet_CS_LOGOUT_NOTI] = &LoginPacketFunc::CS_LOGOUT_NOTI;
    this->func[LoginPacket::Packet_CS_CREATE_CHARACTER_REQ] = &LoginPacketFunc::CS_CREATE_CHARACTER_REQ;
    this->func[LoginPacket::Packet_CS_DELETE_CHARACTER_REQ] = &LoginPacketFunc::CS_DELETE_CHARACTER_REQ;
}
