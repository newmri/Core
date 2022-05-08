#pragma once

class GamePacketHandler
{
public:
	GamePacketHandler();

public:
	void Handle(CoreServerSession& session, const GamePacket::Packet packetID, const void* data);

private:
	void Register(void);

private:
	std::map<GamePacket::Packet, std::function<void(GamePacketFunc&, CoreServerSession&, const void*)>> func;
	GamePacketFunc packetFunc;
};