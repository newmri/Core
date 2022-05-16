#include "Include.h"

IMPLEMENT_SINGLETON(GamePacketSendManager)

thread_local bool GamePacketSendManager::isFinished;
thread_local flatbuffers::FlatBufferBuilder GamePacketSendManager::builder;

void GamePacketSendManager::Init(void)
{
}

void GamePacketSendManager::Release(void)
{
	GetInstance().~GamePacketSendManager();
}

void GamePacketSendManager::Clear(void)
{
	isFinished = false;
	builder.Clear();
}

void GamePacketSendManager::Send(std::shared_ptr<CoreServerSession> session, GamePacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	if (!isFinished)
	{
		isFinished = true;
		auto data = GamePacket::CreateRoot(builder, packetType, packet);
		builder.Finish(data);
	}

	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}