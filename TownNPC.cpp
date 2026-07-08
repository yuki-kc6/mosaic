#include "TownNPC.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/CsvReader.h"
#include "StageManager.h"
#include "Player.h"
namespace
{
	constexpr float MOVE_SPEED = 0.1f;//NPCの移動速度
	constexpr float NPC_SCALE = 0.05f;//NPCの大きさ
	constexpr float NPC_BRUSH_SIZE = 0.2f;//NPCのブラシサイズ
	constexpr int NPC_ANIM_START = 0;//NPCのアニメーション開始フレーム
	constexpr int NPC_ANIM_END = 30;//NPCのアニメーション終了フレーム
	constexpr float NPC_ANIM_SPEED = 0.5f;//NPCのアニメ速度
	constexpr float WALK_OFFSET = 0.1f;//歩く場所をずらすためのオフセット
	constexpr float NPC_ROTATE_UP = 0.0f;//STATE:UPの時の回転
	constexpr float NPC_ROTATE_RIGHT = 90.0f;//STATE:RIGHTの時の回転
	constexpr float NPC_ROTATE_DOWN = 180.0f;//STATE:DOWNの時の回転
	constexpr float NPC_ROTATE_LEFT = 270.0f;//STATE:LEFTの時の回転
}


TownNPC::TownNPC(GameObject* parent)
	: PaintObject(parent, "TownNPC"),  targetPos(0,0,0), state_(NPC_START), direction_(NPC_RIGHT), isGoal_(false), 
	routeIndex_(0),nextTargetX_(0),nextTargetZ_(0),goalX_(0),goalZ_(0),startX_(0),startZ_(0),currentX_(0),currentZ_(0),mapW_(0),mapH_(0),
	gridSize_(0),isRouteDecided_(false),isSerarchStarted_(false),moveSpeed_(0),stageManager(nullptr)
{

}

TownNPC::~TownNPC()
{
}

void TownNPC::Initialize()
{
	hModel_ = Model::Load("Models/Walking.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = { NPC_SCALE,NPC_SCALE,NPC_SCALE };

	state_ = NPC_START;//stateは最初Start

	SetBrushSize(NPC_BRUSH_SIZE);//ブラシサイズの設定

	transform_.position_.y = 1;

	stageManager = (StageManager*)FindObject("StageManager");

	gridSize_ = stageManager->GetGridSize();//StageManagerからgridSizeを取得

	mapW_ = stageManager->GetMapW();//マップの大きさを取得
	mapH_ = stageManager->GetMapH();

	direction_ = NPC_RIGHT;//directionの初期方向

	Model::SetAnimFrame(hModel_, NPC_ANIM_START, NPC_ANIM_END, NPC_ANIM_SPEED);//アニメーションのセット

	moveSpeed_ = MOVE_SPEED;//移動速度の初期化
}

void TownNPC::Update()
{
	//状態に合わせてUpdate
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
	//currentとgoalの座標が同じになったらstateを変更
	if (currentX_ == goalX_ && currentZ_ == goalZ_)
	{
		isGoal_ = true;
		state_ = NPC_START;
	}
	return true;
}

bool TownNPC::UpdateStart()
{
	//Updateが始まる前に道の探索や配列のリセット
	ResetRouteSearch();
	SetGoal();
	routeQueue_.push({ currentZ_,currentX_ });
	SearchRoad();
	direction_ = SetDirection();
	SetTargetPos(direction_);

	return true;
}

void TownNPC::ResetRouteSearch()
{
	if (isGoal_)
	{
		//ゴールに到達した際のリセットならスタート地点をゴールにする
		currentX_ = goalX_;
		currentZ_ = goalZ_;
	}
	else
	{
		//スポーンしたときのリセットなら現在の座標をスタート地点にする
		currentX_ = transform_.position_.x / gridSize_;
		currentZ_ = transform_.position_.z / gridSize_;
		currentZ_ = -currentZ_;//マップの座標とワールド座標はz軸が逆なので
	}
	

	isGoal_ = false;

	parent_.clear();//BFS探索のための親ノードを保存する配列
	routeQueue_= std::queue<std::pair<int, int>>();//BFS探索のためのキュー
	visited_.clear();//BFS探索のための訪問済み配列
	path_.clear();//経路の配列

	parent_.resize(mapH_, std::vector<std::pair<int, int>>(mapW_, { -1,-1 }));

	visited_.resize(mapH_, std::vector<bool>(mapW_, false));

	startX_ = currentX_;
	startZ_ = currentZ_;
}

void TownNPC::MoveRoute()
{
	//targetPosへのベクトルを計算
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTarget = XMLoadFloat3(&targetPos);
	XMVECTOR v = vTarget - vPos;

	// 正規化して進める
	v = XMVector3Normalize(v);

	//targetPosに向かって進める
	vPos += v * moveSpeed_;
	XMStoreFloat3(&transform_.position_, vPos);

	//現在離れている距離を計算
	float dist =XMVectorGetX(XMVector3Length(vTarget - vPos));

	//moveSpeedより小さくなったなら次のpathに進める
	if (dist < moveSpeed_)
	{
		transform_.position_ = targetPos;

		//現在の場所を更新
		currentX_ = path_[routeIndex_].second;
		currentZ_ = path_[routeIndex_].first;

		routeIndex_++;

		//まだpathを進められるなら道を探してdirection更新
		if (routeIndex_ < path_.size())
		{
			direction_ = SetDirection();
			SetTargetPos(direction_);
		}
	}

}

void TownNPC::SearchRoad()
{
	//空なら返す
	if (routeQueue_.empty())return;

	//BFS探索をする
	while (!routeQueue_.empty())
	{
		//queueの先頭をもらう
		auto p = routeQueue_.front();
		routeQueue_.pop();

		int z = p.first;
		int x = p.second;

		for (int dir = 0; dir < 4; dir++)
		{
			//次の場所を計算
			int nx = x + dx[dir];
			int nz = z + dz[dir];

			//マップ外なら飛ばす
			if (nx < 0 || nx >= mapW_ || nz < 0 || nz >= mapH_) continue;

			int map = stageManager->GetMap(nx, nz);

			//歩道かどうか
			if (map == MAP_BUILDING || map == MAP_ROAD)
			{
				// ゴールじゃないなら飛ばす
				if (!(nx == goalX_ && nz == goalZ_))
				{
					continue;
				}
			}

			//来た道なら飛ばす
			if (visited_[nz][nx]) continue;

			//ゴールなら終了して最短経路を作成する
			if (nx == goalX_ && nz == goalZ_)
			{
				parent_[nz][nx] = { z, x };
				CreateRoute();
				return;
			}

			//通ったことを記録
			visited_[nz][nx] = true;
			parent_[nz][nx] = { z, x };
			routeQueue_.push({ nz, nx });
		}
	}
}

void TownNPC::CreateRoute()
{
	//最初にGoalの座標を入れる
	int x = goalX_;
	int z = goalZ_;

	//start座標までparentにそってpathを作る。
	while (!(x == startX_ && z == startZ_))
	{
		path_.push_back({ z, x });

		auto p = parent_[z][x];

		z = p.first;
		x = p.second;
	}

	//最後にstartを入れる
	path_.push_back({ startZ_, startX_ });

	//反転する
	std::reverse(path_.begin(), path_.end());

	//routeIndexの初期化
	routeIndex_ = 1;
}

void TownNPC::SetTargetPos(NPCDirection dir)
{
	//pathの座標を3D空間に直す
	nextTargetX_ = path_[routeIndex_].second * gridSize_;
	nextTargetZ_ = -path_[routeIndex_].first * gridSize_;

	//directionに合わせてそれぞれオフセットを足す
	switch (dir)
	{
	case NPC_UP:
		targetPos.x = nextTargetX_ + WALK_OFFSET;
		targetPos.z = nextTargetZ_;
		transform_.rotate_.y = NPC_ROTATE_UP;
		break;

	case NPC_RIGHT:
		targetPos.x = nextTargetX_;
		targetPos.z = nextTargetZ_ + WALK_OFFSET;
		transform_.rotate_.y = NPC_ROTATE_RIGHT;
		break;

	case NPC_DOWN:
		targetPos.x = nextTargetX_ - WALK_OFFSET;
		targetPos.z = nextTargetZ_;
		transform_.rotate_.y = NPC_ROTATE_DOWN;
		break;

	case NPC_LEFT:
		targetPos.x = nextTargetX_;
		targetPos.z = nextTargetZ_- WALK_OFFSET;
		transform_.rotate_.y = NPC_ROTATE_LEFT;
		break;
	}
}

void TownNPC::SetGoal()
{
	//ビルのリストを取得
	const auto& buildingList = stageManager->GetBuildingList();

	while (true)
	{
		//ビルのランダムなindexを取る
		int index = rand() % buildingList.size();

		//設定
		int gx = buildingList[index].second;
		int gz = buildingList[index].first;

		//現在地ならやり直し
		if (gx == currentX_ && gz == currentZ_)
		{
			continue;
		}

		//ゴールに入れる
		goalX_ = gx;
		goalZ_ = gz;
		break;
	}


}

NPCDirection TownNPC::SetDirection()
{
	//次の座標と現在の座標の比べてxとzどちらに伸びているか計算する。斜めはないので四方向で
	int dx = path_[routeIndex_].second - currentX_;
	int dz = path_[routeIndex_].first - currentZ_;

	if (dx > 0)
		return  NPC_RIGHT;
	else if (dx < 0)
		return  NPC_LEFT;
	else if (dz > 0)
		return  NPC_DOWN;
	else if (dz < 0)
		return  NPC_UP;
}