#pragma once

class GamePacketHandler;

class GameClient : public CoreClient
{
public:
	GameClient(std::shared_ptr<CoreServerSession> session, const int64_t characterUID, std::string_view characterName);
	virtual ~GameClient() override;

public:
	std::string_view GetCharacterName(void);

public:
	void Connect(void);

private:
	std::string characterName;
};