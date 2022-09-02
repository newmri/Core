#pragma once

#pragma pack(push, 1)

struct CoreEffectData
{
	CoreEffectData()
	{
		memset(this, 0, sizeof(this));
	}

	int32_t effectID;
	Define::EffectType effectType;
	int32_t effectValue;
	int32_t effectTick;
	int32_t effectDuration;
};

#pragma pack(pop)
