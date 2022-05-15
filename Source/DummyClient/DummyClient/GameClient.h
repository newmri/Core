#pragma once

class GamePacketHandler;

class GameClient : public CoreClient
{
public:
	GameClient(const int64_t oid, const int64_t accountUID, const int32_t token, const int64_t characterUID);
	virtual ~GameClient() override;

public:
	void Connect(void);

public:
	virtual void OnConnected(void) override;
	virtual void OnDisconnected(void) override;
	virtual void ProcessPacket(const uint8_t* data, size_t size) override;

public:
	int64_t GetCharacterUID(void);

private:
	GamePacketHandler* handler;

private:
	int64_t characterUID;
};