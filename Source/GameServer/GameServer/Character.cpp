#include "Include.h"

Character::Character(const Info::CreatureInfoT& creatureInfo, const GamePacket::MyCharacterInfoT& characterInfo) :
	Creature(creatureInfo), characterInfo(characterInfo)
{
	Init();
}

Character::~Character()
{

}

void Character::Init(void)
{

}

void Character::Update(void)
{

}