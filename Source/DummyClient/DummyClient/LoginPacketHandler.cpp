#include "Include.h"

LoginPacketHandler::LoginPacketHandler()
{
    Register();
}

void LoginPacketHandler::Handle(CoreServerSession& session, const LoginPacket::Packet packetID, const void* data)
{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}

void LoginPacketHandler::Register(void)
{
  
}
