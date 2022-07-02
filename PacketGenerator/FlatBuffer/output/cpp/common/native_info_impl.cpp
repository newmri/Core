#include "CoreInclude.h"

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

	Info::ObjectInfo PackObjectInfo(const NativeInfo::ObjectInfo& obj)
	{
		return Info::ObjectInfo(obj.objectType, obj.oid);
	}

	NativeInfo::ObjectInfo UnPackObjectInfo(const Info::ObjectInfo& obj)
	{
		return NativeInfo::ObjectInfo(obj.object_type(), obj.oid());
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

	Info::GearSlotInfo PackGearSlotInfo(const NativeInfo::GearSlotInfo& obj)
	{
		return Info::GearSlotInfo(obj.itemUID, obj.itemID);
	}

	NativeInfo::GearSlotInfo UnPackGearSlotInfo(const Info::GearSlotInfo& obj)
	{
		return NativeInfo::GearSlotInfo(obj.item_uid(), obj.item_id());
	}

	Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj)
	{
		Info::GearSlotInfo arr[Define::GearType_END];

		for (int32_t i = 0; i < Define::GearType_END; ++i)
			arr[i] = PackGearSlotInfo(obj.info[i]);

		return Info::CharacterGear(arr);
	}

	NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj)
	{
		NativeInfo::CharacterGear gear;

		for (int32_t i = 0; i < Define::GearType_END; ++i)
			gear.info[i] = UnPackGearSlotInfo(*obj.info()->Get(i));

		return gear;
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
