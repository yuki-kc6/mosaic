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
	

	targetPos.x = 10.0f;
	targetPos.y = transform_.position_.y;
	targetPos.z = 10.0f;
}

void Enemy::Update()
{
	//SetPositionの関係でもう一度初期化する
	if(!isInitialized_)
	{
		//Initialize();
		isInitialized_ = true;
	}

	//switch (state_)
	//{

	//case ENEMY_SPAWN:
	//	if (!isSerarchStarted)
	//	{
	//		startX = currentX;
	//		startZ = currentZ;
	//		routeQueue.push({ currentZ,currentX });
	//		visited[currentZ][currentX] = true;
	//		isSerarchStarted = true;
	//	}
	//	else
	//	{
	//		//ルートが決まっていない場合はルートを決める
	//		SearchRoad();
	//	}
	//	break;

	//case ENEMY_SETTARGETPOS:
	//	SetTargetPos();
	//	routeIndex_++;
	//	state_ = ENEMY_MOVE;
	//	break;

	//case ENEMY_MOVE:
	//	UpdateMove();
	//	break;

	//case ENEMY_WAIT:
	//	//ビルの中で待機する

	//	//if()
	//	// {
	//	// setGoal();
	//	// state_=ENEMY_MOVE;
	//	//}
	//	break;
	//case ENEMY_PAINTED:
	//	
	//	break;
	//default:
	//	break;
	//}   
	UpdateMove();
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
	//OutputDebugStringA(("pos: " + std::to_string(transform_.position_.x) + ", " + std::to_string(transform_.position_.z) + "\n").c_str());
	//OutputDebugStringA(("target: " + std::to_string(targetPos.x) + ", " + std::to_string(targetPos.z) + "\n").c_str());

	Player* player= (Player*)FindObject("Player");
	//targetPos.x = player->GetPosition().x;//プレイヤーの位置を目標にする
	//targetPos.z = player->GetPosition().z;//プレイヤーの位置を目標にする
	//targetPos.y = player->GetPosition().y;//プレイヤーの高さに合わせる


	MoveRoute();//道にそって移動する



	//マスの真ん中に到達したら次のマスを設定する
	float dx =
		targetPos.x -
		transform_.position_.x;

	float dz =
		targetPos.z -
		transform_.position_.z;

	float dist =
		sqrtf(dx * dx + dz * dz);

	if (dist < moveSpeed_)
	{
		transform_.position_ = targetPos;

		currentX = nextTargetX;
		currentZ = nextTargetZ;

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

	vPos += XMVectorScale(v, moveSpeed_);

	XMStoreFloat3(
		&transform_.position_,
		vPos
	);


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

			if (nowX > x)
				route.push_back(ENEMY_RIGHT);

			else if (nowX < x)
				route.push_back(ENEMY_LEFT);

			else if (nowZ > z)
				route.push_back(ENEMY_DOWN);

			else if (nowZ < z)
				route.push_back(ENEMY_UP);

		}
		std::reverse(route.begin(), route.end());
}

void Enemy::SetTargetPos()
{
	if (route.empty()) return;


	if (routeIndex_ >= route.size())
	{
		state_ = ENEMY_WAIT;
		return;
	}
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
		transform_.position_.y,
		-nextTargetZ * 29.0f
	};

}


