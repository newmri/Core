#include "Include.h"

CoreLoginPacketHandler::CoreLoginPacketHandler()
{
	Register();
}

void  CoreLoginPacketHandler::Handle(unsigned short packetID, const void* data)
{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](*this, data);
}


void CoreLoginPacketHandler::Register(void)
{
	this->func[Login::Packet_CS_LOGIN_REQ] = &CoreLoginPacketHandler::Login;
}

void CoreLoginPacketHandler::Login(const void* data)
{
	auto raw = static_cast<const Login::CS_LOGIN_REQ*>(data);
	CORE_LOG.Log(raw->name()->c_str());
}