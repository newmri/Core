#include "Include.h"

LoginPacketHandler::LoginPacketHandler()
{
    Register();
}

void LoginPacketHandler::Handle(const Login::Packet packetID, std::shared_ptr<CoreClientSession> session, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, session, data);
}

void LoginPacketHandler::Register(void)
{
    this->func[Login::Packet_CS_LOGIN_REQ] = &LoginPacketFunc::CS_LOGIN_REQ;
}
