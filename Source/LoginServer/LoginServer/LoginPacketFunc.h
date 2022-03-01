#pragma once

class LoginPacketFunc
{
private:
	static void Write(std::shared_ptr<CoreClientSession> session, Login::Packet packetType, flatbuffers::Offset<void> packet);
	flatbuffers::Offset<Login::CHARACTER_INFO> MakeCharacterInfo(const CharacterLoadInfo& loadInfo);

public:
	void CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data);
	static void SC_PING_REQ(std::shared_ptr<CoreClientSession> session);
	void CS_PING_RES(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_LOGOUT_NOTI(std::shared_ptr<CoreClientSession> session, const void* data);
	void CS_CREATE_CHARACTER_REQ(std::shared_ptr<CoreClientSession> session, const void* data);

private:
	static thread_local flatbuffers::FlatBufferBuilder builder;
};