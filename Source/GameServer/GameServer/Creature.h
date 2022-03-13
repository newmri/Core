#pragma once

class Creature : public CoreGameObj
{
	OVERRIDE_GAME_OBJ(Creature)

public:
	Creature(const int64_t uid, const uint8_t ObjType);
};