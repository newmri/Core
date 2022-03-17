#include "Include.h"

namespace flatbuffers
{
	Info::Vec2Int PackVec2Int(const NativeInfo::Vec2Int& obj)
	{
		return Info::Vec2Int(obj.x, obj.y);
	}

	NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj)
	{
		return NativeInfo::Vec2Int(obj.x(), obj.y());
	}

	Info::PositionInfo PackPositionInfo(const NativeInfo::PositionInfo& obj)
	{
		return Info::PositionInfo(obj.mapID, PackVec2Int(obj.pos), obj.state, obj.moveDir);
	}

	NativeInfo::PositionInfo UnPackPositionInfo(const Info::PositionInfo& obj)
	{
		return NativeInfo::PositionInfo(obj.map_id(), UnPackVec2Int(obj.pos()), obj.state(), obj.move_dir());
	}

	Info::Stat PackStat(const NativeInfo::Stat& obj)
	{
		return Info::Stat(obj.value);
	}

	NativeInfo::Stat UnPackStat(const Info::Stat& obj)
	{
		return NativeInfo::Stat(obj.value());
	}

	Info::Ability PackAbility(const NativeInfo::Ability& obj)
	{
		return Info::Ability(obj.value);
	}

	NativeInfo::Ability UnPackAbility(const Info::Ability& obj)
	{
		return NativeInfo::Ability(obj.value());
	}

	Info::Speed PackSpeed(const NativeInfo::Speed& obj)
	{
		return Info::Speed(obj.value);
	}

	NativeInfo::Speed UnPackSpeed(const Info::Speed& obj)
	{
		return NativeInfo::Speed(obj.value());
	}

	Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj)
	{
		return Info::CharacterGear(obj.index);
	}

	NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj)
	{
		return NativeInfo::CharacterGear(obj.index());
	}

	Info::Money PackMoney(const NativeInfo::Money& obj)
	{
		return Info::Money(obj.value);
	}

	NativeInfo::Money UnPackMoney(const Info::Money& obj)
	{
		return NativeInfo::Money(obj.value());
	}
}
