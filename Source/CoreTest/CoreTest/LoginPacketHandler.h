#pragma once

class LoginPacketHandler
{
public:
	LoginPacketHandler();

public:
	void Handle(CoreServerSession& session, const Login::Packet packetID, const void* data);

private:
	void Register(void);

private:
	std::map<Login::Packet, std::function<void(LoginPacketFunc&, CoreServerSession&, const void*)>> func;
	LoginPacketFunc packetFunc;
};