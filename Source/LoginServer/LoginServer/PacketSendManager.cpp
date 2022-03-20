#include "Include.h"

IMPLEMENT_SINGLETON(PacketSendManager)

thread_local flatbuffers::FlatBufferBuilder PacketSendManager::builder;

void PacketSendManager::Init(void)
{
}

void PacketSendManager::Release(void)
{
	GetInstance().~PacketSendManager();
}

void PacketSendManager::Send(std::shared_ptr<CoreClientSession> session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = LoginPacket::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}