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
	case ENEMY_MOVE:
		DecideNextTile();//次に移動するマスを決める
		MoveNextTile();//次のマスに移動する
		if(currentX==goalX && currentZ==goalZ)
		{
			state_ = ENEMY_WAIT;
		}
		break;
	case ENEMY_WAIT:
		//ビルの中で待機する

		//if()
		// {
		// state_=ENEMY_GETOUTBUILDING;
		//}
		break;
	case ENEMY_PAINTED:
		
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

void Enemy::MoveNextTile()
{

	//今のマスの真ん中から次のマスの真ん中まで
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMVECTOR vMoveForward = { 0,0,1,0 };

	vMoveForward = XMVector3TransformNormal(vMoveForward, mRotate);

	vPos += vMoveForward * moveSpeed_;
	XMStoreFloat3(&transform_.position_, vPos);



}

void Enemy::DecideNextTile()
{
	for (int dir = 0; dir < 4; dir++)
	{
		int nx = currentX + dx[dir];
		int nz = currentZ + dz[dir];

		if(nx==goalX && nz==goalZ)
			{
			nextX = nx;
			nextZ = nz;
			return;
		}

		//if (nx < 0 || nx >= stageManager->GetMapW() || nz < 0 || nz >= stageManager->GetMapH()) continue;
		//if (stageManager->GetMap(nx, nz) ==1&& stageManager->GetMap(nx, nz) == 2) continue;

		nextX = nx;
		nextZ = nz;


	
	}
}


