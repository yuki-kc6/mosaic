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

	state_ = ENEMY_SPAWN;

    transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");


	//スポーンしたマスから移動する方向を決める
	currentX = transform_.position_.x/29.0f;
	currentZ = transform_.position_.z/29.0f;

	direction_ = ENEMY_RIGHT;//とりあえず右に行くことにする

	//目標とするビルを決める
	//基本的には
	for (int z = currentZ; z < stageManager->GetMapH(); z++)
	{
		for (int x = currentX; x < stageManager->GetMapW(); x++)
		{
			if (stageManager->GetMap(x, z) == 1)
			{
				goalX = x;
				goalZ = z;
				break;
			}
		}
	}
}

void Enemy::Update()
{
	switch (state_)
	{
	case ENEMY_SPAWN:
		//スポーンしたマスから
		state_ = ENEMY_MOVE;
		break;
	case ENEMY_MOVE:
		//ビルの横を移動させる
		//currentXかcurrentZの座標が次のマスに動くまでは同じ方向に動かす
		//goalZがcurrentZと同じならcurrentXだけを動かす
		//違う場合は、mapの値で3の場所にある横断歩道を目標にする。

		if (currentX < goalX)
		transform_.position_.x += 0.1f;
		currentX = transform_.position_.x / 29.0f;
		currentZ = transform_.position_.z / 29.0f;



		//回転行列をかける
		//基本は真っすぐ、目標となるビルが道路をはさむ場合は後で
		if (currentX = goalX)
		{
			if (currentZ = goalZ)
			{
				state_ = ENEMY_ENTERBUILDING;
			}
		}
		break;
	case ENEMY_ENTERBUILDING:
		//ビルについたらその方向を見る
		transform_.rotate_.y = 180.0f;

		break;
	case ENEMY_ESCAPE:
		//入って消える
		KillMe();
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
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMVECTOR vMoveForward = { 0,0,1,0 };

	vMoveForward = XMVector3TransformNormal(vMoveForward, mRotate);

	vPos += vMoveForward * moveSpeed_;
}

void Enemy::Rotate(EnemyDirection dir)
{
	switch (dir)	
	{
	case ENEMY_UP:
		break;
	case ENEMY_DOWN:
		break;
	case ENEMY_LEFT:
		break;
	case ENEMY_RIGHT:
		break;
	default:
		break;
	}

	

}

