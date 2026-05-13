#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/CsvReader.h"
#include "StageManager.h"

Enemy::Enemy(GameObject* parent)
    : PaintObject(parent, "Enemy")
{

}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
    hModel_ = Model::Load("Models/enemy2.fbx");
    assert(hModel_ >= 0);

    transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");


	//スポーンしたマスから移動する方向を決める
	


	//目標とするビルを決める

}

void Enemy::Update()
{
	switch (state_)
	{
	case ENEMY_MOVE:
		//ビルの横を移動させる
		//基本は真っすぐ、目標となるビルが道路をはさむ場合は後で
		break;
	case ENEMY_ENTERBUILDING:
		//ビルについたらその方向を見る
		break;
	case ENEMY_ESCAPE:
		//入って消える
		break;
	case ENEMY_PAINTED:
		//プレイヤーに塗られたら回転して上昇していく
	//その後、消える
		break;
	default:
		break;
	}
    
}

void Enemy::Draw()
{
    Model::SetTransform(hModel_, transform_,GetMosaicRT()->GetShaderResourceView());
    Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::Move()
{
	
}

