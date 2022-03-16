#pragma once

class Sector
{
public:
	Sector(const NativeInfo::Vec2Int& index);

public:
	void AddPlayer(std::shared_ptr<Player> player);
	void RemovePlayer(const int64_t uid);

private:
	NativeInfo::Vec2Int index;
	std::map<int64_t, std::shared_ptr<Player>> playerList;
};