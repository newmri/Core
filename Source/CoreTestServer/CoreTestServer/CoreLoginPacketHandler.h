#pragma once

class CoreLoginPacketHandler
{
public:
	CoreLoginPacketHandler();

public:
	void Handle(unsigned short packetID, const void* data);

private:
	void Register(void);

private:
	void Login(const void* data);

private:
	std::map<unsigned short, std::function<void(CoreLoginPacketHandler&, const void*)>> func;
};