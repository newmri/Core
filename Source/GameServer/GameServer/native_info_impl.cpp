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

}  
