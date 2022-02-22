#include "Include.h"

thread_local flatbuffers::FlatBufferBuilder LoginPacketFunc::builder;

void LoginPacketFunc::Write(std::shared_ptr<CoreClientSession> session, Login::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = Login::CreateRoot(this->builder, packetType, packet);
	this->builder.Finish(data);
	session->Write(CorePacket(this->builder.GetBufferPointer(), this->builder.GetSize()));
}

void LoginPacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const Login::CS_LOGIN_REQ*>(data);

	//int result = LOGIN_SERVER.GetAccountDB()->Login(STRING_MANAGER.Widen(raw->name()->str()).c_str(), STRING_MANAGER.Widen(raw->password()->str()).c_str());

	this->builder.Clear();
	auto message = Login::CreateSC_LOGIN_RES(this->builder, 0);
	Write(session, Login::Packet_SC_LOGIN_RES, message.Union());
}

void LoginPacketFunc::CS_CHARACTER_CREATE_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const Login::CS_CHARACTER_CREATE_REQ*>(data);
}