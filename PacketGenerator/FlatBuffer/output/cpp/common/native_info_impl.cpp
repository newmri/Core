#include "native_info_impl.h"

#include "info_protocol_generated.h"

namespace flatbuffers
{
	Info::Vec2Int PackVec2Int(const NativeInfo::Vec2Int& obj)
	{
		return Info::Vec2Int(obj.x, obj.y);
	}

	const NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj)
	{
		return NativeInfo::Vec2Int(obj.x(), obj.y());
	}

	Info::Stat PackStat(const NativeInfo::Stat& obj)
	{
		return Info::Stat(obj);
	}

	const NativeInfo::Stat UnPackStat(const Info::Stat& obj)
	{
		return NativeInfo::Vec2Int(obj.value()->Data());
	}

	Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj)
	{
		return Info::CharacterGear(obj.index);
	}

	const NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj)
	{
		return NativeInfo::CharacterGear(obj.index()->Data());
	}
}
