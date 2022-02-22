#pragma once

class LoginPacketFunc
{
private:
	void Write(std::shared_ptr<CoreClientSession> session, Login::Packet packetType, flatbuffers::Offset<void> packet);

public:
	void CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_CHARACTER_CREATE_REQ(std::shared_ptr<CoreClientSession> session, const void* data);

private:
	static thread_local flatbuffers::FlatBufferBuilder builder;
};