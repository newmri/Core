#pragma once

class GamePacketHandler;

class GameClient : public CoreClient
{
public:
	GameClient(std::shared_ptr<CoreServerSession> session, const int64_t characterUID);
	virtual ~GameClient() override;

public:
	void Connect(void);
};