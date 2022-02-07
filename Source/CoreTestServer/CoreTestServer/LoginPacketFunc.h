#pragma once

class LoginPacketFunc
{
private:
	void Write(std::shared_ptr<CoreClientSession> session, flatbuffers::FlatBufferBuilder& builder, Login::Packet packetType, flatbuffers::Offset<void> packet);

public:
	void CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
};