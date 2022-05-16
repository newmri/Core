#pragma once

class LoginPacketHandler
{
public:
	LoginPacketHandler();

public:
	void Handle(std::shared_ptr<CoreServerSession> session, const LoginPacket::Packet packetID, const void* data);

private:
	void Register(void);

private:
	std::map<LoginPacket::Packet, std::function<void(LoginPacketFunc&, std::shared_ptr<CoreServerSession>, const void*)>> func;
	LoginPacketFunc packetFunc;
};