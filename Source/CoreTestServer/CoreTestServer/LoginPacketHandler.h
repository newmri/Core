#pragma once

class LoginPacketHandler
{
public:
	LoginPacketHandler();

public:
	void Handle(const Login::Packet packetID, std::shared_ptr<CoreClientSession> session, const void* data);

private:
	void Register(void);

private:
	std::map<Login::Packet, std::function<void(LoginPacketFunc&, std::shared_ptr<CoreClientSession>, const void*)>> func;
	LoginPacketFunc packetFunc;
};