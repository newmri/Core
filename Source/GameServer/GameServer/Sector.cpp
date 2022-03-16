#include "Include.h"

Sector::Sector(const NativeInfo::Vec2Int& index) : index(index)
{
}

void Sector::AddPlayer(std::shared_ptr<Player> player)
{
	this->playerList[player->GetUID()] = player;
}

void Sector::RemovePlayer(const int64_t uid)
{
	this->playerList.erase(uid);
}
