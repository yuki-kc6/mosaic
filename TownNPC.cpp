#include "TownNPC.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/CsvReader.h"
#include "StageManager.h"
#include "Player.h"

namespace
{
	constexpr float MOVE_SPEED = 0.1f;
	constexpr float NPC_SCALE = 0.05f;
	constexpr float NPC_BRUSH_SIZE = 0.2f;
	constexpr int NPC_ANIM_START = 0;
	constexpr int NPC_ANIM_END = 30;
	constexpr float NPC_ANIM_SPEED = 0.5f;
	constexpr float NPC_MOVE_SPEED = 0.1f;
	constexpr float WALK_OFFSET = 0.1f;
	constexpr float NPC_ROTATE_UP = 0.0f;
	constexpr float NPC_ROTATE_RIGHT = 90.0f;
	constexpr float NPC_ROTATE_DOWN = 180.0f;
	constexpr float NPC_ROTATE_LEFT = 270.0f;
}


TownNPC::TownNPC(GameObject* parent)
	: PaintObject(parent, "TownNPC")
{

}

TownNPC::~TownNPC()
{
}

void TownNPC::Initialize()
{
	hModel_ = Model::Load("Models/manekin.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = { NPC_SCALE,NPC_SCALE,NPC_SCALE };

	state_ = NPC_START;

	SetBrushSize(NPC_BRUSH_SIZE);

	transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");

	gridSize = stageManager->GetGridSize();

	mapW = stageManager->GetMapW();
	mapH = stageManager->GetMapH();

	direction_ = NPC_RIGHT;

	Model::SetAnimFrame(hModel_, NPC_ANIM_START, NPC_ANIM_END, NPC_ANIM_SPEED);

	moveSpeed_ = MOVE_SPEED;
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
	direction_ = SetDirection();
	SetTargetPos(direction_);

	return true;
}

void TownNPC::ResetRouteSearch()
{
	if (isGoal)
	{
		//ゴールに到達した際のリセットならスタート地点をゴールにする
		currentX = goalX;
		currentZ = goalZ;
	}
	else
	{
		//スポーンしたときのリセットなら現在の座標をスタート地点にする
		currentX = transform_.position_.x / gridSize;
		currentZ = transform_.position_.z / gridSize;
		currentZ = -currentZ;//マップの座標とワールド座標はz軸が逆なので
	}
	

	isGoal = false;

	routeIndex_ = 0;

	parent.clear();//BFS探索のための親ノードを保存する配列
	routeQueue = std::queue<std::pair<int, int>>();//BFS探索のためのキュー
	visited.clear();//BFS探索のための訪問済み配列
	path.clear();//経路の配列

	parent.resize(mapH, std::vector<std::pair<int, int>>(mapW, { -1,-1 }));

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

		currentX = path[routeIndex_].second;
		currentZ = path[routeIndex_].first;

		routeIndex_++;

		if (routeIndex_ < path.size())
		{
			direction_ = SetDirection();
			SetTargetPos(direction_);
		}
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

			if (map == MAP_BUILDING || map == MAP_ROAD)
			{
				// ゴールだけは許可したい
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

	routeIndex_ = 1;
}

void TownNPC::SetTargetPos(NPCDirection dir)
{

	nextTargetX = path[routeIndex_].second * gridSize;
	nextTargetZ = -path[routeIndex_].first * gridSize;

	switch (dir)
	{
	case NPC_UP:
		targetPos.x = nextTargetX + WALK_OFFSET;
		targetPos.z = nextTargetZ;
		transform_.rotate_.y = NPC_ROTATE_UP;
		break;

	case NPC_RIGHT:
		targetPos.x = nextTargetX;
		targetPos.z = nextTargetZ + WALK_OFFSET;
		transform_.rotate_.y = NPC_ROTATE_RIGHT;
		break;

	case NPC_DOWN:
		targetPos.x = nextTargetX - WALK_OFFSET;
		targetPos.z = nextTargetZ;
		transform_.rotate_.y = NPC_ROTATE_DOWN;
		break;

	case NPC_LEFT:
		targetPos.x = nextTargetX;
		targetPos.z = nextTargetZ - WALK_OFFSET;
		transform_.rotate_.y = NPC_ROTATE_LEFT;
		break;
	}
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

NPCDirection TownNPC::SetDirection()
{
	int dx = path[routeIndex_].second - currentX;
	int dz = path[routeIndex_].first - currentZ;

	if (dx > 0)
		return  NPC_RIGHT;
	else if (dx < 0)
		return  NPC_LEFT;
	else if (dz > 0)
		return  NPC_DOWN;
	else if (dz < 0)
		return  NPC_UP;
}