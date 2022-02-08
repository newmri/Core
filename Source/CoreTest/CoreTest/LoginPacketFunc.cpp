#include "Include.h"

void LoginPacketFunc::Write(CoreServerSession& session, flatbuffers::FlatBufferBuilder& builder, Login::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = Login::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session.Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

void LoginPacketFunc::SC_LOGIN_RES(CoreServerSession& session, const void* data)
{
	auto raw = static_cast<const Login::SC_LOGIN_RES*>(data);
	CORE_LOG.Log(TO_STR(raw->result()));

	flatbuffers::FlatBufferBuilder builder;

	auto name = builder.CreateString("Hello World!");
	auto message = Login::CreateCS_LOGIN_REQ(builder, name);
	Write(session, builder, Login::Packet_CS_LOGIN_REQ, message.Union());
}