#include "Include.h"

void LoginPacketFunc::Write(std::shared_ptr<CoreClientSession> session, flatbuffers::FlatBufferBuilder& builder, Login::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = Login::CreateRoot(builder, Login::Packet_SC_LOGIN_RES, packet);
	builder.Finish(data);
	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

void LoginPacketFunc::CS_LOGIN_REQ(std::shared_ptr<CoreClientSession> session, const void* data)
{
	auto raw = static_cast<const Login::CS_LOGIN_REQ*>(data);
	CORE_LOG.Log(raw->name()->c_str());

	flatbuffers::FlatBufferBuilder builder;
	auto message = Login::CreateSC_LOGIN_RES(builder, true);
	Write(session, builder, Login::Packet_SC_LOGIN_RES, message.Union());
}