#include "Include.h"

Arrow::Arrow(const std::shared_ptr<ProjectileSkill> owner, const Info::ObjectInfoT& objectInfo) : Projectile(owner, objectInfo)
{
	
}

Arrow::~Arrow()
{

}

void Arrow::Init(void)
{

}

void Arrow::Update(void)
{
	Projectile::Update();
}

void Arrow::DoDamage(std::shared_ptr<Object> target)
{
	GAME_PACKET_SEND_MANAGER.Clear();
	std::vector<flatbuffers::Offset<GamePacket::DamageInfo>> sendList;
	this->owner->DoDamage(target, sendList);

	if (!sendList.empty())
	{
		auto message = GamePacket::CreateSC_GET_DAMAGE_NOTI(GAME_PACKET_SEND_MANAGER.builder, GAME_PACKET_SEND_MANAGER.builder.CreateVector(sendList));
	}
}
