#include "Include.h"

LoginPacketHandler::LoginPacketHandler()
{
    Register();
}

void LoginPacketHandler::Handle(const Login::Packet packetID, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, data);
}

void LoginPacketHandler::Register(void)
{
    this->func[Login::Packet_SC_LOGIN_RES] = &LoginPacketFunc::SC_LOGIN_RES;
}
