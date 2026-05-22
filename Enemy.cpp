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

	route.clear();
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
	moveSpeed_ = 0.5f;
}

void Enemy::Update()
{
	switch (state_)
	{

	case ENEMY_SPAWN:
		if (!isSerarchStarted)
		{
			startX = currentX;
			startZ = currentZ;
			routeQueue.push({ currentZ,currentX });
			visited[currentZ][currentX] = true;
			isSerarchStarted = true;
		}

		if (!isRouteDecided)
		{
			//ルートが決まっていない場合はルートを決める
			SearchRoad();
		}
		else
		{
			state_ = ENEMY_SETTARGETPOS;
		}
		break;

	case ENEMY_SETTARGETPOS:
		SetTargetPos();
		state_ = ENEMY_MOVE;
		break;

	case ENEMY_MOVE:
		UpdateMove();
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
	MoveRoute();//道にそって移動する

	//マスの真ん中に到達したら次のマスを設定する
	if (transform_.position_.x == targetPos.x && transform_.position_.z == targetPos.z)
	{
		state_ = ENEMY_SETTARGETPOS;
	}
}

void Enemy::MoveRoute()
{
	//targetPosを目指して移動する
	XMVECTOR vPos =
		XMLoadFloat3(&transform_.position_);

	XMVECTOR vTarget =
		XMLoadFloat3(&targetPos);

	XMVECTOR v =
		vTarget - vPos;

	v = XMVector3Normalize(v);

	vPos += v * moveSpeed_;

	XMStoreFloat3(
		&transform_.position_,
		vPos
	);

}

void Enemy::SearchRoad()
{
	//BFS探索をする
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

		if (nx == goalX && nz == goalZ)
		{
			isRouteDecided = true;
			CreateRoute();
			break;
		}

	}
}

void Enemy::CreateRoute()
{
	//BFS探索から最短経路を作成してrouteに入れる
	if (!isRouteDecided) return;
		int z= parent[goalZ][goalX].first;
		int x = parent[goalZ][goalX].second;

		while (z != startZ || x != startX)
		{
			int nowX = x;
			int nowZ = z;

			if (z < 0 || x < 0)
			{
				return;
			}

			z = parent[nowZ][nowX].first;
			x = parent[nowZ][nowX].second;

			if (currentX > x)
				route.push_back(ENEMY_RIGHT);

			else if (currentX < x)
				route.push_back(ENEMY_LEFT);

			else if (currentZ > z)
				route.push_back(ENEMY_DOWN);

			else if (currentZ < z)
				route.push_back(ENEMY_UP);

		}
		std::reverse(route.begin(), route.end());
}

void Enemy::SetTargetPos()
{
	if (route.empty()) return;

	direction_ = route[routeIndex_];

	nextTargetX = currentX;
	nextTargetZ = currentZ;

	//次のマスをrouteの中のDirectionに従って決める
	switch (route[routeIndex_])
	{
	case ENEMY_UP:
		transform_.rotate_.y = 0;
		nextTargetZ--;
		break;

	case ENEMY_DOWN:
		transform_.rotate_.y = 180;
		nextTargetZ++;
		break;

	case ENEMY_LEFT:
		transform_.rotate_.y = 270;
		nextTargetX--;
		break;

	case ENEMY_RIGHT:
		transform_.rotate_.y = 90;
		nextTargetX++;
		break;
	}

	targetPos =
	{
		nextTargetX * 29.0f,
		0,
		-nextTargetZ * 29.0f
	};

}


