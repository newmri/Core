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

void PacketSendManager::Send(CoreServerSession& session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = LoginPacket::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session.Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}

void PacketSendManager::Send(CoreServerSession& session, GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	auto data = GamePacket::CreateRoot(builder, packetType, packet);
	builder.Finish(data);
	session.Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}