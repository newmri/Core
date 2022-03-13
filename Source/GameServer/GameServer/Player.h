#pragma once

class Player : public Creature
{
	OVERRIDE_GAME_OBJ(Player)

public:
	Player(std::shared_ptr<Character> character);

private:
	std::shared_ptr<Character> character;
};