#include "Include.h"

IMPLEMENT_SINGLETON(LoginPacketSendManager)

thread_local bool LoginPacketSendManager::isFinished;
thread_local flatbuffers::FlatBufferBuilder LoginPacketSendManager::builder;

void LoginPacketSendManager::Init(void)
{
}

void LoginPacketSendManager::Release(void)
{
	GetInstance().~LoginPacketSendManager();
}

void LoginPacketSendManager::Clear(void)
{
	isFinished = false;
	builder.Clear();
}

void LoginPacketSendManager::Send(std::shared_ptr<CoreServerSession> session, LoginPacket::Packet packetType, flatbuffers::Offset<void> packet)
{
	if (!isFinished)
	{
		isFinished = true;
		auto data = LoginPacket::CreateRoot(builder, packetType, packet);
		builder.Finish(data);
	}

	session->Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
}