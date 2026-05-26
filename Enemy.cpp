#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/CsvReader.h"
#include "StageManager.h"
#include "Player.h"

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

	state_ = ENEMY_MOVE;

    transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");

	mapW = stageManager->GetMapW();
	mapH = stageManager->GetMapH();

	//スポーンしたマス
	currentX = transform_.position_.x/29.0f;
	currentZ = transform_.position_.z/29.0f;
	currentZ = -currentZ;//マップの座標とワールド座標はz軸が逆なので

	direction_ = ENEMY_RIGHT;//とりあえず右に行くことにする

	//目標とするビルを決める
	//基本的には
	if (stageManager->GetMap(currentX + 2, currentZ) == 1)
	{
		if(currentX + 2 < mapW)
		{
			goalX = currentX + 2;
			goalZ = currentZ;
		}
	}
	else if (stageManager->GetMap(currentX - 2, currentZ) == 1)
	{
		if(currentX - 2 >= 0)
		{
			goalX = currentX - 2;
			goalZ = currentZ;
		}
	}
	routeIndex_ = 0;

	parent.clear();//BFS探索のための親ノードを保存する配列
	routeQueue = std::queue<std::pair<int, int>>();//BFS探索のためのキュー
	visited.clear();//BFS探索のための訪問済み配列

	parent.clear();
	parent.resize(
		mapH,
		std::vector<std::pair<int, int>>(
			mapW,
			{ -1,-1 }
		)
	);

	visited.clear();
	visited.resize(
		mapH,
		std::vector<bool>(
			mapW,
			false
		)
	);
	moveSpeed_ = 0.5;
	

	targetPos.x = 10.0f;
	targetPos.y = transform_.position_.y;
	targetPos.z = 10.0f;
}

void Enemy::Update()
{
	//SetPositionの関係でもう一度初期化する
	if(!isInitialized_)
	{
		Initialize();
		isInitialized_ = true;
		startX = currentX;
		startZ = currentZ;
		routeQueue.push({ currentZ,currentX });
		SearchRoad();

	}
	//transform_.position_.x += 1.0f;
	MoveRoute();

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

void Enemy::MoveRoute()
{
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTarget = XMLoadFloat3(&targetPos);
	XMVECTOR v = vTarget - vPos;

	// 正規化して進める
	v = XMVector3Normalize(v);
	vPos += XMVectorScale(v, moveSpeed_);
	XMStoreFloat3(&transform_.position_, vPos);

	float dist =
		XMVectorGetX(
			XMVector3Length(vTarget - vPos));

	if (dist < moveSpeed_)
	{
		transform_.position_ = targetPos;


		SetTargetPos();

		
		return;
	}


}

void Enemy::SearchRoad()
{
	//BFS探索をする
	if (routeQueue.empty())return;

	while (!routeQueue.empty())
	{
		auto p = routeQueue.front();
		routeQueue.pop();

		int z = p.first;
		int x = p.second;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int nz = z + dz[dir];

			if (nx < 0 || nx >= mapW || nz < 0 || nz >= mapH) continue;

			// ゴール到達
			if (nx == goalX && nz == goalZ)
			{
				parent[nz][nx] = { z, x };
				CreateRoute();
				state_ = ENEMY_SETTARGETPOS;
				return;
			}

			int map = stageManager->GetMap(nx, nz);
			if (map == 1 || map == 2) continue;
			if (visited[nz][nx]) continue;

			visited[nz][nx] = true;
			parent[nz][nx] = { z, x };
			routeQueue.push({ nz, nx });
		}
	}

	// キューが空になってもゴール未到達＝経路なし
	OutputDebugStringA("route not found!\n");
}

void Enemy::CreateRoute()
{
	int x = goalX;
	int z = goalZ;

	while (!(x == startX && z == startZ))
	{
		path.push_back({ z, x });

		auto p = parent[z][x];

		z = p.first;
		x = p.second;
	}

	path.push_back({ startZ, startX });

	std::reverse(path.begin(), path.end());

	routeIndex_ = 0;
}

void Enemy::SetTargetPos()
{
	targetPos.x = path[routeIndex_].second * 29.0f;
	targetPos.y = transform_.position_.y;
	targetPos.z = -path[routeIndex_].first * 29.0f;

	if(routeIndex_ < path.size() - 1)
	routeIndex_++;
}


