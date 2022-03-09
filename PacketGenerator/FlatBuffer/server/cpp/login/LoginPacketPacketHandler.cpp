#include "Include.h"

LoginPacketPacketHandler::LoginPacketPacketHandler()
{
    Register();
}

void LoginPacketPacketHandler::Handle(std::shared_ptr<CoreClientSession> session, const LoginPacket::Packet packetID, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, session, data);
}

void LoginPacketPacketHandler::Register(void)
{
    this->func[LoginPacket::Packet_CS_LOGIN_REQ] = &LoginPacketPacketFunc::CS_LOGIN_REQ;
    this->func[LoginPacket::Packet_CS_PING_RES] = &LoginPacketPacketFunc::CS_PING_RES;
    this->func[LoginPacket::Packet_CS_LOGOUT_NOTI] = &LoginPacketPacketFunc::CS_LOGOUT_NOTI;
    this->func[LoginPacket::Packet_CS_CREATE_CHARACTER_REQ] = &LoginPacketPacketFunc::CS_CREATE_CHARACTER_REQ;
}
