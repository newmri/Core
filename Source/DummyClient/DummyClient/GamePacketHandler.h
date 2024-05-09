#pragma once

class GamePacketHandler
{
public:
	GamePacketHandler();

public:
	void Handle(std::shared_ptr<CoreServerSession> session, const GamePacket::Packet packetID, const void* data);

private:
	void Register(void);

private:
	std::unordered_map<GamePacket::Packet, std::function<void(GamePacketFunc&, std::shared_ptr<CoreServerSession>, const void*)>> func;
	GamePacketFunc packetFunc;
};