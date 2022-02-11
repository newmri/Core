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

	//raw->name()->c_str();

	//this->builder.Clear();
	//auto message = Login::CreateSC_LOGIN_RES(this->builder, CORE_RANDOM_MANAGER_INT.GetRandom(0, 1));
	//Write(session, Login::Packet_SC_LOGIN_RES, message.Union());
}