#include "Enemy.h"
#include "Engine/Model.h"

Enemy::Enemy(GameObject* parent)
	: TownNPC(parent)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	TownNPC::Initialize();
	SetSensitive(true);
	//hModel_ = Model::Load("Models/enemy2.fbx");
	//assert(hModel_ >= 0);
	//transform_.scale_ = { 0.1f,0.1f,0.1f };

}

void Enemy::Update()
{
	TownNPC::Update();

	if (IsAllPainted())
	{
		if (!isUpdatePainted)
		{
			hModel_ = Model::Load("Models/gun.fbx");
			isUpdatePainted = true;
		}
	}

}

void Enemy::Draw()
{
	TownNPC::Draw();
}

void Enemy::Release()
{
	TownNPC::Release();
}
