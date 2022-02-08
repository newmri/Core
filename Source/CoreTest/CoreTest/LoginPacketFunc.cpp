#include "Include.h"

void LoginPacketFunc::Write(CoreServerSession& session, Login::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = Login::CreateRoot(this->builder, packetType, packet);
	this->builder.Finish(data);
	session.Write(CorePacket(builder.GetBufferPointer(), this->builder.GetSize()));
}

void LoginPacketFunc::SC_LOGIN_RES(CoreServerSession& session, const void* data)
{
	auto raw = static_cast<const Login::SC_LOGIN_RES*>(data);
	CORE_LOG.Log(TO_STR(raw->result()));

	flatbuffers::FlatBufferBuilder builder;

	auto name = builder.CreateString("Hello World!");
	auto message = Login::CreateCS_LOGIN_REQ(this->builder, name);
	Write(session, Login::Packet_CS_LOGIN_REQ, message.Union());
}