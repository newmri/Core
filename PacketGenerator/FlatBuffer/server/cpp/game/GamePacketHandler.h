#pragma once

class GamePacketHandler
{
public:
	GamePacketHandler();

public:
	void Handle(std::shared_ptr<CoreClientSession> session, const Game::Packet packetID, const void* data);

private:
	void Register(void);

private:
	std::map<Game::Packet, std::function<void(GamePacketFunc&, std::shared_ptr<CoreClientSession>, const void*)>> func;
	GamePacketFunc packetFunc;
};