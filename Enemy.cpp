#include "Enemy.h"

Enemy::Enemy(GameObject* parent)
	:TownNPC(parent)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	TownNPC::Initialize();
	SetSensitive(true);
}

void Enemy::Update()
{
	TownNPC::Update();

}

void Enemy::Draw()
{
	TownNPC::Draw();
}

void Enemy::Release()
{
	TownNPC::Release();
}
