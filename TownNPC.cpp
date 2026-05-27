#include "TownNPC.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/CsvReader.h"
#include "StageManager.h"
#include "Player.h"

TownNPC::TownNPC(GameObject* parent)
	: PaintObject(parent, "Enemy")
{

}

TownNPC::~TownNPC()
{
}

void TownNPC::Initialize()
{
	hModel_ = Model::Load("Models/enemy2.fbx");
	assert(hModel_ >= 0);

	state_ = NPC_START;

	SetBrushSize(0.5f);

	transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");

	mapW = stageManager->GetMapW();
	mapH = stageManager->GetMapH();

	direction_ = NPC_RIGHT;


	moveSpeed_ = 0.1f;
}

void TownNPC::Update()
{
	switch (state_)
	{
	case NPC_START:
		if (UpdateStart())
		{
			state_ = NPC_MOVE;
		}
		break;
	case NPC_MOVE:
		UpdateMove();
		break;
	case NPC_WAIT:
		break;
	case NPC_PAINTED:
		break;
	default:
		break;
	}

}

void TownNPC::Draw()
{
	Model::SetTransform(hModel_, transform_, GetMosaicRT()->GetShaderResourceView());
	Model::Draw(hModel_);
}

void TownNPC::Release()
{
}

bool TownNPC::UpdateMove()
{
	MoveRoute();

	if (currentX == goalX && currentZ == goalZ)
	{
		isGoal = true;
		state_ = NPC_START;
	}
	return true;
}

bool TownNPC::UpdateStart()
{
	ResetRouteSearch();
	SetGoal();
	routeQueue.push({ currentZ,currentX });
	SearchRoad();
	SetTargetPos();

	return true;
}

void TownNPC::ResetRouteSearch()
{
	//スポーンしたマス
	currentX = transform_.position_.x / 29.0f;
	currentZ = transform_.position_.z / 29.0f;
	currentZ = -currentZ;//マップの座標とワールド座標はz軸が逆なので

	isGoal = false;

	routeIndex_ = 0;

	parent.clear();//BFS探索のための親ノードを保存する配列
	routeQueue = std::queue<std::pair<int, int>>();//BFS探索のためのキュー
	visited.clear();//BFS探索のための訪問済み配列

	parent.clear();
	parent.resize(mapH, std::vector<std::pair<int, int>>(mapW, { -1,-1 }));

	visited.clear();
	visited.resize(mapH, std::vector<bool>(mapW, false));

	startX = currentX;
	startZ = currentZ;
}

void TownNPC::MoveRoute()
{
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTarget = XMLoadFloat3(&targetPos);
	XMVECTOR v = vTarget - vPos;

	// 正規化して進める
	v = XMVector3Normalize(v);
	vPos += v * moveSpeed_;
	XMStoreFloat3(&transform_.position_, vPos);

	float dist =
		XMVectorGetX(
			XMVector3Length(vTarget - vPos));

	if (dist < moveSpeed_)
	{
		transform_.position_ = targetPos;

		currentX = transform_.position_.x / 29.0f;
		currentZ = -transform_.position_.z / 29.0f;

		SetTargetPos();
		return;
	}

}

void TownNPC::SearchRoad()
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

			int map = stageManager->GetMap(nx, nz);

			if (map == 1 || map == 2)
			{
				// ゴールだけは許可したいなら別処理
				if (!(nx == goalX && nz == goalZ))
				{
					continue;
				}
			}

			if (visited[nz][nx]) continue;

			if (nx == goalX && nz == goalZ)
			{
				parent[nz][nx] = { z, x };
				CreateRoute();
				return;
			}

			visited[nz][nx] = true;
			parent[nz][nx] = { z, x };
			routeQueue.push({ nz, nx });
		}
	}

	// キューが空になってもゴール未到達＝経路なし
	OutputDebugStringA("route not found!\n");
}

void TownNPC::CreateRoute()
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

void TownNPC::SetTargetPos()
{
	targetPos.x = path[routeIndex_].second * 29.0f;
	targetPos.y = transform_.position_.y;
	targetPos.z = -path[routeIndex_].first * 29.0f;

	if (routeIndex_ < path.size() - 1)
		routeIndex_++;
}

void TownNPC::SetGoal()
{
	const auto& buildingList = stageManager->GetBuildingList();

	while (true)
	{
		int index = rand() % buildingList.size();

		int gx = buildingList[index].second;
		int gz = buildingList[index].first;

		if (gx == currentX && gz == currentZ)
		{
			continue;
		}

		goalX = gx;
		goalZ = gz;
		break;
	}


}


