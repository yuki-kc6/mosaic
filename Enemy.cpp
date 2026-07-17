#include "Enemy.h"
#include "Engine/Audio.h"

Enemy::Enemy(GameObject* parent)
	:TownNPC(parent),isPainted_(false),hSound_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	TownNPC::Initialize();
	SetSensitive(true);//敵はセンシティブオブジェクト判定をオンにする
	//hSound_ = Audio::Load("", false);//音をロードしておく
}

void Enemy::Update()
{
	TownNPC::Update();
	//一度だけ流れるようにする
	if (IsAllPainted())
	{
		if (!isPainted_)
		{
			//Audio::Play(hSound_);
			AllPaintEffect();
			isPainted_ = true;
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
