#include "Include.h"

IMPLEMENT_SINGLETON(PacketSendManager)

thread_local bool PacketSendManager::isFinished;
thread_local flatbuffers::FlatBufferBuilder PacketSendManager::builder;

void PacketSendManager::Init(void)
{
}

void PacketSendManager::Release(void)
{
	GetInstance().~PacketSendManager();
}

void PacketSendManager::Clear(void)
{
	isFinished = false;
	builder.Clear();
}

void PacketSendManager::Send(std::shared_ptr<CoreClientSession> session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	if (!isFinished)
	{
		isFinished = true;
		auto data = LoginPacket::CreateRoot(builder, packetType, packet);
		builder.Finish(data);
	}

	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}