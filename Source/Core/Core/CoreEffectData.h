#pragma once

#pragma pack(push, 1)

struct CoreEffectData
{
	CoreEffectData()
	{

	}

	int32_t effectID = 0;
	Define::EffectType effectType = Define::EffectType::EffectType_HP;
	int32_t effectValue = 0;
	int32_t effectTick = 0;
	int32_t effectDuration = 0;
};

#pragma pack(pop)
