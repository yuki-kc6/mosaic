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

	int mapW = stageManager->GetMapW();
	int mapH = stageManager->GetMapH();


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
		UpdateMove();
		//スタート地点を探す
		if (!isSerarchStarted)
		{
			for (int z = 0; z < mapH; z++)
			{
				for(int x=0; x<mapW; x++)
				{
					if(currentX == x && currentZ == z)
					{
						routeQueue.push({ z,x });
						visited[z][x] = true;
						isSerarchStarted = true;
						break;
					}
				}
			}
		}
		else if (!isRouteDecided)
		{
			//ルートが決まっていない場合はルートを決める
			SerarchRoad();
		}
		else
		{
			MoveNextTile();//ルートが決まったら移動を開始する
		}
		
		if(currentX==goalX && currentZ==goalZ)
		{
			state_ = ENEMY_WAIT;
		}

		break;
	case ENEMY_WAIT:
		//ビルの中で待機する

		//if()
		// {
		// setGoal();
		// state_=ENEMY_MOVE;
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

void Enemy::UpdateMove()
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

void Enemy::SerarchRoad()
{
	//BFS探索でルートを決める
	if (routeQueue.empty())return;

	auto p = routeQueue.front();
	routeQueue.pop();

	int z = p.first;
	int x = p.second;

	for (int dir = 0; dir < 4; dir++)
	{
		int nx = x + dx[dir];
		int nz = z + dz[dir];

		int map = stageManager->GetMap(nx, nz);

		if (nx < 0 || nx >= stageManager->GetMapW() || nz < 0 || nz >= stageManager->GetMapH()) continue;
		if (map ==1 || map == 2) continue;
		if (visited[nz][nx]) continue;

		visited[nz][nx] = true;
		parent[nz][nx] = { z, x};
		routeQueue.push({ nz, nx });

		if(nx==goalX && nz==goalZ)
		{
			isRouteDecided = true;

			break;
		}

	}
}

void Enemy::CreateRoute()
{
	//BFS探索で決定したルートをDirectionにする

}


