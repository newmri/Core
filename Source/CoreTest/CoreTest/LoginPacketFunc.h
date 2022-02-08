#pragma once

class LoginPacketFunc
{
private:
	void Write(CoreServerSession& session, flatbuffers::FlatBufferBuilder& builder, Login::Packet packetType, flatbuffers::Offset<void> packet);

public:
	void SC_LOGIN_RES(CoreServerSession& session, const void* data);
};