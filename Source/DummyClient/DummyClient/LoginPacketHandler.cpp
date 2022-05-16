#include "Include.h"

LoginPacketHandler::LoginPacketHandler()
{
    Register();
}

void LoginPacketHandler::Handle(std::shared_ptr<CoreServerSession> session, const LoginPacket::Packet packetID, const void* data)
{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}

void LoginPacketHandler::Register(void)
{
    this->func[LoginPacket::Packet_SC_LOGIN_RES] = &LoginPacketFunc::SC_LOGIN_RES;
    this->func[LoginPacket::Packet_SC_PING_REQ] = &LoginPacketFunc::SC_PING_REQ;
    this->func[LoginPacket::Packet_SC_CREATE_CHARACTER_RES] = &LoginPacketFunc::SC_CREATE_CHARACTER_RES;
}
