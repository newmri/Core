#pragma once

class Sector
{
public:
	Sector(const NativeInfo::Vec2Int& index);

public:
	void Add(std::shared_ptr<Creature> creature);
	void Remove(const int64_t oid);

private:
	NativeInfo::Vec2Int index;
	std::map<int64_t, std::shared_ptr<Creature>> creatureList;
};