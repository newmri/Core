#include "Include.h"

Player::Player(std::shared_ptr<Character> character) : Creature(character->GetUID(), 0)
{
	Init();
}

Player::~Player()
{

}

void Player::Init(void)
{

}

void Player::Update(void)
{

}